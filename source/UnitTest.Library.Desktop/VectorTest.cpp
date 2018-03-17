#include "stdafx.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Vector.h"
#include "AssertHelp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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

		TEST_METHOD(Vector_Ctors_Dtor_Assignment)
		{
			{
				//ints
				int x = 5, y = 6;
				Vector<int> vector, vector2(0), vector3(4);
				Vector<int> vector4 = vector;
				Vector<int> vector5, vector6(2);
				vector5 = vector2; //copying from smaller to larger capacity
				vector3.PushBack(x);
				vector3.PushBack(y);
				vector6 = vector3; //copying from larger to smaller capacity
				vector3.Front() = 22;
				Assert::AreNotEqual(vector3.Front(), vector6.Front());
				vector5 = vector5; //copying self
			}
			{
				//Foos
				Foo x(5, 5), y(6, 6), z(22, 22);
				Vector<Foo> vector, vector2(0), vector3(4);
				Vector<Foo> vector4 = vector;
				Vector<Foo> vector5, vector6(2);
				vector5 = vector2; //copying from smaller to larger capacity
				vector3.PushBack(x);
				vector3.PushBack(y);
				vector6 = vector3; //copying from larger to smaller capacity
				vector3.Front() = z;
				Assert::AreNotEqual(vector3.Front(), vector6.Front());
				vector5 = vector5; //copying self
				vector5 = std::move(vector3);
				Assert::AreEqual(z, vector5.Front());
				Assert::AreEqual(0U, vector3.Size());
				Vector<Foo> vector7(std::move(vector5));
				Assert::AreEqual(z, vector7.Front());
				Assert::AreEqual(0U, vector5.Size());
			}
			{
				//int ptrs
				int x = 5, y = 6, z = 7;
				Vector<int*> vector, vector2(0), vector3(4);
				Vector<int*> vector4 = vector;
				Vector<int*> vector5, vector6(2);
				vector5 = vector2; //copying from smaller to larger capacity
				vector3.PushBack(&x);
				vector3.PushBack(&y);
				vector6 = vector3; //copying from larger to smaller capacity
				vector3.Front() = &z;
				Assert::AreNotEqual(vector3.Front(), vector6.Front());
				vector5 = vector5; //copying self
			}
		}

		TEST_METHOD(Vector_Clear_Shrink)
		{
			{
				//ints
				Vector<int> vector(3);
				vector.PushBack(1);
				vector.PushBack(2);
				vector.PushBack(3);
				Assert::AreEqual(3U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Clear();
				Assert::AreEqual(0U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Shrink();
				Assert::AreEqual(0U, vector.Capacity());
				vector.PushBack(1);
				vector.PushBack(2);
				vector.Shrink();
				Assert::AreEqual(2U, vector.Capacity());
			}
			{
				//int ptrs
				int x = 1, y = 2, z = 3;
				Vector<int*> vector(3);
				vector.PushBack(&x);
				vector.PushBack(&y);
				vector.PushBack(&z);
				Assert::AreEqual(3U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Clear();
				Assert::AreEqual(0U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Shrink();
				Assert::AreEqual(0U, vector.Capacity());
				vector.PushBack(&x);
				vector.PushBack(&y);
				vector.Shrink();
				Assert::AreEqual(2U, vector.Capacity());
			}
			{
				//Foos
				Vector<Foo> vector(3);
				Foo a(1, 1), b(2, 2), c(3, 3);
				vector.PushBack(a);
				vector.PushBack(b);
				vector.PushBack(c);
				Assert::AreEqual(3U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Clear();
				Assert::AreEqual(0U, vector.Size());
				Assert::AreEqual(3U, vector.Capacity());
				vector.Shrink();
				Assert::AreEqual(0U, vector.Capacity());
				vector.PushBack(a);
				vector.PushBack(b);
				vector.Shrink();
				Assert::AreEqual(2U, vector.Capacity());
			}
		}

		TEST_METHOD(Vector_Index_At)
		{
			int x = 1, y = 2, z = 3, w = 44;
			Vector<int> vector(3);
			vector.PushBack(x);
			vector.PushBack(y);
			vector.PushBack(z);
			Assert::AreEqual(vector[0], x);
			Assert::AreEqual(vector[2], z);
			auto exception = [&vector] { vector[vector.Size()]; };
			Assert::ExpectException<std::out_of_range>(exception);
			vector[1] = w;
			Assert::AreEqual(vector.At(1), w);
			auto exception2 = [&vector] { vector[vector.Size() + 1]; };
			Assert::ExpectException<std::out_of_range>(exception2);
			vector[1] = 44;
			const Vector<int> vector2 = vector;
			Assert::AreEqual(vector2[0], x);
			Assert::AreEqual(vector2[2], z);
			auto exception3 = [&vector2] { vector2[vector2.Size()]; };
			Assert::ExpectException<std::out_of_range>(exception3);
			Assert::AreEqual(vector2.At(0), x);
		}

		TEST_METHOD(Vector_Find)
		{
			int x = 1, y = 2, z = 3, w = 44;
			Vector<int> vector(3);
			vector.PushBack(x);
			vector.PushBack(y);
			vector.PushBack(z);
			auto it = vector.Find(w);
			Assert::IsTrue(it == vector.end());
			it = vector.Find(x);
			Assert::AreEqual(*it, x);
		}

		TEST_METHOD(Vector_PushBack)
		{
			{
				//ints
				int x = 1, y = 2, z = 3, w = 44;
				Vector<int> vector(3);
				vector.PushBack(x);
				Assert::AreEqual(vector.Back(), x);
				Assert::AreEqual(1U, vector.Size());
				vector.PushBack(y);
				Assert::AreEqual(vector.Back(), y);
				Assert::AreEqual(2U, vector.Size());
				vector.PushBack(z);
				Assert::AreEqual(vector.Back(), z);
				Assert::AreEqual(3U, vector.Size());
				vector.PushBack(w);
				Assert::AreEqual(4U, vector.Size());
				Assert::IsTrue(vector.Capacity() > 3U);
			}
			{
				//int ptrs
				int x = 1, y = 2, z = 3, w = 44;
				Vector<int*> vector(3);
				vector.PushBack(&x);
				Assert::AreEqual(vector.Back(), &x);
				Assert::AreEqual(1U, vector.Size());
				vector.PushBack(&y);
				Assert::AreEqual(vector.Back(), &y);
				Assert::AreEqual(2U, vector.Size());
				vector.PushBack(&z);
				Assert::AreEqual(vector.Back(), &z);
				Assert::AreEqual(3U, vector.Size());
				vector.PushBack(&w);
				Assert::AreEqual(4U, vector.Size());
				Assert::IsTrue(vector.Capacity() > 3U);
			}
			{
				//Foos
				Foo x(1, 1), y = (2, 2), z = 3, w = (44, -22);
				Vector<Foo> vector(3);
				vector.PushBack(x);
				Assert::AreEqual(vector.Back(), x);
				Assert::AreEqual(1U, vector.Size());
				vector.PushBack(y);
				Assert::AreEqual(vector.Back(), y);
				Assert::AreEqual(2U, vector.Size());
				vector.PushBack(z);
				Assert::AreEqual(vector.Back(), z);
				Assert::AreEqual(3U, vector.Size());
				vector.PushBack(w);
				Assert::AreEqual(4U, vector.Size());
				Assert::IsTrue(vector.Capacity() > 3U);
			}
		}

		TEST_METHOD(Vector_PopBack)
		{
			{
				//ints
				int x = 1, y = 2, z = 3;
				Vector<int> vector(3);
				vector.PushBack(x);
				vector.PushBack(y);
				vector.PushBack(z);
				Assert::AreEqual(3U, vector.Size());
				vector.PopBack();
				Assert::AreEqual(2U, vector.Size());
				Assert::AreEqual(vector.Back(), y);
				vector.PopBack();
				Assert::AreEqual(1U, vector.Size());
				Assert::AreEqual(vector.Back(), x);
				vector.PopBack();
				Assert::AreEqual(0U, vector.Size());
				auto exception = [&vector] {vector.PopBack(); };
				Assert::ExpectException<std::exception>(exception);
			}
			{
				//int ptrs
				int x = 1, y = 2, z = 3;
				Vector<int*> vector(3);
				vector.PushBack(&x);
				vector.PushBack(&y);
				vector.PushBack(&z);
				Assert::AreEqual(3U, vector.Size());
				vector.PopBack();
				Assert::AreEqual(2U, vector.Size());
				Assert::AreEqual(vector.Back(), &y);
				vector.PopBack();
				Assert::AreEqual(1U, vector.Size());
				Assert::AreEqual(vector.Back(), &x);
				vector.PopBack();
				Assert::AreEqual(0U, vector.Size());
				auto exception = [&vector] {vector.PopBack(); };
				Assert::ExpectException<std::exception>(exception);
			}
			{
				//Foos
				Foo x = (1, 1), y = 2, z = 3;
				Vector<Foo> vector(3);
				vector.PushBack(x);
				vector.PushBack(y);
				vector.PushBack(z);
				Assert::AreEqual(3U, vector.Size());
				vector.PopBack();
				Assert::AreEqual(2U, vector.Size());
				Assert::AreEqual(vector.Back(), y);
				vector.PopBack();
				Assert::AreEqual(1U, vector.Size());
				Assert::AreEqual(vector.Back(), x);
				vector.PopBack();
				Assert::AreEqual(0U, vector.Size());
				auto exception = [&vector] {vector.PopBack(); };
				Assert::ExpectException<std::exception>(exception);
			}
		}

		TEST_METHOD(Vector_Reserve)
		{
			{
				//ints
				int x = 44;
				Vector<int> vector(0);
				Assert::AreEqual(0U, vector.Capacity());
				vector.Reserve(500);
				vector.PushBack(x);
				Assert::AreEqual(500U, vector.Capacity());
				vector.Reserve(1);
				Assert::AreEqual(500U, vector.Capacity());
				Assert::AreEqual(x, vector.Back());
			}
			{
				//ints
				int x = 44;
				Vector<int> vector(0);
				Assert::AreEqual(0U, vector.Capacity());
				vector.Reserve(500);
				vector.PushBack(x);
				Assert::AreEqual(500U, vector.Capacity());
				vector.Reserve(1);
				Assert::AreEqual(500U, vector.Capacity());
				Assert::AreEqual(x, vector.Back());
			}
			{
				//int ptrs
				int x = 44;
				Vector<int*> vector(0);
				Assert::AreEqual(0U, vector.Capacity());
				vector.Reserve(500);
				vector.PushBack(&x);
				Assert::AreEqual(500U, vector.Capacity());
				vector.Reserve(1);
				Assert::AreEqual(500U, vector.Capacity());
				Assert::AreEqual(&x, vector.Back());
			}
			{
				//Foos
				Foo x = (0, 44);
				Vector<Foo> vector(0);
				Assert::AreEqual(0U, vector.Capacity());
				vector.Reserve(500);
				vector.PushBack(x);
				Assert::AreEqual(500U, vector.Capacity());
				vector.Reserve(1);
				Assert::AreEqual(500U, vector.Capacity());
				Assert::AreEqual(x, vector.Back());
			}
		}

		TEST_METHOD(Vector_Empty)
		{
			Vector<int> a;
			Assert::IsTrue(a.IsEmpty());
			a.PushBack(4);
			Assert::IsFalse(a.IsEmpty());
			a.PopBack();
			Assert::IsTrue(a.IsEmpty());
		}

		TEST_METHOD(Vector_Front_Back)
		{
			{
				Vector<int> a;
				auto exception = [&a] { a.Front(); };
				Assert::ExpectException<std::exception>(exception);
				auto exception2 = [&a] { a.Back(); };
				Assert::ExpectException<std::exception>(exception2);
				a.PushBack(4);
				Assert::AreEqual(a.Front(), a.Back());
				Assert::AreEqual(4, a.Back());
				a.PushBack(5);
				Assert::AreNotEqual(a.Front(), a.Back());
				Assert::AreEqual(5, a.Back());
			}
			{
				const  Vector<int> a;
				Vector<int> b;
				b.PushBack(4);
				b.PushBack(5);
				auto exception = [&a] { a.Front(); };
				Assert::ExpectException<std::exception>(exception);
				auto exception2 = [&a] { a.Back(); };
				Assert::ExpectException<std::exception>(exception2);
				const Vector<int> x = b;
				Assert::AreNotEqual(x.Front(), x.Back());
				Assert::AreEqual(5, x.Back());
			}
		}

		TEST_METHOD(Vector_Size_Capacity)
		{
			Vector<int> a(0), b(3);
			Assert::AreEqual(0U, a.Capacity());
			Assert::AreEqual(3U, b.Capacity());
			a.Reserve(55);
			Assert::AreEqual(55U, a.Capacity());
			Assert::AreEqual(0U, a.Size());
			a.PushBack(2);
			a.PushBack(3);
			Assert::AreEqual(2U, a.Size());
		}
		
		TEST_METHOD(Vector_Resize)
		{
			Vector<int> x;
			x.PushBack(0);
			x.PushBack(1);
			x.PushBack(2);
			x.PushBack(3);
			x.PushBack(4);
			x.Resize(2);
		}

		TEST_METHOD(Vector_Remove)
		{
			{
				//int
				Vector<int> vector(3);
				vector.PushBack(1);
				vector.PushBack(2);
				vector.PushBack(3);
				bool removed = vector.Remove(4);
				Assert::IsFalse(removed);
				removed = vector.Remove(vector.begin());
				Assert::IsTrue(removed);
				Assert::AreNotEqual(1, *vector.begin());
				Assert::AreEqual(2U, vector.Size());
				removed = vector.Remove(3);
				Assert::AreEqual(1U, vector.Size());
				Assert::IsTrue(removed);
				Assert::AreNotEqual(3, vector.Back());
				Vector<int> a(3);
				a.PushBack(1);
				a.PushBack(2);
				a.PushBack(3);
				a.PushBack(4);
				Assert::AreEqual(a.Size(), 4U);
				removed = a.Remove(a.Find(3), a.Find(3));
				Assert::IsTrue(removed);
				Assert::AreEqual(3U, a.Size());
				a.PushBack(5);
				removed = a.Remove(a.Find(1), a.Find(4));
				Assert::IsTrue(removed);
				Assert::AreEqual(1U, a.Size());
				Assert::AreEqual(5, a.Front());
			}
			{
				//Foo
				Vector<Foo> vector(3);
				Foo x(1), y(2, 2), z(3, 3), w(4, 4);
				vector.PushBack(x);
				vector.PushBack(y);
				vector.PushBack(z);
				bool removed = vector.Remove(w);
				Assert::IsFalse(removed);
				removed = vector.Remove(vector.end());
				Assert::IsFalse(removed);
				removed = vector.Remove(vector.begin());
				Assert::IsTrue(removed);
				Assert::AreNotEqual(x, *vector.begin());
				Assert::AreEqual(2U, vector.Size());
				removed = vector.Remove(z);
				Assert::AreEqual(1U, vector.Size());
				Assert::IsTrue(removed);
				Assert::AreNotEqual(z, vector.Back());
				Vector<Foo> a(3);
				a.PushBack(x);
				a.PushBack(y);
				a.PushBack(z);
				a.PushBack(w);
				Assert::AreEqual(a.Size(), 4U);
				removed = a.Remove(a.Find(z), a.Find(z));
				Assert::IsTrue(removed);
				Assert::AreEqual(3U, a.Size());
				Foo n = (5, 5);
				a.PushBack(n);
				removed = a.Remove(a.Find(x), a.Find(w));
				Assert::IsTrue(removed);
				Assert::AreEqual(1U, a.Size());
				Assert::AreEqual(n, a.Front());
			}
		}

		TEST_METHOD(Vector_Iterator)
		{
			Vector<int>::Iterator it, it2;
			Assert::IsTrue(it == it2);
			Vector<int> a, b;
			it = a.end();
			it2 = b.end();
			Assert::IsTrue(it != it2);
			it = a.PushBack(11);
			Assert::AreEqual(11, *it);
			it = a.end();
			auto exception = [&it] { *it; };
			Assert::ExpectException<std::exception>(exception);
			a.PushBack(44);
			it = a.begin();
			it++;
			Assert::AreEqual(44, *it);
		}

		TEST_METHOD(Vector_CustomStrat)
		{
			class AdvancedInc
			{
			public:
				uint32_t operator()(uint32_t size, uint32_t capacity)
				{
					return 222 + 0 * (size + capacity);
				}
			};

			Vector<int, AdvancedInc> a(0);
			Assert::AreEqual(0U, a.Capacity());
			a.PushBack(1);
			Assert::AreEqual(222U, a.Capacity());
		}

		TEST_METHOD(Vector_Vectors)
		{
			Vector<int> a, b;
			Vector < Vector<int>> c;
			c.PushBack(a);
			c.PushBack(b);
			c.PopBack();
			b.PushBack(55);
			c.PushBack(b);
			Assert::AreEqual(55, c.Back().Back());
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState VectorTest::sStartMemState;
}