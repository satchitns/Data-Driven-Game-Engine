#include "pch.h"
#include<cassert>
#include "World.h"
#include "WorldState.h"
#include "Entity.h"
#include "Sector.h"
#include "Factory.h"


namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Sector)

		const std::string Sector::sEntities{ "Entities" };

	Sector::Sector()
		:Attributed(TypeIdInstance())
	{
		mEntities = Find(sEntities);
		assert(mEntities != nullptr);
	}

	const std::string & Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string & name)
	{
		mName = name;
	}

	World * Sector::GetWorld() const
	{
		Scope* parentScope = GetParent();
		assert(parentScope->Is(World::TypeIdClass()));
		World* parent = static_cast<World*>(parentScope);
		return parent;
	}

	void Sector::SetWorld(World & world)
	{
		world.Adopt(this, World::sSectors);
	}

	void Sector::Update(WorldState & state)
	{
		state.Sector = this;
		for (uint32_t i = 0; i < mEntities->Size(); ++i)
		{
			Scope* scope = mEntities->Get<Scope*>(i);
			assert(scope->Is(Entity::TypeIdClass()));
			Entity *entity = static_cast<Entity*>(scope);
			entity->Update(state);
		}
		state.Sector = nullptr;
	}

	Entity * Sector::CreateEntity(const std::string & className, const std::string & instanceName)
	{
		Entity* entity = Factory<Entity>::Create(className);
		if (entity == nullptr)
		{
			throw std::exception("Factory error - type not found");
		}
		entity->SetName(instanceName);
		entity->SetSector(*this);
		return entity;
	}

	Datum & Sector::Entities() const
	{
		assert(mEntities != nullptr);
		return *mEntities;
	}

	Scope * Sector::Clone()
	{
		return new Sector(*this);
	}
}