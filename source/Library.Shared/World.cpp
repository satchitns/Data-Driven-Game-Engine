#include "pch.h"
#include<cassert>
#include "Sector.h"
#include "WorldState.h"
#include "World.h"
#include "Reaction.h"
#include "Factory.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	const std::string World::sSectors{"Sectors"};

	const std::string World::sReactions{ "Reactions" };

	World::World(GameTime& time, GameClock& clock)
		:Attributed(TypeIdClass())
	{
		mTime = &time;
		mClock = &clock;
		mSectors = Find(sSectors);
		assert(mSectors != nullptr);
		mReactions = Find(sReactions);
		assert(mReactions != nullptr);
		mState.World = this;
		mState.SetGameTime(*mTime);
	}

	Reaction * World::CreateReaction(const std::string & className, const std::string & instanceName)
	{
		Reaction* reaction = Factory<Reaction>::Create(className);
		if (reaction == nullptr)
		{
			throw std::exception("Action factory error - type not found");
		}
		reaction->SetName(instanceName);
		Adopt(reaction, sReactions);
		return reaction;
	}

	Datum & World::Sectors() const
	{
		return *mSectors;
	}

	Datum & World::Reactions() const
	{
		return *mReactions;
	}

	void World::QueueForDelete(Scope & scope)
	{
		mDeleteQueue.PushBack(&scope);
	}

	const std::string & World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string & name)
	{
		mName = name;
	}

	void World::Update()
	{
		if (mDeleteQueue.Size() > 0)
		{
			for (auto &scope : mDeleteQueue)
			{
				delete scope;
			}
			mDeleteQueue.Clear();
		}

		assert(mTime != nullptr);
		assert(mClock != nullptr);
		mClock->UpdateGameTime(*mTime);
		mEventQueue.Update(*mTime);
		for (uint32_t i = 0; i < mSectors->Size(); ++i)
		{
			Scope* scope = mSectors->Get<Scope*>(i);
			assert(scope->Is(Sector::TypeIdClass()));
			Sector *sector = static_cast<Sector*>(scope);
			sector->Update(mState);
		}
	}

	Sector * World::CreateSector(const std::string & instanceName)
	{
		Sector* sector = new Sector();
		sector->SetName(instanceName);
		sector->SetWorld(*this);
		return sector;
	}
}