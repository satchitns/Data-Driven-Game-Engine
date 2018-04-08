#pragma once
#include "Action.h"
namespace FieaGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)
	public:
		ActionDestroyAction();
		~ActionDestroyAction() = default;
		ActionDestroyAction(const ActionDestroyAction&) = delete;
		ActionDestroyAction(ActionDestroyAction&&) = default;
		ActionDestroyAction& operator=(const ActionDestroyAction&) = delete;
		ActionDestroyAction& operator=(ActionDestroyAction&&) = default;

		/**
		*@brief Update function that removes action from parent and deletes it
		*@param World State object containing time and pointers for context
		**/
		void Update(WorldState& state) override;

		/*
		*@brief accessor for instance name
		*@return reference to instance name
		*/
		std::string& InstanceName()
		{
			return mInstanceName;
		}

		static Vector<Signature> Signatures()
		{
			return Action::Signatures().Append({
				Signature("InstanceName", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ActionDestroyAction, mInstanceName))
			});
		}
	private:
		std::string mInstanceName;
	};
}