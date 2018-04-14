#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed)

	EventMessageAttributed::EventMessageAttributed()
		:Attributed(TypeIdInstance())
	{

	}

	World * EventMessageAttributed::GetWorld() const
	{
		return mWorld;
	}

	const std::string & EventMessageAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void EventMessageAttributed::SetWorld(World & world)
	{
		mWorld = &world;
	}

	void EventMessageAttributed::SetSubtype(const std::string & type)
	{
		mSubtype = type;
	}

	Scope * EventMessageAttributed::Clone()
	{
		return new EventMessageAttributed(*this);
	}
}