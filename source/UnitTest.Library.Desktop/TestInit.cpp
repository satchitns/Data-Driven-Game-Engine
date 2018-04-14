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
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "ActionEvent.h"
#include "ReactionAttributed.h"
#include "EventMessageAttributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace UnitTestLibraryDesktop
{
	TEST_MODULE_INITIALIZE(ClassInit)
	{
		Register(World);
		Register(Sector);
		Register(Entity);
		Register(Action);
		Register(ActionList);
		Register(ActionListIf);
		Register(ActionCreateAction);
		Register(ActionDestroyAction);
		Register(ActionExpression);
		Register(DebugAction);
		Register(Monster);
		Register(AttributedFoo);
		Register(Reaction);
		Register(ReactionAttributed);
		Register(EventMessageAttributed);
		Register(ActionEvent);
	}

	TEST_MODULE_CLEANUP(ClassCleanup)
	{

	}

	TEST_CLASS(TestInit)
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

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState TestInit::sStartMemState;
}