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
		AddExternalAttribute("Name", mName);
		mEntities = &CreateNestedScope(Sector::sEntities);
	}

	Sector::Sector(const Sector& other)
		:Attributed(other), mName(other.mName)
	{
		UpdateExternalAttribute("Name", mName);
		mEntities = Find(Sector::sEntities);
	}

	Sector::Sector(Sector&& other)
		: Attributed(other), mName(std::move(other.mName))
	{
		UpdateExternalAttribute("Name", mName);
		mEntities = other.mEntities;
	}

	Sector& Sector::operator=(const Sector& other)
	{
		if (this != &other)
		{
			Attributed::operator=(other);
			mName = other.mName;
			mEntities = Find(Sector::sEntities);
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	Sector& Sector::operator=(Sector&& other)
	{
		if (this != &other)
		{
			Attributed::operator=(std::move(other));
			mName = std::move(other.mName);
			mEntities = other.mEntities;
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
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
		assert(parentScope->Is(World::TypeName()));
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
			assert(scope->Is(Entity::TypeName()));
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
		return *mEntities;
	}
}