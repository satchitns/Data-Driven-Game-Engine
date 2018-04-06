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
		ActionDestroyAction(ActionDestroyAction&&);
		ActionDestroyAction& operator=(const ActionDestroyAction&) = delete;
		ActionDestroyAction& operator=(ActionDestroyAction&&);

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
	private:
		std::string mInstanceName;
	};
}