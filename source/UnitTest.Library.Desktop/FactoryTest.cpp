#include "stdafx.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "RTTI.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(FactoryTest)
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

		ConcreteFactory(Foo, RTTI)

			TEST_METHOD(FactoryConstruction)
		{
			Assert::AreEqual(0U, Factory<RTTI>::Size());
			{
				FooFactory f;
				Assert::AreEqual(1U, Factory<RTTI>::Size());
			}
			Assert::AreEqual(0U, Factory<RTTI>::Size());
		}

		TEST_METHOD(FactoryCreation)
		{
			FooFactory f;
			RTTI* rttiPtr = Factory<RTTI>::Create("Foo");
			RTTI* nullPtr = Factory<RTTI>::Create("Bar");
			Assert::IsTrue(nullPtr == nullptr);
			Assert::IsTrue(rttiPtr->Is("Foo"));
			delete rttiPtr;
		}

		TEST_METHOD(FactoryIterator)
		{
			{
				FooFactory f;
				auto beginIt = Factory<RTTI>::begin();
				auto endIt = Factory<RTTI>::end();
				Assert::IsTrue(beginIt != endIt);
			}
			auto beginIt = Factory<RTTI>::begin();
			auto endIt = Factory<RTTI>::end();
			Assert::IsTrue(beginIt == endIt);
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState FactoryTest::sStartMemState;
}