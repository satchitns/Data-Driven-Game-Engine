#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListIf)

	ActionListIf::ActionListIf()
		:ActionList(TypeIdInstance())
	{
		mIfDatum = Find("If");
		mElseDatum = Find("Else");
		assert(mIfDatum != nullptr && mElseDatum != nullptr);
	}

	void ActionListIf::Update(WorldState & worldState)
	{
		if (mCondition != 0)
		{
			assert(mIfDatum != nullptr);
			Scope* ifScope = mIfDatum->Get<Scope*>();
			assert(ifScope != nullptr);
			assert(ifScope->Is(Action::TypeIdClass()));
			Action* ifAction = static_cast<Action*>(ifScope);
			ifAction->Update(worldState);
		}
		else
		{
			assert(mElseDatum != nullptr);
			Scope* elseScope = mElseDatum->Get<Scope*>();
			assert(elseScope != nullptr);
			assert(elseScope->Is(Action::TypeIdClass()));
			Action* elseAction = static_cast<Action*>(elseScope);
			elseAction->Update(worldState);
		}
	}

	void ActionListIf::SetIfCase(Action & action)
	{
		Adopt(&action, "If");
	}

	Datum & ActionListIf::GetIf() const
	{
		assert(mIfDatum != nullptr);
		return *mIfDatum;
	}

	Datum & ActionListIf::GetElse() const
	{
		assert(mElseDatum != nullptr);
		return *mElseDatum;
	}

	void ActionListIf::SetElseCase(Action & action)
	{
		Adopt(&action, "Else");
	}
}