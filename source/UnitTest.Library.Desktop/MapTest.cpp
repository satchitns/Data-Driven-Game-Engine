#include "stdafx.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include<utility>
#include<sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace FieaGameEngine
{
	template<>
	class DefaultHash<UnitTestLibraryDesktop::Foo>
	{
	public:
		uint32_t operator()(const UnitTestLibraryDesktop::Foo& key) const
		{
			return ((key.Data() + *key.OtherData()) * 33);
		}
	};
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(MapTest)
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

		TEST_METHOD(Hash_Test)
		{
			{
				int x = -44, y = 2;
				DefaultHash<int> hasher;
				Assert::AreEqual(hasher(-44), hasher(x));
				Assert::AreEqual(hasher(2), hasher(y));
				Assert::AreNotEqual(hasher(x), hasher(y));
			}
			{
				char* a = "Hello";
				char b[80];
				strcpy_s(b, a);
				char* c = "BobDylan";
				DefaultHash<char*> hasher;
				Assert::AreEqual(hasher(a), hasher(b));
				Assert::AreNotEqual(hasher(a), hasher(c));
				b[3] = '2';
				Assert::AreNotEqual(hasher(a), hasher(b));
			}
			{
				std::string a("Hello"), b("Hello"), c("He11o");
				DefaultHash<std::string> hasher;
				Assert::AreEqual(hasher(a), hasher(b));
				Assert::AreNotEqual(hasher(a), hasher(c));
			}
			{
				Foo a(44, 22), b(44, 22), c(44, 2);
				DefaultHash<Foo> hasher;
				Assert::AreEqual(hasher(a), hasher(b));
				Assert::AreNotEqual(hasher(a), hasher(c));
			}
			auto exprn = [] {BernsteinHash(nullptr, 4); };
			Assert::ExpectException<std::exception>(exprn);
		}

		TEST_METHOD(Map_Ctors_Dtor)
		{
			Foo x(1, 1), y(2, 2);
			int z = 1, w = 1;
			auto exprn = [] {HashMap<Foo, Foo> a(0); };
			Assert::ExpectException<std::exception>(exprn);
			HashMap<Foo, int> a(10);
			a.Insert(std::make_pair(x, z));
			a.Insert(std::make_pair(y, w));
			HashMap<Foo, int> b = a;
			Assert::AreEqual(a.Size(), b.Size());
			Assert::IsTrue(b.ContainsKey(x));
			HashMap<Foo, int> n;
			n = std::move(b);
			Assert::AreEqual(0U, b.Size());
			Assert::IsTrue(n.ContainsKey(x));
			HashMap<Foo, int> m(std::move(n));
			Assert::AreEqual(0U, n.Size());
			Assert::IsTrue(m.ContainsKey(x));
		}

		TEST_METHOD(Map_Insert)
		{
			{
				Foo x(1, 1), y(2, 2);
				int a = 1, b = 2;
				HashMap<Foo, int> map(10);
				auto it = map.Insert(std::make_pair(x, a));
				auto it2 = map.Insert(std::make_pair(x, b));
				Assert::IsTrue(it == it2);
				Assert::AreEqual(1U, map.Size());
				it2 = map.Insert(std::make_pair(y, b));
				Assert::IsTrue(it != it2);
				Assert::AreEqual(2U, map.Size());
			}
			{
				int x = 1, y = 2;
				int a = 1, b = 2;
				HashMap<int, int> map(10);
				auto it = map.Insert(std::make_pair(x, a));
				auto it2 = map.Insert(std::make_pair(x, b));
				Assert::IsTrue(it == it2);
				Assert::AreEqual(1U, map.Size());
				it2 = map.Insert(std::make_pair(y, b));
				Assert::IsTrue(it != it2);
				Assert::AreEqual(2U, map.Size());
			}
			{
				std::string x = "faa", y = "buur";
				int a = 1, b = 2;
				HashMap<std::string, int> map(10);
				auto it = map.Insert(std::make_pair(x, a));
				auto it2 = map.Insert(std::make_pair(x, b));
				Assert::IsTrue(it == it2);
				Assert::AreEqual(1U, map.Size());
				it2 = map.Insert(std::make_pair(y, b));
				Assert::IsTrue(it != it2);
				Assert::AreEqual(2U, map.Size());
			}
		}

		TEST_METHOD(Map_Find)
		{
			{
				Foo x(1, 1), y(2, 2);
				int a = 1, b = 2;
				HashMap<Foo, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(x, b));
				map.Insert(std::make_pair(y, b));
				auto it = map.Find(x);
				Assert::IsTrue(*it == std::make_pair(x, a));
				it = map.Find(Foo(22, 22));
				Assert::IsTrue(it == map.end());
			}
			{
				char* a = "hello";
				char b[40];
				strcpy_s(b, a);
				HashMap<char*, int> map;
				map.Insert(std::make_pair(a, 1));
				Assert::AreEqual(1, map.Find(b)->second);
			}
			{
				int a = 1, b = 2, c;
				int *x = &a, *y = &b, *z = &c;
				HashMap<int*, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(x, b));
				map.Insert(std::make_pair(y, b));
				auto it = map.Find(x);
				Assert::IsTrue(*it == std::make_pair(x, a));
				it = map.Find(z);
				Assert::IsTrue(it == map.end());
			}
		}

		TEST_METHOD(Map_Index_At)
		{
			{
				Foo x(1, 1), y(2, 2);
				int a = 1, b = 2;
				HashMap<Foo, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(x, b));
				Assert::AreEqual(map[x], a);
				map[x] = b;
				Assert::AreEqual(map[x], b);
				Assert::AreEqual(1U, map.Size());
				map[y] = b;
				Assert::AreEqual(2U, map.Size());
				HashMap<Foo, int> map2(10);
				map2.Insert(std::make_pair(x, a));
				map2.Insert(std::make_pair(x, b));
				map2.At(x) = b;
				Assert::AreEqual(map2[x], b);
				auto exprn = [&map2, &y] {map2.At(y) = 4; };
				Assert::ExpectException<std::exception>(exprn);
			}
			{
				int x = 55, y = 44;
				int a = 1, b = 2;
				HashMap<int, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(x, b));
				Assert::AreEqual(map[x], a);
				map[x] = b;
				Assert::AreEqual(map[x], b);
				Assert::AreEqual(1U, map.Size());
				map[y] = b;
				Assert::AreEqual(2U, map.Size());
				HashMap<Foo, int> map2(10);
				map2.Insert(std::make_pair(x, a));
				map2.Insert(std::make_pair(x, b));
				map2.At(x) = b;
				Assert::AreEqual(map2[x], b);
				auto exprn = [&map2, &y] {map2.At(y) = 4; };
				Assert::ExpectException<std::exception>(exprn);
			}
		}

		TEST_METHOD(Map_Iterators)
		{
			std::string a = "One", b = "Two", c = "Three";
			int x = 1, y = 2, z = 3;
			HashMap<std::string, int> map(22), map2;
			map.Insert(std::make_pair(a, x));
			map.Insert(std::make_pair(b, y));
			map.Insert(std::make_pair(c, z));
			map2.Insert(std::make_pair(a, x));
			int sum = 0;
			for (const auto &value : map)
			{
				sum += value.second;
			}
			Assert::AreEqual(sum, x + y + z);
			HashMap<std::string, int>::Iterator it, it2;
			auto exprn = [&it] { ++it; };
			Assert::ExpectException<std::exception>(exprn);
			auto exprn2 = [&it] {*it; };
			Assert::ExpectException<std::exception>(exprn2);
			auto exprn3 = [&it] {it->first; };
			Assert::ExpectException<std::exception>(exprn3);
			Assert::IsTrue(it == it2);
			it = map.end();
			Assert::IsTrue(it != it2);
			it = map.begin();
			it2 = map2.begin();
			Assert::IsTrue(it != it2);
			it++;
		}

		TEST_METHOD(Map_Remove_Clear)
		{
			{
				Foo x(1, 1), y(2, 2);
				int a = 1, b = 2;
				HashMap<Foo, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(y, b));
				bool killed = map.Remove((4, 4));
				Assert::IsFalse(killed);
				killed = map.Remove(x);
				Assert::IsTrue(killed);
				auto it = map.Find(x);
				Assert::IsTrue(it == map.end());
				map.Insert(std::make_pair(y, b));
				map.Clear();
				Assert::AreEqual(0U, map.Size());
				map.Insert(std::make_pair(y, b));
				map.Insert(std::make_pair(x, a));
				Assert::AreEqual(2U, map.Size());
			}
			{
				std::string x = "Hell", y = "o";
				int a = 1, b = 2;
				HashMap<std::string, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(y, b));
				bool killed = map.Remove("o0");
				Assert::IsFalse(killed);
				killed = map.Remove("o");
				Assert::IsTrue(killed);
				auto it = map.Find(y);
				Assert::IsTrue(it == map.end());
				map.Insert(std::make_pair(y, b));
				map.Clear();
				Assert::AreEqual(0U, map.Size());
			}
			{
				char* x = "Hell", *y = "o";
				char c[90];
				strcpy_s(c, y);
				int a = 1, b = 2;
				HashMap<char*, int> map(10);
				map.Insert(std::make_pair(x, a));
				map.Insert(std::make_pair(y, b));
				bool killed = map.Remove("o0");
				Assert::IsFalse(killed);
				killed = map.Remove("o");
				Assert::IsTrue(killed);
				auto it = map.Find(c);
				Assert::IsTrue(it == map.end());
				map.Insert(std::make_pair(y, b));
				map.Clear();
				Assert::AreEqual(0U, map.Size());
			}
		}

		TEST_METHOD(Map_ContainsKey)
		{

		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState MapTest::sStartMemState;
}