#pragma once
#include <deque>
#include <chrono>
#include <memory>
#include <mutex>
#include "EventPublisher.h"
#include <vector>

namespace FieaGameEngine
{
	class GameTime;

	/**
	*@brief Event queue class that queues up events and calls Deliver on them in multiple threads when they expire.
	**/
	class EventQueue final
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;
		EventQueue(const EventQueue&) = delete;
		EventQueue(EventQueue&&) = default;
		EventQueue& operator=(const EventQueue&) = delete;
		EventQueue& operator=(EventQueue&&) = default;

		/**
		*@brief Enqueue the event with queue time and delay passed in
		*@param eventPublisher - shared pointer to an event
		*@param gameTime - time to set the event's queue time to
		*@param delay - milliseconds to set the event's delay to
		**/
		void Enqueue(std::shared_ptr<EventPublisher> eventPublisher, GameTime& gameTime,
			std::chrono::milliseconds delay = std::chrono::milliseconds::zero());

		/**
		*@brief Send the event immediately
		*@param eventPublisher - shared pointer to an event
		**/
		void Send(std::shared_ptr<EventPublisher> eventPublisher);

		/**
		*@brief Update the expired status of events and fire and dequeue them if they have expired
		*@param gameTime - current game time
		**/
		void Update(GameTime& gameTime);

		/**
		*@brief Clear the event queue
		**/
		void Clear();

		/**
		*@brief Return whether the event queue is empty
		*@return true if queue is empty
		**/
		bool IsEmpty() const;

		/**
		*@brief Return the size of the event queue
		*@return the size of the event queue
		**/
		size_t Size() const;
	private:
		std::vector<std::shared_ptr<EventPublisher>> mQueue;
		mutable std::mutex mMutex;
	};
}

