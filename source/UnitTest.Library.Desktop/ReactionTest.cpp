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
#include "ActionEvent.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace FieaGameEngine
{
	template<typename T>
	const T & Datum::Get(uint32_t index) const
	{
		index;
		return T();
	}
}

namespace UnitTestLibraryDesktop
{
		ConcreteFactory(Monster, Entity)
		ConcreteFactory(DebugAction, Action)
		ConcreteFactory(ActionList, Action)
		ConcreteFactory(ActionCreateAction, Action)
		ConcreteFactory(ActionDestroyAction, Action)
		ConcreteFactory(ActionListIf, Action)
		ConcreteFactory(ActionExpression, Action)
		ConcreteFactory(ActionEvent, Action)
		ConcreteFactory(ReactionAttributed, Reaction)

		TEST_CLASS(RectionTests)
	{
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
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

		TEST_METHOD(ReactionParseTest)
		{
			MonsterFactory monsterFactory;
			DebugActionFactory debugActionFactory;
			ReactionAttributedFactory raf;
			ActionEventFactory af;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/Reaction.json";
			master.ParseFromFile(path);
			Scope* reactionScope = world->Find("Reactions")->Get<Scope*>();
			Assert::IsTrue(reactionScope->Is(ReactionAttributed::TypeIdClass()));
			ReactionAttributed* reactionAttr = static_cast<ReactionAttributed*>(reactionScope);
			Scope * debugScope = reactionAttr->Find("Actions")->Get<Scope*>();
			Assert::IsTrue(debugScope->Is(DebugAction::TypeIdClass()));
			DebugAction* debugAction = static_cast<DebugAction*>(debugScope);
			Assert::IsTrue(debugAction->String() == "hello");
			Assert::IsFalse(debugAction->UpdateCalled);
			world->Update();
			Assert::IsFalse(debugAction->UpdateCalled);
			world->Update();
			Assert::IsTrue(debugAction->UpdateCalled);
			world->Reactions();
			delete world;
		}

		TEST_METHOD(ReactionAttributedCopies)
		{
			ReactionAttributed a;
			ReactionAttributed b;
			b = a;
			ReactionAttributed d(b);
		}

		TEST_METHOD(ReactionAttributedMoves)
		{
			ReactionAttributed a;
			ReactionAttributed b;
			b = std::move(a);
			ReactionAttributed d(std::move(b));
			a.SetSubtype("mango");
			a.GetSubtype();
		}
		
		TEST_METHOD(NewClones)
		{
			ActionEvent *a = new ActionEvent();
			ReactionAttributed *c = new ReactionAttributed();
			EventMessageAttributed* d = new EventMessageAttributed();
			Scope x;
			x.Adopt(a, "Scopes");
			x.Adopt(d, "Scopes");
			x.Adopt(c, "Scopes");
			Scope x2(x);
		}

	private:
		_CrtMemState RectionTests::sStartMemState;
	};
}