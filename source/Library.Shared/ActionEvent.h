#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionEvent final : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action)
	public:
		ActionEvent(); 
		~ActionEvent() = default;
		ActionEvent(const ActionEvent&) = default;
		ActionEvent(ActionEvent&&) = default;
		ActionEvent& operator=(const ActionEvent&) = default;
		ActionEvent& operator=(ActionEvent&&) = default;

		/**
		*@brief Enqueue the event
		*@param state world state for context
		**/
		void Update(WorldState& state) override;
	
		static Vector<Signature> Signatures()
		{
			return Action::Signatures().Append({
				Signature("Subtype", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ActionEvent, mSubtype)),
				Signature("Delay", FieaGameEngine::Datum::DatumType::FLOAT, 1, offsetof(ActionEvent, mDelay))
				});
		}
	protected:
		virtual Scope* Clone() override;
	private:
		std::string mSubtype;
		float mDelay = 0.0f;
	};
}