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

	TEST_CLASS(WorldParseTest)
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
		
		TEST_METHOD(WorldJsonParse)
		{
			MonsterFactory monsterFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/level.json";
			master.ParseFromFile(path);
			Datum& sectors = world->Sectors();
			Sector* sector = static_cast<Sector*>(sectors.Get<Scope*>());
			Assert::AreEqual(sector->Find("Size")->Get<int>(), 4000);
			Entity* entity = static_cast<Entity*>(sector->Entities().Get<Scope*>());
			Assert::IsTrue(entity->Is("Monster"));
			Monster* monster = static_cast<Monster*>(entity);
			Assert::AreEqual(100, monster->Find("Health")->Get<int>());
			Assert::IsTrue(sector == entity->GetParent());
			Assert::IsTrue(world == sector->GetParent());
			Assert::IsTrue(sector == entity->GetSector());
			Assert::IsTrue(world == sector->GetWorld());
			Assert::IsTrue(world->Name() == "World1"s);
			Assert::IsTrue(sector->Name() == "Sector1"s);
			Assert::IsTrue(monster->Name() == "BlueMonster"s);
			delete world;
		}

		TEST_METHOD(WorldUpdate)
		{
			MonsterFactory monsterFactory;
			GameTime time;
			GameClock clock;
			World *world = new World(time, clock);
			world->SetName("BobWorld");
			TableSharedData data(*world);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/level.json";
			master.ParseFromFile(path);
			WorldState state;
			world->Update(state);
			GameTime *gameTime = state.GetGameTime();
			gameTime;
			delete world;
		}

		TEST_METHOD(BadEntityTest)
		{
			Sector sector;
			auto exprn = [&sector]() { sector.CreateEntity("CuddleMonster", "MyCuddleMonster"); };
			Assert::ExpectException<exception>(exprn);
		}

		TEST_METHOD(EntityCopyMove)
		{
			Monster m1;
			m1.SetName("Bob");
			m1.Find("Health")->Set(45);
			Monster m2;
			m2 = m1;
			Monster m3(m1);
			Monster m4;
			m4 = std::move(m2);
			Monster m5(std::move(m4));
			Assert::AreEqual(m5.Find("Health")->Get<int>(), 45);
		}

		TEST_METHOD(SectorCopyMove)
		{
			MonsterFactory mF;
			Sector s1;
			s1.SetName("Sector1");
			s1.CreateEntity("Monster", "NewMonster");
			Sector s2;
			s2 = s1;
			Scope* s = s2.Entities().Get<Scope*>();
			Sector s3(s1);
			Sector s4;
			s4 = std::move(s2);
			Sector s5(std::move(s4));
			Assert::IsTrue(s5.Name() ==  "Sector1"s);
			Assert::IsTrue(s5.Entities().Get<Scope*>() == s);
		}

		TEST_METHOD(WorldCopyMove)
		{
			GameTime t;
			GameClock c;
			World s1(t, c);
			s1.SetName("World1");
			s1.CreateSector("FirstSector");
			World s2(t, c);
			s2 = s1;
			Scope * s = s2.Sectors().Get<Scope*>();
			World s3(s1);
			World s4(t, c);
			s4 = std::move(s2);
			World s5(std::move(s4));
			Assert::IsTrue(s5.Name() == "World1"s);
			Assert::IsTrue(s5.Sectors().Get<Scope*>() == s);
		}

	private:
		static _CrtMemState sStartMemState;
		GameTime dummyTime;
		GameClock dummyClock;
		World dummyWorld = { dummyTime, dummyClock };
		Sector dummySector;
		Entity dummyEntity;
		Monster dummyMonster;
	};
	_CrtMemState WorldParseTest::sStartMemState;
}