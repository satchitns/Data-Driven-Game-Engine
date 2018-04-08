#pragma once
#include "Action.h"
namespace FieaGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)
	public:
		ActionCreateAction();
		~ActionCreateAction() = default;
		ActionCreateAction(const ActionCreateAction&) = delete;
		ActionCreateAction(ActionCreateAction&&) = default;
		ActionCreateAction& operator=(const ActionCreateAction&) = delete;
		ActionCreateAction& operator=(ActionCreateAction&&) = default;

		/**
		*@brief Update function that creates the given action and appends it in parent container
		*@param World State object containing time and pointers for context
		**/
		void Update(WorldState& state) override;

		/*
		*@brief accessor for prototpye name
		*@return reference to prototype
		*/
		std::string& Prototype()
		{
			return mPrototype;
		}

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
				Signature("InstanceName", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mInstanceName)),
				Signature("Prototype", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(ActionCreateAction, mPrototype))
			});
		}

	private:
		std::string mPrototype;
		std::string mInstanceName;
	};
}