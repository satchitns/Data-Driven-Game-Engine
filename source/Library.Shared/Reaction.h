#pragma once
#include "ActionList.h"
#include "EventSubscriber.h"
namespace FieaGameEngine
{
	/**
	*@brief Bridge class between ActionList and EventSubscriber
	**/
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:
		virtual ~Reaction() = default;
		Reaction(const Reaction&) = default;
		Reaction(Reaction&&) = default;
		Reaction& operator=(const Reaction&) = default;
		Reaction& operator=(Reaction&&) = default;

		static Vector<Signature> Signatures()
		{
			return ActionList::Signatures();
		}

	protected:
		Reaction(uint64_t typeId);
	};
}