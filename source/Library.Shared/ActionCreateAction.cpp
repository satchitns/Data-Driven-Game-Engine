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