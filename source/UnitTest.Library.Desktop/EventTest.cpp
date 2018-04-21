#include "stdafx.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "json\json.h"
#include <fstream>
#include "FooJsonParseHelper.h"
#include "JsonParseMaster.h"
#include "JsonParseHelperTable.h"
#include "TableSharedData.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "Factory.h"
#include "Monster.h"
#include "WorldState.h"
#include "DebugAction.h"
#include "ActionList.h"
#include "ActionListIf.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionExpression.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "Event.h"
#include "TestSubscriber.h"
#include "EventQueue.h"
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventTest)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
			Event<int>::sSubscribers.Reserve(10000);
			Event<Foo>::sSubscribers.Reserve(10000);
			Event<Monster>::sSubscribers.Reserve(10000);
#ifdef _DEBUG
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#ifdef _DEBUG
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(EventBasic)
		{
			int x;
			Event<int> xEvent(x);
			Assert::AreEqual(xEvent.Message(), x);
			TestSubscriber t, t1;
			Assert::AreEqual(t.mCount, 0);
			Event<int>::Subscribe(t);
			xEvent.Deliver();
			Assert::AreEqual(t.mCount, 1);
			Event<int>::Subscribe(t1);
			xEvent.Deliver();
			Assert::AreEqual(t.mCount, 2);
			Assert::AreEqual(t1.mCount, 1);
			Event<int>::Unsubscribe(t);
			xEvent.Deliver();
			Assert::AreEqual(t.mCount, 2);
			Assert::AreEqual(t1.mCount, 2);
			Event<int>::UnsubscribeAll();
			xEvent.Deliver();
			Assert::AreEqual(t1.mCount, 2);
		}

		TEST_METHOD(EventQueueTest)
		{
			EventQueue queue;
			int x, y;
			Event<int> xEvent(x);
			auto xPtr = std::make_shared<Event<int>>(x), yPtr = std::make_shared<Event<int>>(y);
			TestSubscriber intEater;
			Event<int>::Subscribe(intEater);
			GameClock clock;
			GameTime time;
			clock.UpdateGameTime(time);
			Assert::IsTrue(queue.IsEmpty());
			queue.Enqueue(xPtr, time, std::chrono::milliseconds(2));
			queue.Enqueue(yPtr, time);
			Assert::IsFalse(queue.IsEmpty());
			Assert::AreEqual(size_t(2), queue.Size());
			queue.Update(time);
			Assert::AreEqual(size_t(1), queue.Size());
			Assert::AreEqual(1, intEater.mCount);
			auto newTime = time.CurrentTime();
			newTime += std::chrono::milliseconds(2);
			time.SetCurrentTime(newTime);
			queue.Update(time);
			Assert::AreEqual(size_t(0), queue.Size());
			Assert::AreEqual(2, intEater.mCount);
			Event<int>::Unsubscribe(intEater);
		}

		TEST_METHOD(EventQueueRTTITest)
		{
			EventQueue queue;
			Monster m;
			int x;
			TestSubscriber t;
			auto xPtr = std::make_shared<Event<int>>(x);
			auto mPtr = std::make_shared<Event<Monster>>(m);
			Event<int>::Subscribe(t);
			Event<Monster>::Subscribe(t);
			GameClock clock;
			GameTime time;
			clock.UpdateGameTime(time);
			queue.Enqueue(mPtr, time, std::chrono::milliseconds(5));
			queue.Enqueue(xPtr, time);
			queue.Update(time);
			Assert::AreEqual(t.mCount, 1);
			Assert::AreEqual(t.mMonsterCount, 0);
			auto newTime = time.CurrentTime();
			newTime += std::chrono::milliseconds(5);
			time.SetCurrentTime(newTime);
			queue.Update(time);
			Assert::AreEqual(t.mCount, 2);
			Assert::AreEqual(t.mMonsterCount, 1);
			Event<int>::UnsubscribeAll();
			Event<Monster>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueSendTest)
		{
			EventQueue queue;
			Monster m;
			int x;
			TestSubscriber t;
			auto xPtr = std::make_shared<Event<int>>(x);
			GameClock clock;
			GameTime time;
			clock.UpdateGameTime(time);
			xPtr->SetTime(time.CurrentTime());
			Event<int>::Subscribe(t);
			queue.Send(xPtr);
			Assert::AreEqual(t.mCount, 1);
			Assert::IsTrue(xPtr->Delay() == std::chrono::milliseconds::zero());
			Assert::IsTrue(time.CurrentTime() == xPtr->TimeEnqueued());
			queue.Clear();
			Event<int>::UnsubscribeAll();
		}

		TEST_METHOD(EventQueueMultiStressTest)
		{
			EventQueue queue;
			Monster m, y;
			int x;
			Foo a;
			Vector<std::shared_ptr<Event<Monster>>> eventPtrs;
			Vector<std::shared_ptr<Event<int>>> intEventPtrs;
			Vector<std::shared_ptr<Event<Foo>>> fooEventPtrs;
			for (int i = 0; i < 200; ++i)
			{
				eventPtrs.PushBack(std::make_shared<Event<Monster>>(m));
				intEventPtrs.PushBack(std::make_shared<Event<int>>(x));
			}
			for (int i = 0; i < 5; i++)
			{
				fooEventPtrs.PushBack(std::make_shared<Event<Foo>>(a));
			}
			auto yPtr = std::make_shared<Event<Monster>>(y);
			Vector<TestSubscriber> intEaters;
			intEaters.Resize(100);
			GameTime time;
			for (auto& intEater : intEaters)
			{
				intEater.queue = &queue;
				Event<Monster>::Subscribe(intEater);
				Event<int>::Subscribe(intEater);
				Event<Foo>::Subscribe(intEater);
			}

			GameClock clock;
			clock.UpdateGameTime(time);
			Assert::IsTrue(queue.IsEmpty());
			for (auto& ptr : eventPtrs)
			{
				queue.Enqueue(ptr, time, std::chrono::milliseconds(0));
			}
			for (auto& ptr : intEventPtrs)
			{
				queue.Enqueue(ptr, time, std::chrono::milliseconds(0));
			}
			for (auto& ptr : fooEventPtrs)
			{
				queue.Enqueue(ptr, time, std::chrono::milliseconds(0));
			}
			queue.Enqueue(yPtr, time);
			queue.Update(time);
			auto newTime = time.CurrentTime();
			newTime += std::chrono::milliseconds(20);
			time.SetCurrentTime(newTime);
			queue.Update(time);
			Assert::AreEqual(intEaters[0].mMonsterCount, 201);
			Assert::AreNotEqual(intEaters[0].mIntCount, 201);
			Event<Monster>::UnsubscribeAll();
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState EventTest::sStartMemState;
}