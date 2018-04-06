#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList)
	public:
		ActionListIf();
		~ActionListIf() = default;
		ActionListIf(const ActionListIf&) = delete;
		ActionListIf(ActionListIf&&);
		ActionListIf& operator=(const ActionListIf&) = delete;
		ActionListIf& operator=(ActionListIf&&);

		/**
		*@brief Update function that calls either the if Action or the else Action's update
		*depending on condition
		*@param World State object containing time and pointers for context
		**/
		void Update(WorldState& worldState) override;

		/**
		*@brief Set the if action
		*@param if action reference
		**/
		void SetIfCase(Action& action);

		/**
		*@brief Set the else action
		*@param else action reference
		**/
		void SetElseCase(Action& action);

		/**
		*@brief Get the if action datum
		*@param if action datum reference
		**/
		Datum& GetIf() const;

		/**
		*@brief Get the else action datum
		*@param else action datum reference
		**/
		Datum& GetElse() const;

		/**
		*@brief Get the condition integer
		*@param condition integer reference
		**/
		int& Condition()
		{ 
			return mCondition; 
		};
	private:
		int mCondition = 0;
		Datum* mIfDatum = nullptr;
		Datum* mElseDatum = nullptr;
	};
}