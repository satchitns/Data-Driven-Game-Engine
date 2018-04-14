#include "stdafx.h"
#include "DebugAction.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(DebugAction)

	DebugAction::DebugAction()
		:Action(TypeIdInstance())
	{
	}

	void DebugAction::Update(FieaGameEngine::WorldState & state)
	{
		state;
		std::wstring wideString;
		OutputDebugString(wideString.c_str());
		UpdateCalled = true;
	}

	std::string& DebugAction::String()
	{
		return mString;
	}

	FieaGameEngine::Scope* DebugAction::Clone()
	{
		return new DebugAction(*this);
	}
}