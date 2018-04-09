#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"
#include<algorithm>

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> eventPublisher,
		GameTime & gameTime,
		std::chrono::milliseconds delay)
	{
		eventPublisher->SetTime(gameTime.CurrentTime(), delay);
		mQueue.push_back(eventPublisher);
	}

	void EventQueue::Send(std::shared_ptr<EventPublisher> eventPublisher)
	{
		eventPublisher->Deliver();
	}

	void EventQueue::Update(GameTime & gameTime)
	{
		auto & currentTime = gameTime.CurrentTime();
		auto partition = std::stable_partition(mQueue.begin(), mQueue.end(), [&currentTime](const auto & publisher) { return publisher->IsExpired(currentTime); });
		for (auto i = mQueue.begin(); i != partition; ++i)
		{
			(*i)->Deliver();
		}
		mQueue.erase(mQueue.begin(), partition);
	}

	void EventQueue::Clear()
	{
		mQueue.clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return mQueue.empty();
	}

	size_t EventQueue::Size() const
	{
		return mQueue.size();
	}
}
