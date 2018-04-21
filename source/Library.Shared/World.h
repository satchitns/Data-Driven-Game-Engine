#pragma once
#include "Attributed.h"
#include "GameClock.h"
#include "GameTime.h"
#include "WorldState.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	class Sector;
	class Reaction;

	/**
	*@brief Class that represents a World of the game - equivalent to a level. Possesses a name.
	**/
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		World(GameTime& time, GameClock& clock);
		~World() = default;
		World(const World& other) = delete;
		World& operator=(const World& other) = delete;
		World(World&& other) = default;
		World& operator=(World&& other) = default;

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
		**/
		void Update();


		/**
		*@brief Helper function to create a sector with name instanceName within the world
		*@param instanceName - name of Sector instance
		*@return pointer to sector
		**/
		Sector* CreateSector(const std::string& instanceName);

		/**
		*@brief Helper function to create a reaction with name instanceName within the world
		*@param className - type of Reaction to make
		*@param instanceName - name of Reaction instance
		*@return pointer to sector
		**/
		Reaction* CreateReaction(const std::string & className, const std::string& instanceName);

		/**
		*@brief Returns a reference to the sectors datum
		*@return reference to sectors datum
		**/
		Datum& Sectors() const;

		/**
		*@brief Returns a reference to the Reactions datum
		*@return reference to Reactions datum
		**/
		Datum& Reactions() const;

		/**
		*@brief Queues the given scope for deletion
		*@param scope to delete
		**/
		void QueueForDelete(Scope& scope);

		static Vector<Signature> Signatures()
		{
			return Vector<Signature>
			{
				Signature("Name", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(World, mName)),
				Signature(sSectors, FieaGameEngine::Datum::DatumType::TABLE),
				Signature(sReactions, FieaGameEngine::Datum::DatumType::TABLE)
			};
		}

		WorldState& GetState()
		{
			return mState;
		}

		EventQueue& GetEventQueue()
		{
			return mEventQueue;
		}

		static const std::string sSectors;
		static const std::string sReactions;
	private:
		Vector<Scope*> mDeleteQueue;
		std::string mName;
		WorldState mState;
		EventQueue mEventQueue;
		Datum* mSectors = nullptr;
		Datum* mReactions = nullptr;
		GameClock *mClock = nullptr;
		GameTime *mTime = nullptr;
	};
}