#pragma once
#include "Attributed.h"

namespace FieaGameEngine
{
	class Entity;
	class WorldState;
	class World;

	/**
	*@brief Class that represents an Sector of the game - a playable area/a unit of containment. Possesses a name.
	**/
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		Sector();
		~Sector() = default;
		Sector(Sector&& other);
		Sector& operator=(Sector&& other);

		/**
		*@brief Returns the name of the sector
		*@return name of the sector
		**/
		const std::string& Name() const;

		/**
		*@brief Sets the name of the sector
		*@param name - name of the sector
		**/
		void SetName(const std::string& name);

		/**
		*@brief Returns the world the sector belongs to
		*@return pointer to world
		**/
		World* GetWorld() const;

		/**
		*@brief Sets the world the entity belongs to
		*@param Reference to world
		**/
		void SetWorld(World& world);

		/**
		*@brief Update function for the sector that runs every frame - updates all contained entities
		*@param World State object containing time and pointers for context
		**/
		void Update(WorldState& state);

		/**
		*@brief Helper function to create an entity of type className and name instanceName within the sector
		*@param className - type of Entity to create
		*@param instanceName - name of Entity instance
		*@return pointer to entity
		**/
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/**
		*@brief Returns a reference to the entities datum
		*@return reference to entities datum
		**/
		Datum& Entities() const;

		static const std::string sEntities;
	private:
		std::string mName;
		Datum* mEntities = nullptr;
	};
}