#pragma once
#include<string>
#include "Attributed.h"

namespace FieaGameEngine
{
	class Sector;
	class WorldState;

	/**
	*@brief Class that represents an Entity in the game. Contains a name and is extensible.
	**/
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		Entity();
		virtual ~Entity() = default;
		Entity(const Entity& other) = default;
		Entity(Entity&& other) = default;
		Entity& operator=(const Entity&) = default;
		Entity& operator=(Entity&& other) = default;

		/**
		*@brief Returns the name of the entity
		*@return name of the entity
		**/
		const std::string& Name() const;

		/**
		*@brief Sets the name of the entity
		*@param name - name of the entity
		**/
		void SetName(const std::string& name);

		/**
		*@brief Returns the sector the entity belongs to
		*@return pointer to sector
		**/
		Sector* GetSector() const;

		/**
		*@brief Sets the sector the entity belongs to
		*@param Reference to sector
		**/
		void SetSector(Sector& sector);

		/**
		*@brief Update function for the entity that runs every frame
		*@param World State object containing time and pointers for context
		**/
		virtual void Update(WorldState& state);

		/**
		*@brief Returns a reference to the actions datum
		*@return reference to actions datum
		**/
		Datum& Actions() const;

		/**
		*@brief Helper function to create an action of type className and name instanceName within the entity
		*@param className - type of Action to create
		*@param instanceName - name of Action instance
		*@return pointer to action
		**/
		class Action* CreateAction(const std::string & className, const std::string & instanceName);
		
		static Vector<Signature> Signatures()
		{
			return Vector<Signature>
			{
				Signature("Name", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(Entity, mName)),
				Signature(sActions, FieaGameEngine::Datum::DatumType::TABLE)
			};
		}

		static const std::string sActions;
	protected:
		Entity(uint64_t);
		virtual Scope* Clone() override;
	private:
		std::string mName;
		Datum* mActions = nullptr;
	};
}