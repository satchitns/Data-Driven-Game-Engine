#pragma once
namespace FieaGameEngine
{
	/**
	*@brief Class that represents the current world state, with pointers for context and a pointer to the game time instance being used.
	**/
	class WorldState final
	{
	public:

		class World* World;
		class Sector* Sector;
		class Entity* Entity;
		class Action* Action;

		/**
		*@brief Returns a pointer to the game time
		*@return GameTime pointer
		**/
		class GameTime* GetGameTime() const;

		/**
		*@brief Sets the game time
		*@param GameTime reference
		**/
		void SetGameTime(GameTime& gameTime);
	private:
		class GameTime* mGameTime = nullptr;
	};
}