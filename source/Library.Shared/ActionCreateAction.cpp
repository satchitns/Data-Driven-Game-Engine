#include "pch.h"
#include <cassert>
#include "Factory.h"
#include "Entity.h"
#include "ActionCreateAction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction()
		:Action(TypeIdInstance())
	{
		AddExternalAttribute("Prototype", mPrototype);
		AddExternalAttribute("InstanceName", mInstanceName);
	}

	ActionCreateAction::ActionCreateAction(ActionCreateAction&& other)
		:Action(std::move(other)), mPrototype(std::move(other.mPrototype)), mInstanceName(std::move(other.mInstanceName))
	{
		UpdateExternalAttribute("Prototype", mPrototype);
		UpdateExternalAttribute("InstanceName", mInstanceName);
	}

	ActionCreateAction & ActionCreateAction::operator=(ActionCreateAction &&other)
	{
		if (this != &other)
		{
			Action::operator=(std::move(other));
			mPrototype = std::move(other.mPrototype);
			mInstanceName = std::move(other.mInstanceName);
			UpdateExternalAttribute("Prototype", mPrototype);
			UpdateExternalAttribute("InstanceName", mInstanceName);
		}
		return *this;
	}

	void ActionCreateAction::Update(WorldState & state)
	{
		state;
		Scope * parent = GetParent();
		assert(parent != nullptr);
		Action* action = Factory<Action>::Create(mPrototype);
		assert(action != nullptr);
		action->SetName(mInstanceName);
		parent->Adopt(action, Entity::sActions);
	}
}