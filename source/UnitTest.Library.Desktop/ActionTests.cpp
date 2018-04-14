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

		TEST_CLASS(ActionTests)
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

		TEST_METHOD(ActionWorldJsonParse)
		{
			MonsterFactory monsterFactory;
			DebugActionFactory debugActionFactory;
			ActionListIfFactory ifActionFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/ActionLevel.json";
			master.ParseFromFile(path);
			Datum& sectors = world->Sectors();
			Sector* sector = static_cast<Sector*>(sectors.Get<Scope*>());
			Entity* entity = static_cast<Entity*>(sector->Entities().Get<Scope*>());
			Action* action = static_cast<Action*>(entity->Actions().Get<Scope*>());
			Assert::IsTrue(action->Is(DebugAction::TypeIdClass()));
			DebugAction *debug = static_cast<DebugAction*>(action);
			Assert::IsTrue("hello" == debug->String());
			delete world;
		}

		TEST_METHOD(ActionWorldConditionalTest)
		{
			MonsterFactory monsterFactory;
			DebugActionFactory debugActionFactory;
			ActionListIfFactory ifActionFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/ActionLevel.json";
			master.ParseFromFile(path);
			Datum& sectors = world->Sectors();
			Sector* sector = static_cast<Sector*>(sectors.Get<Scope*>());
			Entity* entity = static_cast<Entity*>(sector->Entities().Get<Scope*>());
			Action* action = static_cast<Action*>(entity->Actions().Get<Scope*>(1));
			Assert::IsTrue(action->Is(ActionListIf::TypeIdClass()));
			ActionListIf *ifAction = static_cast<ActionListIf*>(action);
			Assert::AreEqual(ifAction->Condition(), 1);
			Datum& ifDatum = ifAction->GetIf();
			Datum& elseDatum = ifAction->GetElse();
			Scope *ifScope = ifDatum.Get<Scope*>();
			Scope *elseScope = elseDatum.Get<Scope*>();
			DebugAction *ifCase = static_cast<DebugAction*>(ifScope);
			DebugAction *elseCase = static_cast<DebugAction*>(elseScope);
			Assert::AreEqual(ifCase->String().c_str(), "ifDebug");
			Assert::AreEqual(elseCase->String().c_str(), "elseDebug");
			delete world;
		}

		TEST_METHOD(ActionWorldUpdate)
		{
			MonsterFactory monsterFactory;
			DebugActionFactory debugActionFactory;
			ActionListIfFactory ifActionFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			world->SetName("BobWorld");
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/ActionLevel.json";
			master.ParseFromFile(path);
			world->Update();

			Datum& sectors = world->Sectors();
			Sector* sector = static_cast<Sector*>(sectors.Get<Scope*>());
			Entity* entity = static_cast<Entity*>(sector->Entities().Get<Scope*>());
			Action* action = static_cast<Action*>(entity->Actions().Get<Scope*>(1));
			Assert::IsTrue(action->Is(ActionListIf::TypeIdClass()));
			ActionListIf *ifAction = static_cast<ActionListIf*>(action);
			ifAction->Condition() = 0;

			world->Update();

			WorldState state;
			GameTime *gameTime = state.GetGameTime();
			gameTime;
			delete world;
		}

		TEST_METHOD(ActionMoveTest)
		{
			DebugAction *action1 = new DebugAction(), action2;
			action1->SetName("hello");
			action1->String() = "Debug";
			action2 = std::move(*action1);
			Assert::IsTrue("hello" == action2.Name());
			DebugAction action3(std::move(action2));
			Assert::IsTrue("hello" == action3.Name());
			delete action1;
		}

		TEST_METHOD(ActionExpressionMoveTest)
		{
			ActionExpression a1;
			a1.Result.RightToLeft = true;
			ActionExpression a2(std::move(a1)), a3;
			a3 = std::move(a2);
			Assert::IsTrue(a3.Result.RightToLeft);
		}

		TEST_METHOD(ActionListMoveTest)
		{
			DebugActionFactory factory;
			ActionList actionList;
			actionList.SetName("bob");
			actionList.CreateAction("DebugAction", "1");
			ActionList actionList2, actionList3(std::move(actionList));
			actionList2 = std::move(actionList3);
			Assert::IsTrue(actionList2.Name() == "bob");
			auto exprn = [&actionList2] { actionList2.CreateAction("FakeAction", "fakename"); };
			Assert::ExpectException<exception>(exprn);
			Datum& datum = actionList2.Actions();
			Assert::IsTrue(datum.Get<Scope*>(0)->Is(DebugAction::TypeIdClass()));
			WorldState state;
			actionList2.Update(state);
		}

		TEST_METHOD(ActionListIfMoveTest)
		{
			ActionListIf actionList;
			actionList.SetName("bob");
			ActionListIf actionList2, actionList3(std::move(actionList));
			actionList2 = std::move(actionList3);
			Assert::IsTrue(actionList2.Name() == "bob");
		}

		TEST_METHOD(DestroyCreateActionTests)
		{
			DebugActionFactory factory;
			Entity *entity = new Entity();
			Sector *sector = new Sector();
			sector->Adopt(entity, "Entities");
			GameTime time;
			GameClock clock;
			World world(time, clock);
			world.Adopt(sector, "Sectors");
			ActionDestroyAction des, des1, des2(std::move(des));
			ActionCreateAction crea, crea1, crea2(std::move(crea));

			des1 = std::move(des2);
			crea1 = std::move(crea2);

			ActionList *a = new ActionList();
			entity->Adopt(a, Entity::sActions);
			a->Adopt(&crea1, Entity::sActions);
			a->Adopt(&des1, Entity::sActions);
			a->SetName("List");

			crea1.Prototype() = "DebugAction";
			crea1.InstanceName() = "TestAction";

			WorldState state;
			crea1.Update(state);
			Assert::IsTrue(a->Actions().Size() == 3U);
			des1.InstanceName() = "List";
			a->Relinquish(&crea1);
			Assert::IsTrue(a->Actions().Size() == 2U);
			world.Update();
			a->Relinquish(&des1);
			world.Update();
			Assert::IsTrue(entity->Actions().Size() == 0U);
		}

		TEST_METHOD(ActionExpressionExpressionTest)
		{
			ActionExpression e;
			WorldState state;
			e.String() = "2 * ( 2 * ( 3 + 4 ) )";
			e.Update(state);
			Assert::AreEqual(28.0f, e.Result.TokenValue);

			e.String() = "12/(5.0%(1+1))";
			e.Update(state);
			Assert::AreEqual(12.0f, e.Result.TokenValue);

			e.String() = "6>(3-2*9)";
			e.Update(state);
			Assert::AreEqual(1.0f, e.Result.TokenValue);

			e.String() = "6>=(3-2*9)";
			e.Update(state);
			Assert::AreEqual(1.0f, e.Result.TokenValue);

			e.String() = "6<=7";
			e.Update(state);
			Assert::AreEqual(1.0f, e.Result.TokenValue);

			e.String() = "6<7";
			e.Update(state);
			Assert::AreEqual(1.0f, e.Result.TokenValue);

			e.String() = "(3+3)==(2*3)";
			e.Update(state);
			Assert::AreEqual(1.0f, e.Result.TokenValue);

			e.String() = "!((3+3)==(2*3))";
			e.Update(state);
			Assert::AreEqual(0.0f, e.Result.TokenValue);

			e.String() = "!((3+3+1)!=(2*3))";
			e.Update(state);
			Assert::AreEqual(0.0f, e.Result.TokenValue);
		}

		TEST_METHOD(ActionExprnFn)
		{
			ActionExpression e;
			WorldState state;
			e.String() = "sine(0.0)";
			e.Update(state);
			Assert::AreEqual(0.0f, e.Result.TokenValue);

			e.String() = "cos(5.0)";
			e.Update(state);
			Assert::AreEqual((float)std::cos(5), e.Result.TokenValue);

			e.String() = "sqrt(25.0)";
			e.Update(state);
			Assert::AreEqual((float)std::sqrt(25), e.Result.TokenValue);
			e.String() = "pow((5)3)";
			e.Update(state);
			Assert::AreEqual(125.0f, e.Result.TokenValue);
		}

		TEST_METHOD(ActionExpressionParse)
		{
			ActionExpressionFactory f;
			MonsterFactory monsterFactory;
			DebugActionFactory debugActionFactory;
			ActionListIfFactory ifActionFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/Exprn.json";
			master.ParseFromFile(path);
			Datum& sectors = world->Sectors();
			Sector* sector = static_cast<Sector*>(sectors.Get<Scope*>());
			Entity* entity = static_cast<Entity*>(sector->Entities().Get<Scope*>());
			Action* action = static_cast<Action*>(entity->Actions().Get<Scope*>());
			Assert::IsTrue(action->Is(ActionExpression::TypeIdClass()));
			ActionExpression *debug = static_cast<ActionExpression*>(action);
			world->Update();
			Assert::AreEqual(debug->Result.TokenValue, 320.0f);
			float flo = entity->Find("Multipliers")->Get<float>(1);
			Assert::IsTrue(flo == 190.0f);
			Assert::AreEqual(entity->Find("Health")->Get<int>(), 320);
			entity->Find("Multipliers")->Set(50.0f, 3);
			world->Update();
			Assert::AreEqual(debug->Result.TokenValue, 400.0f);
			delete world;
		}

		TEST_METHOD(ActionClones)
		{
			ActionList  *a = new ActionList();
			ActionListIf *b = new ActionListIf();
			ActionExpression* c = new ActionExpression();
			DebugAction *d = new DebugAction();
			ActionCreateAction *e = new ActionCreateAction();
			ActionDestroyAction *f = new ActionDestroyAction();
			Scope x;
			x.Adopt(a, "Scopes");
			x.Adopt(b, "Scopes");
			x.Adopt(c, "Scopes");
			x.Adopt(d, "Scopes");
			x.Adopt(e, "Scopes");
			x.Adopt(f, "Scopes");
			int a11 = x.Find("Scopes")->Size();
			a11;
			Scope x2(x);
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState ActionTests::sStartMemState;
}