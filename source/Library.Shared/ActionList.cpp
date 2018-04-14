#include "pch.h"
#include <cassert>
#include "ActionList.h"
#include "Factory.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	const std::string ActionList::sActions{ "Actions" };

	ActionList::ActionList()
		:ActionList(TypeIdInstance())
	{
	}

	ActionList::ActionList(uint64_t id)
		: Action(id)
	{
		mActions = Find(sActions);
		assert(mActions != nullptr);
	}

	void ActionList::Update(WorldState & worldState)
	{
		for (uint32_t i = 0; i < mActions->Size(); ++i)
		{
			Scope* scope = mActions->Get<Scope*>(i);
			assert(scope->Is(Action::TypeIdClass()));
			Action *action = static_cast<Action*>(scope);
			action->Update(worldState);
		}
	}

	Datum & ActionList::Actions() const
	{
		assert(mActions != nullptr);
		return *mActions;
	}

	Action* ActionList::CreateAction(const std::string & className, const std::string & instanceName)
	{
		Action* action = Factory<Action>::Create(className);
		if (action == nullptr)
		{
			throw std::exception("Action factory error - type not found");
		}
		action->SetName(instanceName);
		action->SetParent(*this);
		return action;
	}

	Scope * ActionList::Clone()
	{
		return new ActionList(*this);
	}
}