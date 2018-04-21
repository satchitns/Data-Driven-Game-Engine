#include "stdafx.h"
#include "TestSubscriber.h"
#include "EventPublisher.h"
#include "Monster.h"
#include"Event.h"
#include "Foo.h"
#include <random>
#include <Event.h>

namespace UnitTestLibraryDesktop
{
	void TestSubscriber::Notify(const FieaGameEngine::EventPublisher & event)
	{
		if (event.Is(FieaGameEngine::Event<Monster>::TypeIdClass()))
		{
			mMonsterCount++;
		}
		else if (event.Is(FieaGameEngine::Event<Foo>::TypeIdClass()))
		{
				FieaGameEngine::Event<Foo>::Unsubscribe(*this);
				auto ptr = std::make_shared<FieaGameEngine::Event<int>>(mCount);
				queue->Enqueue(ptr, time, std::chrono::milliseconds(0));
		}
		else
		{
			mIntCount++;
		}

		event;
		mCount++;
	}
}
