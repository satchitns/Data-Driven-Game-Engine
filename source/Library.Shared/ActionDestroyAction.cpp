#include "pch.h"
#include <cassert>
#include "Factory.h"
#include "Entity.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction()
		:Action(TypeIdInstance())
	{
		AddExternalAttribute("InstanceName", mInstanceName);
	}

	ActionDestroyAction::ActionDestroyAction(ActionDestroyAction&& other)
		: Action(std::move(other)), mInstanceName(std::move(other.mInstanceName))
	{
		UpdateExternalAttribute("InstanceName", mInstanceName);
	}

	ActionDestroyAction & ActionDestroyAction::operator=(ActionDestroyAction &&other)
	{
		if (this != &other)
		{
			Action::operator=(std::move(other));
			mInstanceName = std::move(other.mInstanceName);
			UpdateExternalAttribute("InstanceName", mInstanceName);
		}
		return *this;
	}

	void ActionDestroyAction::Update(WorldState & state)
	{
		Scope * currentScope = GetParent(), *resultScope = nullptr;
		while (currentScope != nullptr)
		{
			Datum* result = currentScope->Find("Actions");
			assert(result != nullptr);
			for (uint32_t i = 0; i < result->Size(); ++i)
			{
				resultScope = result->Get<Scope*>(i);
				Datum* nameDatum = (resultScope)->Find("Name");
				if (nameDatum != nullptr && nameDatum->Get<std::string>() == mInstanceName)
				{
					state.World->QueueForDelete(*resultScope);
					return;
				}
			}
			currentScope = currentScope->GetParent();
		}
	}
}