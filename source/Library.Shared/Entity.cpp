#include "pch.h"
#include <cassert>
#include "Sector.h"
#include "WorldState.h"
#include "Entity.h"
#include "Action.h"
#include "Factory.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Entity)
		
	const std::string Entity::sActions{ "Actions" };

	Entity::Entity()
		:Entity(TypeIdInstance())
	{
	}

	Datum & Entity::Actions() const
	{
		assert(mActions != nullptr);
		return *mActions;
	}

	Entity::Entity(uint64_t id)
		: Attributed(id)
	{
		mActions = Find(sActions);
		assert(mActions != nullptr);
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
		assert(parentScope->Is(Sector::TypeIdClass()));
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
		for (uint32_t i = 0; i < mActions->Size(); ++i)
		{
			Scope* scope = mActions->Get<Scope*>(i);
			assert(scope->Is(Action::TypeIdClass()));
			Action *action = static_cast<Action*>(scope);
			action->Update(state);
		}
		state.Entity = nullptr;
	}

	Action* Entity::CreateAction(const std::string & className, const std::string & instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		if (action == nullptr)
		{
			throw std::exception("Action factory error - type not found");
		}
		action->SetName(instanceName);
		action->SetEntity(*this);
		return action;
	}
}