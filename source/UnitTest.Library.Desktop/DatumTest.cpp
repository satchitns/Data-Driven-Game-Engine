#include "stdafx.h"
#include "CppUnitTest.h"
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default:4201)
#include "Datum.h"
#include "Foo.h"
#include "TestRTTI.h"
#include "Scope.h"

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
	TEST_CLASS(DatumTest)
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

		TEST_METHOD(Datum_Ctors_Dtors_Assigns)
		{

			Datum d, d1, d3;
			d = 2.2f;
			RTTI* x = new Foo(1, 1);
			Datum RTTIDatum, IntDatum, StringDatum, VecDatum, MatDatum;
			RTTIDatum = x;
			IntDatum = 4;
			StringDatum = "HELLO";
			StringDatum = "WORLD";
			VecDatum = glm::vec4();
			MatDatum = glm::mat4();
			auto exprn = [&d]() { d = 4; };
			Assert::ExpectException<exception>(exprn);
			int array[3] = { 1, 2, 3 };
			d1.SetStorage(array, 3);
			Assert::IsTrue(d1.IsExternal());
			d1.Set(4, 1);
			Assert::AreEqual(4, array[1]);
			d = d1;
			Assert::AreEqual(d1.Get<int>(2), 3);
			Assert::AreEqual(d.Get<int>(2), 3);
			d = d3;
			Datum newExtDatum(d1);
			Datum SecondStringDatum(StringDatum);
			Datum AnotherStringDatum;
			AnotherStringDatum = StringDatum;
			Datum SecondRTTIDatum(RTTIDatum);
			Assert::AreEqual(3U, newExtDatum.Size());
			Assert::AreEqual(newExtDatum.Get<int>(2), 3);
			auto exprn2 = [&newExtDatum]() {newExtDatum.Get<int>(3); };
			Assert::ExpectException<exception>(exprn2);
			delete x;
			Datum d4;
			auto exprn3 = [&d4, &array](){d4.SetStorage(array, 0); };
			Assert::ExpectException<exception>(exprn3);
			Datum d5;
			d5 = 4;
			auto exprn4 = [&d5, &array](){d5.SetStorage(array, 22); };
			Assert::ExpectException<exception>(exprn4);
			Assert::IsTrue(newExtDatum.Type() == d1.Type());
			Datum d1Killdatum;
			d1Killdatum = std::move(d1);
			Assert::AreEqual(3, d1Killdatum.Get<int32_t>(2));
			Assert::AreEqual(0U, d1.Size());
			Datum StringMoveDatum(std::move(StringDatum));
			Assert::IsTrue("WORLD" == StringMoveDatum.Get<string>());
			Assert::AreEqual(0U, StringDatum.Size());
			Datum StringMoveAssgnDatum;
			StringMoveAssgnDatum = std::move(StringMoveDatum);
			Assert::AreEqual(0U, StringMoveDatum.Size());
			Assert::IsTrue("WORLD" == StringMoveAssgnDatum.Get<string>());
		};

		TEST_METHOD(Datum_Set_Get)
		{
			Datum intDatum, floatDatum, stringDatum, vecDatum, matDatum, RTTIDatum;
			Foo f1(2, 2), f2(3, 3);
			RTTI *foo = &f1, *foo2 = &f2;
			RTTI *bar[2];
			bar[0] = foo;
			bar[1] = foo2;

			intDatum.SetType(Datum::DatumType::INTEGER);
			floatDatum.SetType(Datum::DatumType::FLOAT);
			stringDatum.SetType(Datum::DatumType::STRING);
			vecDatum.SetType(Datum::DatumType::VECTOR);
			matDatum.SetType(Datum::DatumType::MATRIX);
			RTTIDatum.SetType(Datum::DatumType::POINTER);

			intDatum.Resize(1);
			floatDatum.Resize(1);
			stringDatum.Resize(1);
			vecDatum.Resize(1);
			matDatum.Resize(1);
			RTTIDatum.Resize(1);

			auto exprn = [&intDatum] {intDatum.Set(5, 11); };
			Assert::ExpectException<exception>(exprn);
			intDatum.Set(5);
			float flval = 22.0f;
			floatDatum.Set(flval);
			stringDatum.Set("hey");
			vecDatum.Set(glm::vec4(1, 2, 3, 4));
			matDatum.Set(glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16));
			RTTIDatum.Set(foo);
			Assert::AreEqual(matDatum.Get<glm::mat4>(0)[0][0], 1.0f);
			Assert::AreEqual(floatDatum.Get<float>(0), flval);
			Assert::IsTrue(vecDatum.Get<glm::vec4>(0) == glm::vec4(1, 2, 3, 4));
			auto exprn2 = [&RTTIDatum, &bar] { RTTIDatum.SetStorage(bar, 3); };
			Assert::ExpectException<exception>(exprn2);
			Datum otherRTTIDatum;
			otherRTTIDatum.SetStorage(bar, 2);
			auto exprn3 = [&otherRTTIDatum] { otherRTTIDatum.Get<int>(0); };
			Assert::ExpectException<exception>(exprn3);
		}

		TEST_METHOD(Datum_Resize)
		{
			Datum intDatum, floatDatum, stringDatum, vecDatum, matDatum, RTTIDatum;
			Foo f2(3, 3);
			RTTI *foo2 = &f2;
			intDatum.SetType(Datum::DatumType::INTEGER);
			floatDatum.SetType(Datum::DatumType::FLOAT);
			stringDatum.SetType(Datum::DatumType::STRING);
			vecDatum.SetType(Datum::DatumType::VECTOR);
			matDatum.SetType(Datum::DatumType::MATRIX);
			RTTIDatum.SetType(Datum::DatumType::POINTER);
			intDatum.Resize(2);
			intDatum.Set(55, 1);
			RTTIDatum.Resize(2);
			RTTIDatum.Set(foo2, 1);
			Assert::IsTrue(foo2->Equals(RTTIDatum.Get<RTTI*>(1)));
			stringDatum.Resize(3);
			stringDatum.Set("bob", 2);
			Assert::IsTrue(!strcmp("bob", stringDatum.Get<string>(2).c_str()));
			floatDatum.Resize(2);
			Assert::AreEqual(2U, floatDatum.Size());
			floatDatum.Set(4.4f, 1);
			vecDatum.Resize(2);
			vecDatum.Set(glm::vec4(0), 1);
			matDatum.Resize(2);
			matDatum.Set(glm::mat4(0), 1);
			stringDatum.Resize(2);
			Assert::AreEqual(2U, stringDatum.Capacity());
			Datum nDatum;
		}
		TEST_METHOD(Datum_SetStorage)
		{
			Datum flDatum, stDatum, vDatum, mDatum;
			float f[] = { 1.2f, 2.2f };
			string s[] = { "hey", "hello" };
			glm::vec4 v[] = { glm::vec4(1), glm::vec4(2) };
			glm::mat4 m[] = { glm::mat4(1), glm::mat4(f[1]) };
			flDatum.SetStorage(f, 2);
			stDatum.SetStorage(s, 2);
			mDatum.SetStorage(m, 2);
			vDatum.SetStorage(v, 2);
			auto exprn = [&flDatum] {flDatum.PushBack(4.4f); };
			Assert::ExpectException<exception>(exprn);
			Datum nxDatum;
			int *a = nullptr;
			float *b = nullptr;
			string *c = nullptr;
			glm::vec4 *d = nullptr;
			glm::mat4 *e = nullptr;
			RTTI **f1 = nullptr;
			auto exprn1 = [&nxDatum, &a] { nxDatum.SetStorage(a, 2); };
			Assert::ExpectException<exception>(exprn1);
			auto exprn2 = [&nxDatum, &b] { nxDatum.SetStorage(b, 2); };
			Assert::ExpectException<exception>(exprn2);
			auto exprn3 = [&nxDatum, &c] { nxDatum.SetStorage(c, 2); };
			Assert::ExpectException<exception>(exprn3);
			auto exprn4 = [&nxDatum, &d] { nxDatum.SetStorage(d, 2); };
			Assert::ExpectException<exception>(exprn4);
			auto exprn5 = [&nxDatum, &e] { nxDatum.SetStorage(e, 2); };
			Assert::ExpectException<exception>(exprn5);
			auto exprn6 = [&nxDatum, &f1] { nxDatum.SetStorage(f1, 2); };
			Assert::ExpectException<exception>(exprn6);
		}

		TEST_METHOD(Datum_SetType)
		{
			Datum intDatum2;
			auto exprn6 = [&intDatum2] {intDatum2.SetType(Datum::DatumType::MAX); };
			Assert::ExpectException<exception>(exprn6);
			auto exprn7 = [&intDatum2] {intDatum2.SetType(Datum::DatumType::UNKNOWN); };
			Assert::ExpectException<exception>(exprn7);
			intDatum2.SetType(Datum::DatumType::INTEGER);
			auto exprn5 = [&intDatum2] {intDatum2.SetType(Datum::DatumType::FLOAT); };
			Assert::ExpectException<exception>(exprn5);
		}

		TEST_METHOD(Datum_Compares)
		{
			{
				Datum flDatum, stDatum, vDatum, mDatum, pDatum;
				float f[] = { 1.2f, 2.2f, 3.3f };
				string s[] = { "hey", "hello" };
				glm::vec4 v[] = { glm::vec4(1), glm::vec4(2) };
				glm::mat4 m[] = { glm::mat4(1), glm::mat4(f[1]) };
				Foo f1(2, 2), f2(3, 3);
				RTTI *foo1 = &f1, *foo2 = &f2;
				RTTI* p[] = { foo1, foo2 };

				flDatum.SetStorage(f, 3);
				Datum nDatum = flDatum;
				Assert::IsTrue(nDatum == flDatum);
				nDatum.Set(111.0f, 0);
				Assert::IsTrue(nDatum == flDatum);
				Datum floatDatum;
				floatDatum = 111.0f;
				floatDatum.PushBack(2.2f);
				floatDatum.PushBack(3.3f);
				Assert::IsTrue(floatDatum == flDatum);
				floatDatum.Set(22.0f, 0);
				Assert::IsTrue(floatDatum != flDatum);

				stDatum.SetStorage(s, 2);
				Datum st2;
				st2 = stDatum;
				Assert::IsFalse(st2 != stDatum);
				Datum st3;
				st3 = "hey";
				st3.PushBack("hello");
				Assert::IsTrue(st2 == st3);
				st2 = "hi";
				Assert::IsTrue(st2 != st3);

				vDatum.SetStorage(v, 2);
				Datum v2;
				v2 = glm::vec4(1);
				v2.PushBack(glm::vec4(2));
				Assert::IsTrue(v2 == vDatum);
				v2.Set(glm::vec4(), 1);
				Assert::IsTrue(v2 != vDatum);
				v2.PopBack();
				Assert::IsTrue(v2 != vDatum);

				pDatum.SetStorage(p, 2);
				Datum p2;
				p2 = foo1;
				p2.PushBack(foo2);
				Assert::IsTrue(p2 == pDatum);
				p2.PopBack();
				Assert::IsTrue(p2 != pDatum);
			}
			{
				//scalar comparisons
				Datum i, f, v, m, p, s;
				i = 5;
				Datum i2, f2, v2, m2, p2, s2;
				i2 = 5;
				Assert::IsFalse(i != i2);
				f = 22.2f;
				f2 = f;
				Assert::IsFalse(f != f2);
				v = glm::vec4(10);
				v2 = glm::vec4(22);
				Assert::IsFalse(v == v2);
				v2.Set(glm::vec4(10), 0);
				Assert::IsTrue(v2 == v);
				m = glm::mat4();
				m2 = m;
				Assert::IsFalse(m != m2);
				Foo a(2), b(2, 2);
				p = &a;
				p2 = &a;
				Assert::IsFalse(p != p2);
				p2 = &b;
				Assert::IsTrue(p != p2);
				s = "fff";
				s2 = "fff";
				Assert::IsFalse(s != s2);
				s2.Set("aaa", 0);
				Assert::IsTrue(s != s2);
			}
		}

		TEST_METHOD(Datum_Push_Pop)
		{
			Datum intDatum, floatDatum, stringDatum, vecDatum, matDatum, RTTIDatum;
			Foo f1(2, 2), f2(3, 3), f3(2, 2);
			RTTI *foo = &f1, *foo2 = &f2, *foo3 = &f3;
			RTTI *bar[2];
			bar[0] = foo;
			bar[1] = foo2;
			intDatum.PushBack(4);
			floatDatum.PushBack(99.99f);
			stringDatum.PushBack("hello");
			Assert::AreEqual(1U, stringDatum.Size());
			stringDatum.Set("world");
			stringDatum.PopBack();
			Assert::AreEqual(0U, stringDatum.Size());
			stringDatum.PushBack("world");
			vecDatum.PushBack(glm::vec4(44));
			matDatum.PushBack(glm::mat4(4));
			RTTIDatum.PushBack(foo);
			RTTIDatum.PushBack(foo2);
			Assert::AreEqual(2U, RTTIDatum.Size());
			Assert::IsTrue(RTTIDatum.Get<RTTI*>()->Equals(foo3));
			Assert::IsTrue(RTTIDatum.Get<RTTI*>(1)->Equals(foo2));
			RTTIDatum.PopBack();
			Assert::AreEqual(1U, RTTIDatum.Size());
			Assert::IsTrue(RTTIDatum.Get<RTTI*>()->Equals(foo));
			RTTIDatum.PopBack();
			Assert::AreEqual(0U, RTTIDatum.Size());
			RTTIDatum.PushBack(foo2);
			Assert::IsTrue(RTTIDatum.Get<RTTI*>()->Equals(foo2));
		}

		TEST_METHOD(Datum_StringConversion)
		{
			Datum intDatum, floatDatum, stringDatum, vecDatum, matDatum, RTTIDatum;
			Foo f1(2, 2), f2(3, 3);
			RTTI *foo = &f1, *foo2 = &f2;
			RTTI *bar[2];
			bar[0] = foo;
			bar[1] = foo2;
			{
				vecDatum = glm::vec4(1, 2, 3, 4);
				Datum copyDatum;
				copyDatum = glm::vec4();
				Assert::IsFalse(vecDatum.Get<glm::vec4>() == copyDatum.Get<glm::vec4>());
				copyDatum.SetFromString(vecDatum.ToString());
				Assert::IsTrue(vecDatum.Get<glm::vec4>() == copyDatum.Get<glm::vec4>());
			}
			{
				matDatum = glm::mat4(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
				Datum copyDatum;
				copyDatum = glm::mat4();
				Assert::IsTrue(matDatum.Get<glm::mat4>() != copyDatum.Get<glm::mat4>());
				copyDatum.SetFromString(matDatum.ToString(0));
				Assert::IsTrue(matDatum.Get<glm::mat4>() == copyDatum.Get<glm::mat4>());
			}
			{
				RTTIDatum = bar[0];
				auto exprn = [&RTTIDatum] { RTTIDatum.SetFromString("0, 0", 0); };
				Assert::ExpectException<exception>(exprn);
				string s = RTTIDatum.ToString();
				Assert::IsFalse(s.length() == 0);
			}
			{
				stringDatum = "hello";
				stringDatum.SetFromString("hi");
				Assert::IsTrue(!strcmp("hi", stringDatum.ToString().c_str()));
			}
			{
				intDatum.SetType(Datum::DatumType::INTEGER);
				intDatum.Resize(1);
				int x = 4;
				intDatum.SetFromString(to_string(x));
				Assert::IsTrue(intDatum.ToString() == to_string(x));
				Assert::AreEqual(x, intDatum.Get<int>());
			}
			{
				floatDatum.SetType(Datum::DatumType::FLOAT);
				floatDatum.Resize(1);
				float x = 4.4f;
				floatDatum.SetFromString(to_string(x));
				Assert::IsTrue(floatDatum.ToString() == to_string(x));
				Assert::AreEqual(x, floatDatum.Get<float>());
			}
		}

		TEST_METHOD(Foo_RTTI_Test)
		{
			Foo f1(2, 2), f2(3, 3), f3(2, 2);
			RTTI *foo = &f1, *foo2 = &f2;
			RTTI *bar[3];
			bar[0] = foo;
			bar[1] = foo2;
			TestRTTI x;
			bar[2] = &x;
			Assert::IsFalse(bar[0]->Equals(bar[1]));
			Assert::IsTrue(bar[0]->Equals(bar[0]));
			bar[1] = &f3;
			Assert::IsTrue(bar[0]->Equals(bar[1]));
			Assert::IsTrue(bar[0]->Is("Foo"));
			Assert::IsTrue(bar[0]->Is(Foo::TypeIdClass()));
			Assert::IsFalse(bar[2]->Is(Foo::TypeName()));
			Assert::AreEqual(f1.Data(), bar[0]->As<Foo>()->Data());
			Assert::IsTrue(bar[0]->As<TestRTTI>() == nullptr);
			Assert::IsTrue(bar[0]->QueryInterface(TestRTTI::TypeIdClass()) == nullptr);
			Assert::IsTrue(bar[0]->QueryInterface(Foo::TypeIdClass()) != nullptr);
			string s = bar[0]->ToString();
			Assert::IsTrue(s.length() > 0);
		}

		TEST_METHOD(RTTI_Test)
		{
			TestRTTI test;
			RTTI* rtti = &test, *other = nullptr;
			Assert::IsFalse(rtti->Is("Foo"));
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->QueryInterface(Foo::TypeIdClass()) == nullptr);
			rtti->ToString();
			rtti->Equals(other);
		}

		TEST_METHOD(Datum_ScalarConstructors)
		{
			Foo a(3, 3);
			Datum intDatum = 5, floatDatum = 4.4f, stringDatum = "hi", ptrDatum = &a,
				vecDatum = glm::vec4(2), matDatum = glm::mat4(33);
			Assert::AreEqual(1U, intDatum.Size());
			Assert::AreEqual(1U, floatDatum.Size());
			Assert::AreEqual(1U, stringDatum.Size());
			Assert::AreEqual(1U, ptrDatum.Size());
			Assert::AreEqual(1U, vecDatum.Size());
			Assert::AreEqual(1U, matDatum.Size());
			Assert::AreEqual(5, intDatum.Get<int>());
			Assert::AreEqual(4.4f, floatDatum.Get<float>());
			Assert::IsTrue(string("hi") == stringDatum.Get<string>());
			Assert::IsTrue(&a == ptrDatum.Get<RTTI*>());
			Assert::IsTrue(glm::vec4(2) == vecDatum.Get<glm::vec4>());
			Assert::IsTrue(glm::mat4(33) == matDatum.Get<glm::mat4>());
		}

		TEST_METHOD(Datum_Removes)
		{
			Foo a(3, 3);
			Scope x;
			Datum intDatum = 5, floatDatum = 4.4f, stringDatum = "hi", ptrDatum = &a,
				vecDatum = glm::vec4(2), matDatum = glm::mat4(33), scopeDatum = &x;
			Assert::IsTrue(intDatum.Remove(5));
			Assert::IsTrue(floatDatum.Remove(4.4f));
			Assert::IsTrue(stringDatum.Remove("hi"));
			Assert::IsTrue(ptrDatum.Remove(&a));
			Assert::IsTrue(vecDatum.Remove(glm::vec4(2)));
			Assert::IsTrue(matDatum.Remove(glm::mat4(33)));
			Assert::IsTrue(scopeDatum.Remove(&x));
			Assert::IsFalse(intDatum.Remove(5));
			Assert::IsFalse(floatDatum.Remove(4.4f));
			Assert::IsFalse(stringDatum.Remove("hi"));
			Assert::IsFalse(ptrDatum.Remove(&a));
			Assert::IsFalse(vecDatum.Remove(glm::vec4(2)));
			Assert::IsFalse(matDatum.Remove(glm::mat4(33)));
			Assert::IsFalse(scopeDatum.Remove(&x));
			Assert::IsFalse(intDatum.Remove(22));
		}

		TEST_METHOD(Datum_Finds)
		{
			Foo a(3, 3);
			Scope x;
			Datum intDatum = 5, floatDatum = 4.4f, stringDatum = "hi", ptrDatum = &a,
				vecDatum = glm::vec4(2), matDatum = glm::mat4(33), scopeDatum = &x;
			Assert::IsTrue(intDatum.Find(5));
			Assert::IsTrue(floatDatum.Find(4.4f));
			Assert::IsTrue(stringDatum.Find("hi"));
			Assert::IsTrue(ptrDatum.Find(&a));
			Assert::IsTrue(vecDatum.Find(glm::vec4(2)));
			Assert::IsTrue(matDatum.Find(glm::mat4(33)));
			Assert::IsTrue(scopeDatum.Find(&x));

			Assert::IsTrue(intDatum.Remove(5));
			Assert::IsTrue(floatDatum.Remove(4.4f));
			Assert::IsTrue(stringDatum.Remove("hi"));
			Assert::IsTrue(ptrDatum.Remove(&a));
			Assert::IsTrue(vecDatum.Remove(glm::vec4(2)));
			Assert::IsTrue(matDatum.Remove(glm::mat4(33)));
			Assert::IsTrue(scopeDatum.Remove(&x));

			Assert::IsFalse(intDatum.Find(5));
			Assert::IsFalse(floatDatum.Find(4.4f));
			Assert::IsFalse(stringDatum.Find("hi"));
			Assert::IsFalse(ptrDatum.Find(&a));
			Assert::IsFalse(vecDatum.Find(glm::vec4(2)));
			Assert::IsFalse(matDatum.Find(glm::mat4(33)));
			Assert::IsFalse(scopeDatum.Find(&x));
			Assert::IsFalse(intDatum.Find(22));
		}

		TEST_METHOD(Datum_ScopeMethods)
		{
			Datum scopeDatum;
			Scope *x = nullptr, y;
			scopeDatum = x;
			auto exprn0 = [&scopeDatum] { scopeDatum[0]; };
			Assert::ExpectException<exception>(exprn0);
			auto exprn = [&scopeDatum] {scopeDatum.ToString(); };
			Assert::ExpectException<exception>(exprn);
			scopeDatum.Set(&y);
			scopeDatum[0].Append("Legs") = 4;
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState DatumTest::sStartMemState;
}