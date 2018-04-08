#pragma once
#include "Attributed.h"

namespace FieaGameEngine
{
	class WorldState;
	class Entity;
	class ActionList;

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)
	public:
		Action() = delete;
		virtual ~Action() = default;
		Action(const Action&) = delete;
		Action(Action&&) = default;
		Action& operator=(const Action&) = delete;
		Action& operator=(Action&&) = default;

		/**
		*@brief Update function that needs to be overloaded in concrete actions
		*@param worldState contains current state of the world
		**/
		virtual void Update(WorldState& worldState) = 0;

		/**
		*@brief Returns the name of the action
		*@return name of the action
		**/
		const std::string& Name() const;

		/**
		*@brief Sets the name of the action
		*@param name - name of the action
		**/
		void SetName(const std::string& name);

		/**
		*@brief Sets the entity the action belongs to
		*@param Reference to entity
		**/
		void SetEntity(Entity & entity);

		/**
		*@brief Sets the action list the action belongs to
		*@param Reference to action list
		**/
		void SetParent(ActionList& list);

		static Vector<Signature> Signatures()
		{
			return Vector<Signature>
			{
				Signature("Name", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(Action, mName))
			};
		}
	protected:
		Action(uint64_t);
	private:
		std::string mName;
	};
}