#include "pch.h"
#include "EventSubscriber.h"
#include "EventPublisher.h"
#include <future>
#include <vector>

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
		Vector<gsl::not_null<EventSubscriber*>> copyVector;
		std::vector<std::future<void>> futures;
		{
			assert(mMutex != nullptr);
			std::lock_guard<std::mutex> lock(*mMutex); //the subscriber list is locked while notify is called
			copyVector = *mSubscribers;
		}
		for (auto it = copyVector.begin(); it != copyVector.end(); ++it)
		{
			futures.emplace_back(std::async(std::launch::async, [it, this]
			{
				(*it)->Notify(*this);
			}));
		}
		//unlock the subscriber list (by leaving the scope) while waiting for the threads to complete
		for (auto & future : futures)
		{
			future.get();
		}
	}

	EventPublisher::EventPublisher(Vector<gsl::not_null<EventSubscriber*>>& subscribers, std::mutex& mutex)
		: mSubscribers(&subscribers), mMutex(&mutex)
	{

	}
}