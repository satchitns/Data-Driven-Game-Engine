#include "pch.h"
#include <cassert>
#include "Sector.h"
#include "WorldState.h"
#include "Entity.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)

	Entity::Entity()
		:Attributed(TypeIdInstance())
	{
		AddExternalAttribute("Name", mName);
	}

	Entity::Entity(uint64_t id)
		: Attributed(id)
	{
		AddExternalAttribute("Name", mName);
	}

	Entity::Entity(const Entity& other)
		:Attributed(other), mName(other.mName)
	{
		UpdateExternalAttribute("Name", mName);
	}

	Entity::Entity(Entity&& other)
		: Attributed(other), mName(std::move(other.mName))
	{
		UpdateExternalAttribute("Name", mName);
	}

	Entity& Entity::operator=(const Entity& other)
	{
		if (this != &other)
		{
			Attributed::operator=(other);
			mName = other.mName;
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	Entity& Entity::operator=(Entity&& other)
	{
		if (this != &other)
		{
			Attributed::operator=(std::move(other));
			mName = std::move(other.mName);
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	const std::string & Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string & name)
	{
		mName = name;
	}

	Sector * Entity::GetSector() const
	{
		Scope* parentScope = GetParent();
		assert(parentScope->Is(Sector::TypeName()));
		Sector* parent = static_cast<Sector*>(parentScope);
		return parent;
	}

	void Entity::SetSector(Sector & sector)
	{	
		sector.Adopt(this, Sector::sEntities);
	}
	void Entity::Update(WorldState & state)
	{
		state.Entity = this;
		//TODO : actions
		state.Entity = nullptr;
	}
}