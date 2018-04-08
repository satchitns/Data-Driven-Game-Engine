#include "stdafx.h"
#include "CppUnitTest.h"
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default:4201)
#include "Datum.h"
#include "Foo.h"
#include "TestRTTI.h"
#include "Attributed.h"
#include "AttributedFoo.h"

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
	TEST_CLASS(AttributedTest)
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

		TEST_METHOD(Attributed_Ctor_Dtor)
		{
			AttributedFoo x;
			AttributedFoo y(x);
			AttributedFoo nFoo(move(y));
			AttributedFoo xFoo, yFoo;
			xFoo = nFoo;
			yFoo = move(nFoo);
			yFoo.mHealth = 100.0f;
			yFoo.AppendAuxiliaryAttribute("AuxiliaryHealth");
			auto exprn0 = [&yFoo] {yFoo.AppendAuxiliaryAttribute("Health"); };
			Assert::ExpectException<exception>(exprn0);
			Vector<Scope::TableElement> aVector = yFoo.AuxiliaryAttributes();
			Assert::AreEqual(1U, aVector.Size());
			Vector<Attributed::Signature> pVector = yFoo.PrescribedAttributes();
			Assert::AreEqual(11U, pVector.Size());
			Vector<Scope::TableElement> vector = yFoo.Attributes();
			Assert::AreEqual(13U, vector.Size());
			Assert::IsFalse(yFoo.IsAttribute("BanjoKazooie"));
			Assert::IsTrue(yFoo.IsAuxiliaryAttribute("AuxiliaryHealth"));
			yFoo.strArray[2] = "hello";
			Assert::IsTrue("hello" == yFoo["StrArray"].Get<string>(2));
			Assert::IsTrue(100.0f == yFoo["Health"].Get<float>(0));
		}
	private:
		static _CrtMemState sStartMemState;
		AttributedFoo dummy;
	};
	_CrtMemState AttributedTest::sStartMemState;
}