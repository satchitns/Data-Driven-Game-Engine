#pragma once
#include "Action.h"

namespace UnitTestLibraryDesktop
{
	class DebugAction final : public FieaGameEngine::Action
	{
		RTTI_DECLARATIONS(DebugAction, Action)
	public:
		DebugAction();
		~DebugAction() = default;
		DebugAction(const DebugAction&) = delete;
		DebugAction(DebugAction&&);
		DebugAction& operator=(const DebugAction&) = delete;
		DebugAction& operator=(DebugAction&&);

		std::string& DebugAction::String();
		void Update(FieaGameEngine::WorldState& state) override;
	private:
		std::string mString;
	};
}