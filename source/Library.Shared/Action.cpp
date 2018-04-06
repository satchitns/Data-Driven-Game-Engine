#include "pch.h"
#include "Entity.h"
#include "ActionList.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Action::Action(uint64_t id)
		: Attributed(id)
	{
		AddExternalAttribute("Name", mName);
	}

	Action::Action(Action&& other)
		: Attributed(other), mName(std::move(other.mName))
	{
		UpdateExternalAttribute("Name", mName);
	}

	Action& Action::operator=(Action&& other)
	{
		if (this != &other)
		{
			Attributed::operator=(std::move(other));
			mName = std::move(other.mName);
			UpdateExternalAttribute("Name", mName);
		}
		return *this;
	}

	const std::string & Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string & name)
	{
		mName = name;
	}

	void Action::SetEntity(Entity & entity)
	{
		entity.Adopt(this, Entity::sActions);
	}

	void Action::SetParent(ActionList& list)
	{
		list.Adopt(this, ActionList::sActions);
	}
}