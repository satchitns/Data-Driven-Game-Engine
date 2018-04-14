#pragma once
#include "EventPublisher.h"
#include "Vector.h"
#include "gsl/gsl"

namespace FieaGameEngine
{
	/**
	*@brief Event class that sends out events of specific types (passed in as template argument) to subscribers.
	**/
	template<typename T>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event<T>, EventPublisher)
	public:
		Event(T& message);
		Event(const Event&) = default;
		Event(Event&&) = default;
		Event& operator=(const Event&) = default;
		Event& operator=(Event&&) = default;

		/**
		*@brief static method to subscribe a subscriber to this type of event
		*@param subscriber reference
		**/
		static void Subscribe(EventSubscriber& subscriber);

		/**
		*@brief static method to unsubscribe a subscriber from this type of event
		*@param subscriber reference
		**/
		static void Unsubscribe(EventSubscriber& subscriber);

		/**
		*@brief static method to unsubscribe all subscribers from this type of event
		**/
		static void UnsubscribeAll();

		/**
		*@brief Method to get the payload of the event
		*@return reference to a templated type object
		**/
		const T& Message() const;

		static Vector<gsl::not_null<EventSubscriber*>> sSubscribers;
	private:
		T mMessage;
	};
}

#include "Event.inl"