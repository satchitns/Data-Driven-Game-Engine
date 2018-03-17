#include "stdafx.h"
#include "CppUnitTest.h"
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default:4201)
#include "Datum.h"
#include "Foo.h"
#include "Scope.h"
#include "TestRTTI.h"
#include "Attributed.h"

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
	TEST_CLASS(ScopeTest)
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

		TEST_METHOD(Scope_Ctor_Dtor)
		{
			{
				Scope S1;
				S1.Append("A") = 100;
				S1.AppendScope("Scopes");
				Scope *S2 = new Scope();
				S2->Append("B") = glm::vec4(3);
				Scope S3(S1);
				Assert::IsTrue(S3 == S1);
				*S2 = S1;
				Assert::IsTrue(S3 == *S2);
				S2->AppendScope("ChildScope");
				Scope* killScope = &(S2->AppendScope("ChildScope"));
				delete killScope;
				delete S2;
			}
			{
				//contrived copy
				Scope S1;
				S1.AppendScope("Scopes");
				Scope* x = nullptr;
				S1["Scopes"].PushBack(x);
				Scope S2(S1);
				Assert::IsTrue(S2 == S1);
			}
			{
				//move semantics
				Scope *S1 = new Scope();
				S1->AppendScope("Scope");
				Scope S3(std::move(*S1));
				Assert::IsTrue(S3.Find("Scope") != nullptr);
				Scope S4;
				S4 = std::move(S3);
				Scope& explodingScope = S4.AppendScope("TestExplosionScope");
				Scope *S5 = new Scope(move(explodingScope));
				S5;
				delete &explodingScope;
				delete S1;
			}
		}

		TEST_METHOD(Scope_Append)
		{
			Scope S1;
			Foo a(2, 2), b(1, 1);
			S1.Append("A") = 100;
			auto exprn = [&S1] {S1.Append("A") = 2.2f; };
			Assert::ExpectException<exception>(exprn);
			S1.Append("Name") = "TestScope";
			S1.Append("Foos") = &a;
			S1.Find("Foos")->PushBack(&b);
			auto exprn2 = [&S1] {S1.Append(""); };
			Assert::ExpectException<exception>(exprn2);
		}

		TEST_METHOD(Scope_AppendScope)
		{
			Scope S1;
			S1.Append("Count") = 19;
			auto exprn = [&S1] {S1.AppendScope("Count"); };
			Assert::ExpectException<exception>(exprn);
			S1.Append("Scopes");
			S1.AppendScope("Scopes");
			S1.AppendScope("Scopes!");
			auto exprn2 = [&S1] {S1.AppendScope(""); };
			Assert::ExpectException<exception>(exprn2);
		}

		TEST_METHOD(Scope_Find)
		{
			Scope S1;
			Foo a(2, 2), b(1, 1);
			S1.Append("Name") = "TestScope";
			S1.Append("Foos") = &a;
			S1.Find("Foos")->PushBack(&b);
			Assert::IsTrue(S1.Find("NotFoo") == nullptr);
		}

		TEST_METHOD(Scope_FindName)
		{
			{
				Scope *S1 = new Scope(), S2;
				Foo a(2, 2), b(1, 1);
				(*S1).Append("Name");
				(*S1).Append("Foos");
				(*S1).Find("Foos")->PushBack(&b);
				S2.Adopt(S1, "Scope");
				Assert::IsTrue("Scope" == S2.FindName(S1));
				Assert::IsTrue("" == S1->FindName(S1));
			}
			{
				//contrived Find
				Scope S1, S2;
				S1.Append("A") = 5;
				S2.Append("C") = 5.5f;
				S1.AppendScope("B");
				Assert::AreEqual(string(), S1.FindName(&S2));
			}
		}

		TEST_METHOD(Scope_Adopt_Relinquish)
		{
			Scope *S1 = new Scope(), S2, S3, S4;
			Foo a(2, 2), b(1, 1);
			(*S1).Append("Name");
			(*S1).Append("Foos");
			(*S1).Find("Foos")->PushBack(&b);
			S2.Adopt(S1, "Scope");
			Assert::IsTrue(S1->GetParent() == &S2);
			S3.AppendScope("Scopes!");
			S3.Adopt(S1, "Scopes!");
			Assert::IsTrue(S1->GetParent() == &S3);
			S4.Append("Health") = 100.0f;
			auto exprn = [&S1, &S4] {S4.Adopt(S1, "Health"); };
			Assert::ExpectException<exception>(exprn);
			auto exprn1 = [&S4] {S4.Adopt(nullptr, "Scopes"); };
			Assert::ExpectException<exception>(exprn1);
			auto exprn2 = [&S4] {S4.Adopt(&S4, "Scopes"); };
			Assert::ExpectException<exception>(exprn2);
			auto exprn3 = [&S4] {S4.Relinquish(nullptr); };
			Assert::ExpectException<exception>(exprn3);
			auto exprn4 = [&S4] {S4.Relinquish(&S4); };
			Assert::ExpectException<exception>(exprn4);
			delete S1;
		}

		TEST_METHOD(Scope_Compares)
		{
			Scope S1;
			Scope *S2 = new Scope();
			Scope S3;
			Assert::IsFalse(S1 != *S2);
			S3.Adopt(S2, "scope");
			Assert::IsFalse(S1 != *S2);
			Assert::IsTrue(S3 != S1);
			S1["Health"] = 22;
			uint32_t x = 0;
			S1[x] = 4;
			S2->Append("Health") = 3;
			Assert::IsTrue(S1 != *S2);
			S2->Find("Health")->Get<int>() = 4;
			Assert::IsFalse(S1 != *S2);
			Assert::IsTrue(S1.Equals(S2));
			Assert::IsTrue(S1.Equals(&S1));
			Foo foo(2, 2);
			RTTI *fPtr = &foo;
			Assert::IsFalse(S1.Equals(fPtr));
		}

		TEST_METHOD(Scope_Searches)
		{
			Scope *S1 = new Scope(), S2;
			S2.Append("Potato") = "big potato";
			Foo a(2, 2), b(1, 1);
			(*S1).Append("Name");
			(*S1).Append("Foos");
			(*S1).Find("Foos")->PushBack(&b);
			S2.Adopt(S1, "Scope");
			Scope *whichScope;
			S1->Search("Potato", whichScope)->PushBack("small potato");
			Assert::IsTrue(whichScope == &S2);
			Assert::IsTrue("small potato" == S2.Find("Potato")->Get<string>(1));
			Assert::IsTrue(S1->Search("Tomato") == nullptr);
		}

		TEST_METHOD(Scope_ToStrings)
		{
			Scope *S1 = new Scope(), S2;
			S2.Append("Potato") = "big potato";
			Foo a(2, 2), b(1, 1);
			(*S1).Append("Name");
			(*S1).Append("Foos");
			(*S1).Find("Foos")->PushBack(&b);
			S2.Adopt(S1, "Scope");
			Scope *whichScope;
			S1->Search("Potato", whichScope)->PushBack("small potato");
			Assert::IsTrue(whichScope == &S2);
			Assert::IsTrue("small potato" == S2.Find("Potato")->Get<string>(1));
			Assert::IsTrue(S1->Search("Tomato") == nullptr);
			string name = "Potato, Scope";
			Assert::AreEqual(name, S2.ToString());
			name = "Name, Foos";
			Assert::AreEqual(name, S1->ToString());
			Scope s3;
			name = "";
			Assert::AreEqual(name, s3.ToString());
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState ScopeTest::sStartMemState;
}