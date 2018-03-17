#include "stdafx.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "json\json.h"
#include <fstream>
#include "FooJsonParseHelper.h"
#include "JsonParseMaster.h"
#include "AssertHelp.h"
#include "JsonParseHelperTable.h"
#include "TableSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(JsonParseTests)
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

		TEST_METHOD(JsonCPPBasicTest)
		{
			stringstream data(R"({"integer":{"char":1, "int":2}})");
			Json::Value root;
			data >> root;
			stringstream a;
			Json::Value v = root["integer"];
			a << v;
			int u = root.size();
			u = v.size();
			u;
			string b = a.str();
			Json::Value x = v["char"];
			string newString = x.asString();
			newString;
			b = "Random";
		}

		TEST_METHOD(FooJsonHelperStringTest)
		{
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster master(data);
			FooJsonParseHelper helper;
			helper.Initialize();
			master.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			master.Parse(s);
			Assert::AreEqual(Foo(1, 2), data.GetFoo());
		}

		TEST_METHOD(FooJsonHelperFileTest)
		{
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster master(data);
			FooJsonParseHelper helper;
			master.AddHelper(helper);
			string path = "Content/a.txt";
			master.ParseFromFile(path);
			master.RemoveHelper(helper);
			master.Parse("{}{}{}{}{}{}");
			Assert::AreEqual(master.GetFilePath(), path);
			Assert::AreEqual(Foo(1, 2), data.GetFoo());
		}

		TEST_METHOD(JsonParseMasterCloningExperiments)
		{
			JsonParseMaster::SharedData dummyData;
			JsonParseMaster dummy(dummyData);
			JsonParseMaster* dummyPtr = dummy.Clone();
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster massah(data);
			massah.SetSharedData(data);
			FooJsonParseHelper helper;
			massah.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			massah.Parse(s);
			JsonParseMaster* newMaster = massah.Clone();
			string s1 = R"({"Foo" : {"Data":4,"OtherData":44}})";
			newMaster->Parse(s1);
			FooSharedData* f = (newMaster->GetSharedData())->As<FooSharedData>();
			Assert::IsTrue(&f->GetJsonParseMaster() == newMaster);
			Assert::AreEqual(Foo(1, 2), data.GetFoo());
			Assert::AreEqual(f->GetFoo(), Foo(4, 44));
			delete newMaster;
			delete dummyPtr;
		}

		TEST_METHOD(JsonFooParseDepthTest)
		{
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster master(data);
			FooJsonParseHelper helper;
			helper.Initialize();
			master.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			master.Parse(s);
			Assert::AreEqual(0U, data.Depth());
		}

		TEST_METHOD(JsonFooParseResetTest)
		{
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster master(data);
			FooJsonParseHelper helper;
			helper.Initialize();
			master.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			master.Parse(s);
			master.Reset();
		}

		TEST_METHOD(JsonNoHelperTest)
		{
			JsonParseMaster::SharedData data;
			JsonParseMaster master(data);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			master.Parse(s);
		}

		TEST_METHOD(JsonTableHelperTest)
		{
			Scope *topScope = new Scope();
			TableSharedData data(*topScope);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/scope.json";
			master.ParseFromFile(path);
			Assert::IsTrue("Bob" == (*topScope)["Name"].Get<string>());
			Assert::IsTrue(5 == (*topScope)["Healths"].Get<int>(1));
			glm::vec4 forward = glm::vec4(2.0f, 3.0f, 4.0f, 5.0f);
			glm::vec4 &other = (*topScope)["Forward"].Get<glm::vec4>();
			Assert::IsTrue(forward == other);
			glm::mat4 transform = (*topScope)["Transform"].Get<glm::mat4>();
			Assert::IsTrue(transform == glm::mat4(2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f));
			Datum &datum = (*topScope)["Turret"];
			Scope *turretScope = datum.Get<Scope*>();
			Assert::AreEqual((*turretScope)["Health"].Get<float>(), 1.0f);
			delete topScope;
		}

		TEST_METHOD(JsonTableCloneTest)
		{
			Scope *topScope = new Scope();
			TableSharedData data(*topScope);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			JsonParseMaster *secondMaster = master.Clone();
			string path = "Content/scope.json";
			secondMaster->ParseFromFile(path);
			delete topScope;
			topScope = secondMaster->GetSharedData()->As<TableSharedData>()->GetScope();
			Assert::IsTrue("Bob" == (*topScope)["Name"].Get<string>());
			Assert::IsTrue(5 == (*topScope)["Healths"].Get<int>(1));
			glm::vec4 forward = glm::vec4(2.0f, 3.0f, 4.0f, 5.0f);
			glm::vec4 &other = (*topScope)["Forward"].Get<glm::vec4>();
			Assert::IsTrue(forward == other);
			glm::mat4 transform = (*topScope)["Transform"].Get<glm::mat4>();
			Assert::IsTrue(transform == glm::mat4(2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f, 2.0f, 3.0f, 4.0f, 5.0f));
			Datum &datum = (*topScope)["Turret"];
			Scope *turretScope = datum.Get<Scope*>();
			Assert::AreEqual((*turretScope)["Health"].Get<float>(), 1.0f);
			delete secondMaster;
		}

		TEST_METHOD(JsonTableInitialize)
		{
			Scope *topScope = new Scope();
			TableSharedData data(*topScope);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string path = "Content/scope.json";
			master.ParseFromFile(path);
			master.Reset();
			JsonParseMaster other1 = std::move(master);
			delete topScope;
		}

		TEST_METHOD(JsonTableSharedDataTest)
		{
			Scope *topScope = new Scope();
			TableSharedData data(*topScope);
			TableSharedData otherData(std::move(data)), thirdData(*topScope);
			thirdData = std::move(otherData);
			delete topScope;
		}

		TEST_METHOD(JsonTableHelperFail)
		{
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster master(data);
			FooJsonParseHelper helper;
			JsonParseHelperTable tableHelper;
			helper.Initialize();
			master.AddHelper(tableHelper);
			master.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			master.Parse(s);
			Assert::AreEqual(Foo(1, 2), data.GetFoo());
		}

		TEST_METHOD(CloneAddHelpersAndFileErrors)
		{
			JsonParseMaster::SharedData dummyData;
			JsonParseMaster dummy(dummyData);
			Foo storageFoo;
			FooSharedData data(storageFoo);
			JsonParseMaster massah(data);
			massah.SetSharedData(data);
			FooJsonParseHelper helper;
			massah.AddHelper(helper);
			string s = R"({"Foo" : {"Data":1,"OtherData":2}})";
			massah.Parse(s);
			JsonParseMaster* newMaster = massah.Clone();
			string s1 = R"({"Foo" : {"Data":4,"OtherData":44}})";
			newMaster->Parse(s1);
			auto exprn = [&newMaster, &helper] {newMaster->AddHelper(helper); };
			Assert::ExpectException<exception>(exprn);
			auto exprn2 = [&newMaster, &helper] {newMaster->RemoveHelper(helper); };
			Assert::ExpectException<exception>(exprn2);
			auto exprn3 = [&newMaster] {newMaster->ParseFromFile("ffffffffffffffff1f"); };
			Assert::ExpectException<exception>(exprn3);
			delete newMaster;
		}

		TEST_METHOD(JsonBadParseClone)
		{

			Scope *topScope = new Scope();
			TableSharedData data(*topScope);
			JsonParseMaster master(data);
			JsonParseHelperTable helper;
			master.AddHelper(helper);
			string badJson =
				R"({
					"Healths":
					{
					}
				})";
			try
			{
				master.Parse(badJson);
			}
			catch (exception e)
			{

			}
			JsonParseMaster *master2 = master.Clone();
			master.Reset();
			delete master2;
			delete topScope;
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState JsonParseTests::sStartMemState;
}