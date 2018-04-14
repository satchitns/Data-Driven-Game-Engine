#include "pch.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)
		void EventPublisher::SetTime(const std::chrono::high_resolution_clock::time_point & currentTime, std::chrono::milliseconds delay)
	{
		mTimeEnqueued = currentTime;
		mDelay = delay;
	}

	const std::chrono::high_resolution_clock::time_point & EventPublisher::TimeEnqueued() const
	{
		return mTimeEnqueued;
	}

	const std::chrono::milliseconds & EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const std::chrono::high_resolution_clock::time_point & currentTime) const
	{
		std::chrono::high_resolution_clock::time_point expireTime = mTimeEnqueued;
		expireTime += mDelay;
		if (currentTime >= expireTime)
		{
			return true;
		}
		return false;
	}

	void EventPublisher::Deliver() const
	{
		assert(mSubscribers != nullptr);
		for (auto& subscriber : *mSubscribers)
		{
			subscriber->Notify(*this);
		}
	}

	EventPublisher::EventPublisher(Vector<gsl::not_null<EventSubscriber*>>& subscribers)
	{
		mSubscribers = &subscribers;
	}
}