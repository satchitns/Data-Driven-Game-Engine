#pragma once
#include "RTTI.h"
#include "Vector.h"
#include <chrono>
#include <gsl/gsl>

namespace FieaGameEngine
{
	class EventSubscriber;

	/**
	*@brief Base class for Events, that sends out events of specific types (passed in as template argument) to subscribers.
	**/
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)
	public:
		virtual ~EventPublisher() = default;
		EventPublisher(const EventPublisher&) = default;
		EventPublisher(EventPublisher&&) = default;
		EventPublisher& operator=(const EventPublisher&) = default;
		EventPublisher& operator=(EventPublisher&&) = default;

		/**
		*@brief Set the queue time for the event, and the delay
		*@param currentTime time of enqueue
		*@param delay delay in milliseconds
		**/
		void SetTime(const std::chrono::high_resolution_clock::time_point& currentTime, std::chrono::milliseconds delay = std::chrono::milliseconds::zero());

		/**
		*@brief Get the queue time for the event
		*@return time of enqueue
		**/
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued() const;

		/**
		*@brief Get the queue time for the event
		*@return time of enqueue
		**/
		const std::chrono::milliseconds& Delay() const;

		/**
		*@brief Return the expiry state of the event given current time
		*@param current time
		*@return true if event has expired
		**/
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/**
		*@brief Call Notify for all subscribers of this type of event
		**/
		void Deliver() const;
	protected:
		EventPublisher(Vector<gsl::not_null<EventSubscriber*>>& subscribers);
	private:
		Vector<gsl::not_null<EventSubscriber*>>* mSubscribers = nullptr;
		std::chrono::milliseconds mDelay;
		std::chrono::high_resolution_clock::time_point mTimeEnqueued;
	};
}