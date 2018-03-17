#include "stdafx.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "SList.h"
#include "AssertHelp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
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

		TEST_METHOD(SListPushBackTest)
		{
			int32_t a = 5, b = 6;

			//int32_t List
			SList<int32_t> int32_tList;

			int32_tList.PushBack(a);
			Assert::AreEqual(1U, int32_tList.Size());
			Assert::AreEqual(a, int32_tList.Back());
			Assert::AreEqual(int32_tList.Front(), int32_tList.Back());
			Assert::AreSame(int32_tList.Front(), int32_tList.Back());

			int32_tList.PushBack(b);
			Assert::AreEqual(2U, int32_tList.Size());
			Assert::AreEqual(b, int32_tList.Back());
			Assert::AreNotEqual(int32_tList.Front(), int32_tList.Back());

			//int32_t ptr list
			SList<int32_t*> int32_tPtrList;

			int32_tPtrList.PushBack(&a);
			Assert::AreEqual(1U, int32_tPtrList.Size());
			Assert::AreEqual(&a, int32_tPtrList.Back());
			Assert::AreEqual(int32_tPtrList.Front(), int32_tPtrList.Back());
			Assert::AreSame(int32_tPtrList.Front(), int32_tPtrList.Back());

			int32_tPtrList.PushBack(&b);
			Assert::AreEqual(2U, int32_tPtrList.Size());
			Assert::AreEqual(&b, int32_tPtrList.Back());
			Assert::AreNotEqual(int32_tPtrList.Front(), int32_tPtrList.Back());

			//Foo list
			int32_t c = 10, d = 11;
			SList<Foo> fooList;
			Foo A(a, c), B(b, d);

			fooList.PushBack(A);
			Assert::AreEqual(1U, fooList.Size());
			Assert::AreEqual(A, fooList.Back());
			Assert::AreEqual(fooList.Back(), fooList.Front());
			Assert::AreSame(fooList.Back(), fooList.Front());

			fooList.PushBack(B);
			Assert::AreEqual(2U, fooList.Size());
			Assert::AreEqual(B, fooList.Back());
			Assert::AreNotEqual(fooList.Back(), fooList.Front());
		}

		TEST_METHOD(SListPushFrontTest)
		{
			//int32_t list
			SList<int32_t> int32_tList;
			int32_t a = 5, b = 6;

			int32_tList.PushFront(a);
			Assert::AreEqual(1U, int32_tList.Size());
			Assert::AreEqual(a, int32_tList.Front());
			Assert::AreEqual(int32_tList.Front(), int32_tList.Back());
			Assert::AreSame(int32_tList.Front(), int32_tList.Back());

			int32_tList.PushFront(b);
			Assert::AreEqual(2U, int32_tList.Size());
			Assert::AreEqual(b, int32_tList.Front());
			Assert::AreNotEqual(int32_tList.Front(), int32_tList.Back());

			//int32_t ptr list
			SList<int32_t*> int32_tPtrList;

			int32_tPtrList.PushFront(&a);
			Assert::AreEqual(1U, int32_tPtrList.Size());
			Assert::AreEqual(&a, int32_tPtrList.Front());
			Assert::AreEqual(int32_tPtrList.Front(), int32_tPtrList.Back());
			Assert::AreSame(int32_tPtrList.Front(), int32_tPtrList.Back());

			int32_tPtrList.PushFront(&b);
			Assert::AreEqual(2U, int32_tPtrList.Size());
			Assert::AreEqual(&b, int32_tPtrList.Front());
			Assert::AreNotEqual(int32_tPtrList.Front(), int32_tPtrList.Back());

			//Foo list
			SList<Foo> fooList;
			Foo A(a, b), B(b, a);

			fooList.PushFront(A);
			Assert::AreEqual(1U, fooList.Size());
			Assert::AreEqual(A, fooList.Front());
			Assert::AreEqual(fooList.Front(), fooList.Back());
			Assert::AreSame(fooList.Front(), fooList.Back());

			fooList.PushFront(B);
			Assert::AreEqual(2U, fooList.Size());
			Assert::AreEqual(B, fooList.Front());
			Assert::AreNotEqual(fooList.Front(), fooList.Back());
		}

		TEST_METHOD(SListDefaultConstructorTest)
		{
			//int32_t
			SList<int32_t> *int32_tList = new SList<int32_t>();
			delete int32_tList;

			//int32_t ptr
			SList<int32_t*> *int32_tPtrList = new SList<int32_t*>();
			delete int32_tPtrList;

			//float
			SList<Foo> *fooList = new SList<Foo>();
			delete fooList;

			SList<int32_t> i, j;
			i.PushBack(4);
			i.PushBack(6);
			j = std::move(i);
			Assert::AreEqual(2U, j.Size());
			Assert::AreEqual(0U, i.Size());
			Assert::AreEqual(6, j.Back());
			SList<int32_t> k(std::move(j));
			Assert::AreEqual(2U, k.Size());
			Assert::AreEqual(0U, j.Size());
			Assert::AreEqual(6, k.Back());
		}

		TEST_METHOD(SListCopyConstructorTest)
		{
			int32_t a = 5, b = 6;

			//int32_t 
			SList<int32_t> int32_tList;

			int32_tList.PushBack(a);
			int32_tList.PushBack(b);

			SList<int32_t> copyint32_tList(int32_tList);
			Assert::AreNotSame(int32_tList.Front(), copyint32_tList.Front());
			Assert::AreNotSame(int32_tList.Back(), copyint32_tList.Back());
			int32_tList.Front() = b;
			Assert::AreNotEqual(b, copyint32_tList.Front());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList;

			int32_tPtrList.PushBack(&a);
			int32_tPtrList.PushBack(&b);

			SList<int32_t*> copyint32_tPtrList(int32_tPtrList);
			Assert::AreNotSame(int32_tPtrList.Front(), copyint32_tPtrList.Front());
			Assert::AreNotSame(int32_tPtrList.Back(), copyint32_tPtrList.Back());
			int32_tPtrList.Front() = &b;
			Assert::AreNotEqual(&b, copyint32_tPtrList.Front());

			//Foo
			SList<Foo> fooList;
			Foo A(a, b), B(b, a);

			fooList.PushBack(A);
			fooList.PushBack(B);

			SList<Foo> copyFooList(fooList);
			Assert::AreNotSame(fooList.Front(), copyFooList.Front());
			Assert::AreNotSame(fooList.Back(), copyFooList.Back());
			fooList.Front() = B;
			Assert::AreNotEqual(B, copyFooList.Front());
		}

		TEST_METHOD(SListFront)
		{
			//int32_t
			SList<int32_t>  int32_tList;
			const SList<int32_t> *constint32_tList = &int32_tList;
			int32_t a = 5, b = 6;

			auto exception = [&int32_tList] {int32_tList.Front(); };
			Assert::ExpectException<std::exception>(exception);


			auto constException = [&constint32_tList] {constint32_tList->Front(); };
			Assert::ExpectException<std::exception>(constException);

			int32_tList.PushFront(a);
			Assert::AreEqual(a, int32_tList.Front());
			int32_tList.Front() = b;
			Assert::AreEqual(b, int32_tList.Front());

			Assert::AreEqual(b, constint32_tList->Front());

			//int32_t ptr
			SList<int32_t*>  int32_tPtrList;
			const SList<int32_t*> *constint32_tPtrList = &int32_tPtrList;

			auto exceptionPtr = [&int32_tPtrList] {int32_tPtrList.Front(); };
			Assert::ExpectException<std::exception>(exceptionPtr);


			auto constExceptionPtr = [&constint32_tPtrList] {constint32_tPtrList->Front(); };
			Assert::ExpectException<std::exception>(constExceptionPtr);

			int32_tPtrList.PushFront(&a);
			Assert::AreEqual(&a, int32_tPtrList.Front());
			int32_tPtrList.Front() = &b;
			Assert::AreEqual(&b, int32_tPtrList.Front());

			Assert::AreEqual(&b, constint32_tPtrList->Front());

			//Foo
			SList<Foo>  fooList;
			const SList<Foo> *constFooList = &fooList;
			Foo A(a, b), B(b, a);

			auto exceptionFoo = [&fooList] {fooList.Front(); };
			Assert::ExpectException<std::exception>(exceptionFoo);


			auto constExceptionFoo = [&constFooList] {constFooList->Front(); };
			Assert::ExpectException<std::exception>(constExceptionFoo);

			fooList.PushFront(A);
			Assert::AreEqual(A, fooList.Front());
			fooList.Front() = B;
			Assert::AreEqual(B, fooList.Front());

			Assert::AreEqual(B, constFooList->Front());
		}

		TEST_METHOD(SListBack)
		{
			//int32_t
			SList<int32_t>  int32_tList;
			const SList<int32_t> *constint32_tList = &int32_tList;
			int32_t a = 5, b = 6;

			auto exception = [&int32_tList] {int32_tList.Back(); };
			Assert::ExpectException<std::exception>(exception);


			auto constException = [&constint32_tList] {constint32_tList->Back(); };
			Assert::ExpectException<std::exception>(constException);

			int32_tList.PushBack(a);
			Assert::AreEqual(a, int32_tList.Back());
			int32_tList.Back() = b;
			Assert::AreEqual(b, int32_tList.Back());

			Assert::AreEqual(b, constint32_tList->Back());

			//int32_t ptr
			SList<int32_t*>  int32_tPtrList;
			const SList<int32_t*> *constint32_tPtrList = &int32_tPtrList;

			auto exceptionPtr = [&int32_tPtrList] {int32_tPtrList.Back(); };
			Assert::ExpectException<std::exception>(exceptionPtr);


			auto constExceptionPtr = [&constint32_tPtrList] {constint32_tPtrList->Back(); };
			Assert::ExpectException<std::exception>(constExceptionPtr);

			int32_tPtrList.PushBack(&a);
			Assert::AreEqual(&a, int32_tPtrList.Back());
			int32_tPtrList.Back() = &b;
			Assert::AreEqual(&b, int32_tPtrList.Back());

			Assert::AreEqual(&b, constint32_tPtrList->Back());

			//Foo
			SList<Foo>  fooList;
			const SList<Foo> *constFooList = &fooList;
			Foo A(a, b), B(b, a);

			auto exceptionFoo = [&fooList] {fooList.Back(); };
			Assert::ExpectException<std::exception>(exceptionFoo);


			auto constExceptionFoo = [&constFooList] {constFooList->Back(); };
			Assert::ExpectException<std::exception>(constExceptionFoo);

			fooList.PushBack(A);
			Assert::AreEqual(A, fooList.Back());
			fooList.Back() = B;
			Assert::AreEqual(B, fooList.Back());

			Assert::AreEqual(B, constFooList->Back());
		}

		TEST_METHOD(SListPopFront)
		{
			//int32_t 
			SList<int32_t> int32_tList;
			int32_t a = 6;
			auto exception = [&int32_tList] {int32_tList.PopFront(); };
			Assert::ExpectException<std::exception>(exception);
			int32_tList.PushFront(a);
			int32_tList.PopFront();
			Assert::IsTrue(int32_tList.IsEmpty());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList;
			auto exceptionPtr = [&int32_tPtrList] {int32_tPtrList.PopFront(); };
			Assert::ExpectException<std::exception>(exceptionPtr);
			int32_tPtrList.PushFront(&a);
			int32_tPtrList.PopFront();
			Assert::IsTrue(int32_tPtrList.IsEmpty());

			//Foo
			SList<Foo> FooList;
			Foo A(a);
			auto exceptionFoo = [&FooList] {FooList.PopFront(); };
			Assert::ExpectException<std::exception>(exceptionFoo);
			FooList.PushFront(A);
			FooList.PopFront();
			Assert::IsTrue(FooList.IsEmpty());
		}

		TEST_METHOD(SListEmpty)
		{
			//int32_t 
			SList<int32_t> int32_tList;
			int32_t a = 5;
			Assert::IsTrue(int32_tList.IsEmpty());
			int32_tList.PushBack(a);
			Assert::IsFalse(int32_tList.IsEmpty());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList;
			Assert::IsTrue(int32_tPtrList.IsEmpty());
			int32_tPtrList.PushBack(&a);
			Assert::IsFalse(int32_tPtrList.IsEmpty());

			//Foo
			SList<Foo> FooList;
			Foo A(a);
			Assert::IsTrue(FooList.IsEmpty());
			FooList.PushBack(A);
			Assert::IsFalse(FooList.IsEmpty());
		}

		TEST_METHOD(SListSize)
		{
			//int32_t
			SList<int32_t> int32_tList;
			int32_t a = 6;
			Assert::AreEqual(0U, int32_tList.Size());
			int32_tList.PushBack(a);
			Assert::AreEqual(1U, int32_tList.Size());
			int32_tList.PushBack(a);
			Assert::AreEqual(2U, int32_tList.Size());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList;
			Assert::AreEqual(0U, int32_tPtrList.Size());
			int32_tPtrList.PushBack(&a);
			Assert::AreEqual(1U, int32_tPtrList.Size());
			int32_tPtrList.PushBack(&a);
			Assert::AreEqual(2U, int32_tPtrList.Size());

			//foo
			SList<Foo> FooList;
			Foo A(a);
			Assert::AreEqual(0U, FooList.Size());
			FooList.PushBack(A);
			Assert::AreEqual(1U, FooList.Size());
			FooList.PushBack(A);
			Assert::AreEqual(2U, FooList.Size());
		}

		TEST_METHOD(SListClear)
		{
			//int32_t 
			SList<int32_t> int32_tList;
			int32_t a = 5, b = 6;
			int32_tList.PushBack(a);
			int32_tList.PushBack(b);
			int32_tList.Clear();
			Assert::IsTrue(int32_tList.IsEmpty());
			Assert::AreEqual(0U, int32_tList.Size());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList;
			int32_tPtrList.PushBack(&a);
			int32_tPtrList.PushBack(&b);
			int32_tPtrList.Clear();
			Assert::IsTrue(int32_tPtrList.IsEmpty());
			Assert::AreEqual(0U, int32_tPtrList.Size());

			//foo
			SList<Foo> fooList;
			Foo A(a, b), B(b, a);
			fooList.PushBack(A);
			fooList.PushBack(B);
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
			Assert::AreEqual(0U, fooList.Size());
		}

		TEST_METHOD(SListDestructor)
		{
			//int32_t 
			SList<int32_t> *int32_tList = new SList<int32_t>();
			int32_t a = 5, b = 6;
			int32_tList->PushBack(a);
			int32_tList->PushBack(b);
			delete int32_tList;

			//int32_t 
			SList<int32_t*> *int32_tPtrList = new SList<int32_t*>();
			int32_tPtrList->PushBack(&a);
			int32_tPtrList->PushBack(&b);
			delete int32_tPtrList;

			//Foo
			SList<Foo> *fooList = new SList<Foo>();
			fooList->PushBack(a);
			fooList->PushBack(b);
			delete fooList;
		}

		TEST_METHOD(SListAssignmentOverload)
		{
			//int32_t 
			SList<int32_t> int32_tList, secondList;
			int32_t a = 5, b = 6;
			int32_tList.PushBack(a);
			int32_tList.PushBack(b);

			int32_tList = int32_tList;
			Assert::AreEqual(a, int32_tList.Front());

			secondList = int32_tList;
			Assert::AreEqual(int32_tList.Front(), secondList.Front());
			Assert::AreEqual(int32_tList.Back(), secondList.Back());
			Assert::AreNotSame(int32_tList.Front(), secondList.Front());
			Assert::AreNotSame(int32_tList.Back(), secondList.Back());

			//int32_t ptr
			SList<int32_t*> int32_tPtrList, secondPtrList;
			int32_tPtrList.PushBack(&a);
			int32_tPtrList.PushBack(&b);

			int32_tPtrList = int32_tPtrList;
			Assert::AreEqual(&a, int32_tPtrList.Front());

			secondPtrList = int32_tPtrList;
			Assert::AreEqual(int32_tPtrList.Front(), secondPtrList.Front());
			Assert::AreEqual(int32_tPtrList.Back(), secondPtrList.Back());
			Assert::AreNotSame(int32_tPtrList.Front(), secondPtrList.Front());
			Assert::AreNotSame(int32_tPtrList.Back(), secondPtrList.Back());

			//foo
			SList<Foo> fooList, secondFooList;
			Foo A(a, b), B(b, a);
			fooList.PushBack(A);
			fooList.PushBack(B);

			fooList = fooList;
			Assert::AreEqual(A, fooList.Front());

			secondFooList = fooList;
			Assert::AreEqual(fooList.Front(), secondFooList.Front());
			Assert::AreEqual(fooList.Back(), secondFooList.Back());
			Assert::AreNotSame(fooList.Front(), secondFooList.Front());
			Assert::AreNotSame(fooList.Back(), secondFooList.Back());
		}

		TEST_METHOD(SListIteratorBeginEnd)
		{
			{
				//int32_ts
				SList<int32_t> a, b;
				int32_t x = 1, y = 2;
				a.PushBack(x);
				a.PushBack(y);
				SList<int32_t>::Iterator it, it2;
				Assert::IsTrue(it == it2);
				it = a.end();
				it2 = b.end();
				Assert::IsTrue(it != it2);
				it = a.begin();
				it2 = a.begin();
				Assert::IsTrue(it == it2);
				it2 = b.begin();
				Assert::IsTrue(it != it2);
				int32_t sum = 0;
				for (it = a.begin(); it != a.end(); ++it)
				{
					sum += (*it);
				}
				Assert::AreEqual(x + y, sum);
			}
			{
				//int32_t ptrs
				SList<int32_t*> a, b;
				int32_t x = 1, y = 2;
				a.PushBack(&x);
				a.PushBack(&y);
				SList<int32_t*>::Iterator it, it2;
				Assert::IsTrue(it == it2);
				it = a.end();
				it2 = b.end();
				Assert::IsTrue(it != it2);
				it = a.begin();
				it2 = a.begin();
				Assert::IsTrue(it == it2);
				it2 = b.begin();
				Assert::IsTrue(it != it2);
				int32_t sum = 0;
				for (it = a.begin(); it != a.end(); ++it)
				{
					sum += *(*it);
				}
				Assert::AreEqual(x + y, sum);
			}
			{
				//Foos
				SList<Foo> a, b;
				Foo x = (1, 2), y = (2, 1);
				a.PushBack(x);
				a.PushBack(y);
				SList<Foo>::Iterator it, it2;
				Assert::IsTrue(it == it2);
				it = a.end();
				it2 = b.end();
				Assert::IsTrue(it != it2);
				it = a.begin();
				it2 = a.begin();
				Assert::IsTrue(it == it2);
				it2 = b.begin();
				Assert::IsTrue(it != it2);
				int32_t sum = 0;
				for (it = a.begin(); it != a.end(); ++it)
				{
					sum += ((*it).Data());
				}
				Assert::AreEqual(x.Data() + y.Data(), sum);
			}
		}

		TEST_METHOD(SListIteratorComparisonOperators)
		{
			{
				//int32_ts
				SList<int32_t> a;
				int32_t x = 1, y = 2;
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.begin();
				auto it2 = a.begin();
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
				it2 = a.end();
				Assert::IsTrue(it != it2);
				Assert::IsFalse(it == it2);
			}
			{
				//int32_t ptrs
				SList<int32_t*> a;
				int32_t x = 1, y = 2;
				a.PushBack(&x);
				a.PushBack(&y);
				auto it = a.begin();
				auto it2 = a.begin();
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
				it2 = a.end();
				Assert::IsTrue(it != it2);
				Assert::IsFalse(it == it2);
			}
			{
				//Foos
				SList<Foo> a;
				Foo x = (1, 1), y = (2, 2);
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.begin();
				auto it2 = a.begin();
				Assert::IsTrue(it == it2);
				Assert::IsFalse(it != it2);
				it2 = a.end();
				Assert::IsTrue(it != it2);
				Assert::IsFalse(it == it2);
			}
		}

		TEST_METHOD(SListIteratorIncrements)
		{
			{
				//int32_ts
				SList<int32_t> a;
				int32_t x = 1, y = 2;
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.begin(), it2 = a.begin();
				Assert::AreEqual(x, *it);
				++it;
				Assert::AreEqual(y, *it);
				++it;
				Assert::IsTrue(a.end() == it);
				auto exception = [&it] { ++it; };
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(x, *it2);
				it2++;
				Assert::AreEqual(y, *it2);
				it2++;
				Assert::IsTrue(a.end() == it2);
				auto postException = [&it2] { it2++; };
				Assert::ExpectException<std::exception>(postException);
			}
			{
				//int32_t ptrs
				SList<int32_t*> a;
				int32_t x = 1, y = 2;
				a.PushBack(&x);
				a.PushBack(&y);
				auto it = a.begin(), it2 = a.begin();
				Assert::AreEqual(&x, *it);
				++it;
				Assert::AreEqual(&y, *it);
				++it;
				Assert::IsTrue(a.end() == it);
				auto exception = [&it] { ++it; };
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(&x, *it2);
				it2++;
				Assert::AreEqual(&y, *it2);
				it2++;
				Assert::IsTrue(a.end() == it2);
				auto postException = [&it2] { it2++; };
				Assert::ExpectException<std::exception>(postException);
			}
			{
				//Foos
				SList<Foo> a;
				Foo x = (1, 1), y = (2, 2);
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.begin(), it2 = a.begin();
				Assert::AreEqual(x, *it);
				++it;
				Assert::AreEqual(y, *it);
				++it;
				Assert::IsTrue(a.end() == it);
				auto exception = [&it] { ++it; };
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(x, *it2);
				it2++;
				Assert::AreEqual(y, *it2);
				it2++;
				Assert::IsTrue(a.end() == it2);
				auto postException = [&it2] { it2++; };
				Assert::ExpectException<std::exception>(postException);
			}
		}

		TEST_METHOD(SListIteratorDereferenceTest)
		{
			{
				//int32_ts
				SList<int32_t> a;
				int32_t x = 1, y = 2;
				a.PushBack(x);
				a.PushBack(y);

				SList<int32_t>::Iterator it2;
				auto firstException = [&it2] { *it2; };
				Assert::ExpectException<std::exception>(firstException);

				const SList<int32_t>::Iterator it3 = it2;
				auto firstExceptionConst = [&it3] { *it3; };
				Assert::ExpectException<std::exception>(firstExceptionConst);

				const SList<int32_t>::Iterator it4 = a.begin();
				Assert::AreEqual(x, *it4);

				auto it = a.begin();
				Assert::AreEqual(x, *it);

				it++;
				Assert::AreEqual(y, *it);

				it++;
				auto exception = [&it] { *it; };
				Assert::ExpectException<std::exception>(exception);
			}
			{
				//int32_t ptrs
				SList<int32_t*> a;
				int32_t x = 1, y = 2;
				a.PushBack(&x);
				a.PushBack(&y);

				SList<int32_t*>::Iterator it2;
				auto firstException = [&it2] { *it2; };
				Assert::ExpectException<std::exception>(firstException);

				const SList<int32_t*>::Iterator it3 = it2;
				auto firstExceptionConst = [&it3] { *it3; };
				Assert::ExpectException<std::exception>(firstExceptionConst);

				auto it = a.begin();
				Assert::AreEqual(&x, *it);

				const SList<int32_t*>::Iterator it4 = a.begin();
				Assert::AreEqual(&x, *it4);

				it++;
				Assert::AreEqual(&y, *it);

				it++;
				auto exception = [&it] { *it; };
				Assert::ExpectException<std::exception>(exception);
			}
			{
				//Foos
				SList<Foo> a;
				Foo x = (1, 1), y = (2, 2);
				a.PushBack(x);
				a.PushBack(y);

				SList<Foo>::Iterator it2;
				auto firstException = [&it2] { *it2; };
				Assert::ExpectException<std::exception>(firstException);

				const SList<Foo>::Iterator it3 = it2;
				auto firstExceptionConst = [&it3] { *it3; };
				Assert::ExpectException<std::exception>(firstExceptionConst);

				const SList<Foo>::Iterator it4 = a.begin();
				Assert::AreEqual(x, *it4);

				auto it = a.begin();
				Assert::AreEqual(x, *it);

				it++;
				Assert::AreEqual(y, *it);

				it++;
				auto exception = [&it] { *it; };
				Assert::ExpectException<std::exception>(exception);
			}
		}

		TEST_METHOD(SListIteratorInsertAfterTest)
		{
			{
				//int32_ts
				SList<int32_t> a, b;
				int32_t x = 1, y = 2;
				a.PushBack(x);
				a.PushBack(y);
				b = a;
				SList<int32_t>::Iterator it;
				auto exception = [&a, &it, &x] {a.InsertAfter(it, x); }; //should do nothing since it isn't an iterator of a
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(a.Size(), b.Size());
				it = a.begin();
				a.InsertAfter(it, x + y);
				Assert::AreEqual(3U, a.Size());
				++it;
				++it;
				a.InsertAfter(it, x - y);
				++it;
				Assert::AreEqual(4U, a.Size());
				Assert::AreEqual(x - y, *it);
				++it;
				a.InsertAfter(it, x);
				Assert::AreEqual(5U, a.Size());
				Assert::AreEqual(x, a.Back());
			}
			{
				//int32_t ptrs
				SList<int32_t*> a, b;
				int32_t x = 1, y = 2, z = 3, w = 4;
				a.PushBack(&x);
				a.PushBack(&y);
				b = a;
				SList<int32_t*>::Iterator it;
				auto exception = [&a, &it, &x] {a.InsertAfter(it, &x); }; //should do nothing since it isn't an iterator of a
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(a.Size(), b.Size());
				it = a.begin();
				a.InsertAfter(it, &z);
				Assert::AreEqual(3U, a.Size());
				++it;
				++it;
				a.InsertAfter(it, &w);
				++it;
				Assert::AreEqual(4U, a.Size());
				Assert::AreEqual(&w, *it);
				++it;
				a.InsertAfter(it, &x);
				Assert::AreEqual(5U, a.Size());
				Assert::AreEqual(&x, a.Back());
			}
			{
				//Foos
				SList<Foo> a, b;
				Foo x(1, 1), y(2, 2), z(3, 3), w(4, 4);
				a.PushBack(x);
				a.PushBack(y);
				b = a;
				SList<Foo>::Iterator it;
				auto exception = [&a, &it, &x] {a.InsertAfter(it, x); }; //should do nothing since it isn't an iterator of a
				Assert::ExpectException<std::exception>(exception);
				Assert::AreEqual(a.Size(), b.Size());
				it = a.begin();
				a.InsertAfter(it, z);
				Assert::AreEqual(3U, a.Size());
				++it;
				Assert::AreEqual(*it, z);
				++it;
				a.InsertAfter(it, w);
				++it;
				Assert::AreEqual(4U, a.Size());
				Assert::AreEqual(w, *it);
				++it;
				a.InsertAfter(it, x);
				Assert::AreEqual(5U, a.Size());
				Assert::AreEqual(x, a.Back());
			}
		}

		TEST_METHOD(SListIteratorConstructorTest)
		{
			{
				//int32_ts
				SList<int32_t>::Iterator it, it2;
				SList<int32_t> a;
				Assert::IsTrue(it == it2);
				it = a.begin();
				auto it3 = it;
				Assert::IsTrue(it3 == a.begin());
			}
			{
				//int32_t ptrs
				SList<int32_t*>::Iterator it, it2;
				SList<int32_t*> a;
				Assert::IsTrue(it == it2);
				it = a.begin();
				auto it3 = it;
				Assert::IsTrue(it3 == a.begin());
			}
			{
				//Foos
				SList<Foo>::Iterator it, it2;
				SList<Foo> a;
				Assert::IsTrue(it == it2);
				it = a.begin();
				auto it3 = it;
				Assert::IsTrue(it3 == a.begin());
			}
		}

		TEST_METHOD(SListIteratorFind)
		{
			{
				//int32_ts
				SList<int32_t> a;
				int32_t x = 1, y = 2, z = -500;
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.Find(z);
				Assert::IsTrue(it == a.end());
				it = a.Find(x);
				Assert::AreEqual(*it, x);
				it = a.Find(y);
				Assert::AreEqual(*it, y);
			}
			{
				//int32_t ptrs
				SList<int32_t*> a;
				int32_t x = 1, y = 2, z = -500;
				a.PushBack(&x);
				a.PushBack(&y);
				auto it = a.Find(&z);
				Assert::IsTrue(it == a.end());
				it = a.Find(&x);
				Assert::AreEqual(*it, &x);
				it = a.Find(&y);
				Assert::AreEqual(*it, &y);
			}
			{
				//Foos
				SList<Foo> a;
				Foo x(1, 1), y(2, 2), z(-500, -500);
				a.PushBack(x);
				a.PushBack(y);
				auto it = a.Find(z);
				Assert::IsTrue(it == a.end());
				it = a.Find(x);
				Assert::AreEqual(*it, x);
				it = a.Find(y);
				Assert::AreEqual(*it, y);
			}
		}

		TEST_METHOD(SListIteratorRemove)
		{
			{
				//int32_ts
				SList<int32_t> a;
				int32_t x = 1, y = 2, z = -500;
				bool found = false;
				a.PushBack(x);
				a.PushBack(y);
				a.PushBack(z);
				found = a.Remove(x + y);
				Assert::IsFalse(found);
				Assert::AreEqual(3U, a.Size());
				found = a.Remove(x);
				Assert::IsTrue(found);
				Assert::AreEqual(2U, a.Size());
				Assert::AreEqual(y, a.Front());
				found = a.Remove(a.Find(z));
				Assert::IsTrue(found);
				Assert::AreEqual(y, a.Back());
				Assert::AreEqual(1U, a.Size());
				found = a.Remove(y);
				Assert::IsTrue(found);
				a.PushFront(400);
				found = a.Remove(a.begin());
				Assert::IsTrue(found);
				Assert::AreEqual(0U, a.Size());
			}
			{
				//int32_t ptrs
				SList<int32_t*> a;
				int32_t x = 1, y = 2, z = -500, w;
				bool found = false;
				a.PushBack(&x);
				a.PushBack(&y);
				a.PushBack(&z);
				found = a.Remove(&w);
				Assert::AreEqual(3U, a.Size());
				Assert::IsFalse(found);
				found = a.Remove(&x);
				Assert::IsTrue(found);
				Assert::AreEqual(2U, a.Size());
				Assert::AreEqual(&y, a.Front());
				a.Remove(&z);
				Assert::AreEqual(&y, a.Back());
				Assert::AreEqual(1U, a.Size());
				a.Remove(&y);
				Assert::AreEqual(0U, a.Size());
			}
			{
				//Foos
				SList<Foo> a;
				Foo x = (1, 1), y = (2, 2), z = (-500, -500), w;
				bool found = false;
				a.PushBack(x);
				a.PushBack(y);
				a.PushBack(z);
				found = a.Remove(w);
				Assert::AreEqual(3U, a.Size());
				Assert::IsFalse(found);
				found = a.Remove(x);
				Assert::IsTrue(found);
				Assert::AreEqual(2U, a.Size());
				Assert::AreEqual(y, a.Front());
				a.Remove(z);
				Assert::AreEqual(y, a.Back());
				Assert::AreEqual(1U, a.Size());
				a.Remove(y);
				Assert::AreEqual(0U, a.Size());
			}
			{
				//Foos extra
				SList<Foo> a;
				Foo x = (1, 1), y = (2, 2), z = (-500, -500);
				a.PushBack(x);
				a.PushBack(y);
				a.PushBack(z);
				Assert::IsTrue(a.Remove(a.Find(y)));
			}
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState SListTest::sStartMemState;
}