#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"
#include "World.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed)

	ReactionAttributed::ReactionAttributed()
		:Reaction(TypeIdInstance())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(const ReactionAttributed& other)
		:Reaction(other), mSubtype(other.mSubtype)
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed::ReactionAttributed(ReactionAttributed&& other)
		:Reaction(std::move(other)), mSubtype(std::move(other.mSubtype))
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	ReactionAttributed& ReactionAttributed::operator=(const ReactionAttributed& other)
	{
		if (this != &other)
		{
			Reaction::operator=(other);
			mSubtype = other.mSubtype;
		}
		return *this;
	}
	
	ReactionAttributed& ReactionAttributed::operator=(ReactionAttributed&& other)
	{
		if (this != &other)
		{
			Reaction::operator=(std::move(other));
			mSubtype = std::move(other.mSubtype);
		}
		return *this;
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessageAttributed>::Unsubscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher & publisher)
	{
		if (publisher.Is(Event<EventMessageAttributed>::TypeIdClass()))
		{
			const Event<EventMessageAttributed>* event = static_cast<const Event<EventMessageAttributed>*>(&publisher);
			const EventMessageAttributed& message = event->Message();
			if (message.GetSubtype() == mSubtype)
			{
				auto attributes = message.AuxiliaryAttributes();
				for (auto const & attribute : attributes)
				{
					Datum& datum = attribute->second;
					std::string& name = attribute->first;
					if (datum.Type() != Datum::DatumType::TABLE)
					{
						Append(name) = datum;
					}
					else
					{
						for (uint32_t i = 0; i < datum.Size(); ++i)
						{
							Adopt(new Scope(datum[i]), name);
						}
					}
				}
				Update(message.GetWorld()->GetState());
			}
		}
	}

	const std::string & ReactionAttributed::GetSubtype() const
	{
		return mSubtype;
	}

	void ReactionAttributed::SetSubtype(const std::string & type)
	{
		mSubtype = type;
	}

	Scope * ReactionAttributed::Clone()
	{
		return new ReactionAttributed(*this);
	}
}
