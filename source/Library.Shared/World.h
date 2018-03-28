#pragma once
#include "Attributed.h"
#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class Sector;
	class WorldState;

	/**
	*@brief Class that represents a World of the game - equivalent to a level. Possesses a name.
	**/
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		World(GameTime& time, GameClock& clock);
		~World() = default;
		World(const World& other);
		World(World&& other);
		World& operator=(const World& other);
		World& operator=(World&& other);

		/**
		*@brief Returns the name of the world
		*@return name of the world
		**/
		const std::string& Name() const;

		/**
		*@brief Sets the name of the world
		*@param name - name of the world
		**/
		void SetName(const std::string& name);

		/**
		*@brief Update function for the sector that runs every frame - updates all contained sectors
		*@param World State object containing time and pointers for context
		**/
		void Update(WorldState& state);


		/**
		*@brief Helper function to create a sector with name instanceName within the world
		*@param instanceName - name of Sector instance
		*@return pointer to sector
		**/
		Sector* CreateSector(const std::string& instanceName);

		/**
		*@brief Returns a reference to the sectors datum
		*@return reference to sectors datum
		**/
		Datum& Sectors() const;

		static const std::string sSectors;
	private:
		std::string mName;
		Datum* mSectors = nullptr;
		GameClock *mClock = nullptr;
		GameTime *mTime = nullptr;
	};
}