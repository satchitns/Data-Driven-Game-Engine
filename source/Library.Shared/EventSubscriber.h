#pragma once

namespace FieaGameEngine
{
	class EventPublisher;

	/**
	*@brief abstract base class for a subscriber to an event
	**/
	class EventSubscriber abstract
	{
	public:

		/**
		*@brief pure virtual Notify method that takes in a reference to an event
		*@param event - const reference to an event
		**/
		virtual void Notify(const EventPublisher& event) = 0;
	};
}