#include "pch.h"
#include "EventQueue.h"
#include "GameTime.h"
#include <algorithm>
#include <future>
#include <thread>

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> eventPublisher,
		GameTime & gameTime,
		std::chrono::milliseconds delay)
	{
		eventPublisher->SetTime(gameTime.CurrentTime(), delay);
		std::lock_guard<std::mutex> lock(mMutex);
		mQueue.push_back(eventPublisher);
	}

	void EventQueue::Send(std::shared_ptr<EventPublisher> eventPublisher)
	{
		eventPublisher->Deliver();
	}

	void EventQueue::Update(GameTime & gameTime)
	{
		auto & currentTime = gameTime.CurrentTime();
		std::vector<std::shared_ptr<EventPublisher>>::iterator partition;
		std::vector<std::shared_ptr<EventPublisher>> deployQueue;
		{
			std::lock_guard<std::mutex> lock(mMutex); //locking this block before mutating event queue
			partition = std::stable_partition(mQueue.begin(), mQueue.end(), [&currentTime](const auto & publisher) { return !publisher->IsExpired(currentTime); });
			std::move(partition, mQueue.end(), std::back_inserter(deployQueue));
			mQueue.erase(partition, mQueue.end());
		}
		std::vector<std::future<void>> futures;

		for (auto it = deployQueue.begin(); it != deployQueue.end(); ++it)
		{
			futures.emplace_back(std::async(std::launch::async, [it]
			{
				(*it)->Deliver();
			}));
		}

		for (auto & future : futures)
		{
			future.get();
		}
		deployQueue.clear();
	}

	void EventQueue::Clear()
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mQueue.clear();
	}

	bool EventQueue::IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mQueue.empty();
	}

	size_t EventQueue::Size() const
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mQueue.size();
	}
}
