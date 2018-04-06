#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)
	public:
		ActionList();
		virtual ~ActionList() = default;
		ActionList(const ActionList&) = delete;
		ActionList(ActionList&&);
		ActionList& operator=(const ActionList&) = delete;
		ActionList& operator=(ActionList&&);

		/**
		*@brief Update function for the actionlist that runs every frame
		*and calls Update on each sub-action
		*@param World State object containing time and pointers for context
		**/
		virtual void Update(WorldState& worldState) override;
		
		/**
		*@brief Returns a reference to the actions datum
		*@return reference to actions datum
		**/
		Datum & ActionList::Actions() const;

		/**
		*@brief Helper function to create an action of type className and name instanceName within the actionlist
		*@param className - type of Action to create
		*@param instanceName - name of Action instance
		*@return pointer to action
		**/
		class Action* CreateAction(const std::string & className, const std::string & instanceName);

		static const std::string sActions;
	protected:
		ActionList(uint64_t id);
	private:
		Datum * mActions = nullptr;
	};
}