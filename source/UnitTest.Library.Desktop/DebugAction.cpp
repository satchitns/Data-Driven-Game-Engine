#include "stdafx.h"
#include "DebugAction.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(DebugAction)

	DebugAction::DebugAction()
		:Action(TypeIdInstance())
	{
		AddExternalAttribute("DebugString", mString);
	}

	DebugAction::DebugAction(DebugAction&& other)
		: Action(std::move(other)), mString(std::move(other.mString))
	{
		UpdateExternalAttribute("DebugString", mString);
	}

	DebugAction & DebugAction::operator=(DebugAction &&other)
	{
		if (this != &other)
		{
			Action::operator=(std::move(other));
			mString = std::move(other.mString);
			UpdateExternalAttribute("DebugString", mString); 
		}
		return *this;
	}

	void DebugAction::Update(FieaGameEngine::WorldState & state)
	{
		state;
		std::wstring wideString;
		wideString.assign(mString.begin(), mString.end());
		OutputDebugString(wideString.c_str());
	}

	std::string& DebugAction::String()
	{
		return mString;
	}
}