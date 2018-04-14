#include "pch.h"
#include "ActionEvent.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include<memory>
#include "WorldState.h"
#include "World.h"
#include<chrono>

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionEvent)

		ActionEvent::ActionEvent()
		:Action(TypeIdInstance())
	{

	}

	void ActionEvent::Update(WorldState & state)
	{
		EventMessageAttributed message;
		message.SetSubtype(mSubtype);
		message.SetWorld(*state.World);
		auto attributes = AuxiliaryAttributes();
		for (auto const & attribute : attributes)
		{
			Datum& datum = attribute->second;
			std::string& name = attribute->first;
			if (datum.Type() != Datum::DatumType::TABLE)
			{
				message.Append(name) = datum;
			}
			else
			{
				for (uint32_t i = 0; i < datum.Size(); ++i)
				{
					message.Adopt(new Scope(datum[i]), name);
				}
			}
		}
		std::shared_ptr<Event<EventMessageAttributed>> eventPtr = std::make_shared<Event<EventMessageAttributed>>(message);
		state.World->GetEventQueue().Enqueue(eventPtr, *state.GetGameTime(), std::chrono::milliseconds((int)mDelay*1000));
	}

	Scope * ActionEvent::Clone()
	{
		return new ActionEvent(*this);
	}
}