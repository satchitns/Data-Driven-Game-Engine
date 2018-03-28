#include "pch.h"
#include<cassert>
#include "Sector.h"
#include "WorldState.h"
#include "World.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(World)

	const std::string World::sSectors{"Sectors"};
		
	World::World(GameTime& time, GameClock& clock)
		:Attributed(TypeIdInstance())
	{
		AddExternalAttribute("Name", mName);
		mTime = &time;
		mClock = &clock;
		mSectors = &CreateNestedScope(World::sSectors);
	}

	World::World(const World& other)
		:Attributed(other), mName(other.mName), mTime(other.mTime), mClock(other.mClock)
	{
		UpdateExternalAttribute("Name", mName);
		mSectors = Find(World::sSectors);
	}

	World::World(World&& other)
		: Attributed(other), mName(std::move(other.mName)), mTime(std::move(other.mTime)), mClock(std::move(other.mClock))
	{
		UpdateExternalAttribute("Name", mName);
		mSectors = other.mSectors;
	}

	World& World::operator=(const World& other)
	{
		if (this != &other)
		{
			Attributed::operator=(other);
			mName = other.mName;
			mTime = other.mTime;
			mClock = other.mClock;
			mSectors = Find(World::sSectors);
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	World& World::operator=(World&& other)
	{
		if (this != &other)
		{
			Attributed::operator=(std::move(other));
			mName = std::move(other.mName);
			mTime = std::move(other.mTime);
			mClock = std::move(other.mClock);
			mSectors = other.mSectors;
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	Datum & World::Sectors() const
	{
		return *mSectors;
	}

	const std::string & World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string & name)
	{
		mName = name;
	}

	void World::Update(WorldState & state)
	{
		state.World = this;
		assert(mTime != nullptr);
		assert(mClock != nullptr);
		mClock->UpdateGameTime(*mTime);
		state.SetGameTime(*mTime);
		for (uint32_t i = 0; i < mSectors->Size(); ++i)
		{
			Scope* scope = mSectors->Get<Scope*>(i);
			assert(scope->Is(Sector::TypeName()));
			Sector *sector = static_cast<Sector*>(scope);
			sector->Update(state);
		}
		state.World = nullptr;
	}

	Sector * World::CreateSector(const std::string & instanceName)
	{
		Sector* sector = new Sector();
		sector->SetName(instanceName);
		sector->SetWorld(*this);
		return sector;
	}
}