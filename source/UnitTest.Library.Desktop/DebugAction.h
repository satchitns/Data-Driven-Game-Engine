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
		DebugAction(const DebugAction&) = default;
		DebugAction(DebugAction&&) = default;
		DebugAction& operator=(const DebugAction&) = default;
		DebugAction& operator=(DebugAction&&) = default;

		std::string& DebugAction::String();
		void Update(FieaGameEngine::WorldState& state) override;

		static FieaGameEngine::Vector<Signature> Signatures()
		{
			return Action::Signatures().Append({
				Signature("DebugString", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(DebugAction, mString))
			});
		}

		bool UpdateCalled = false;
	protected:
		virtual Scope* Clone() override;
	private:
		std::string mString;
	};
}