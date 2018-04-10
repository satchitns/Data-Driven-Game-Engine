#pragma once


namespace FieaGameEngine
{

	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Vector<gsl::not_null<EventSubscriber*>> Event<T>::sSubscribers;

	template<typename T>
	Event<T>::Event(T& message)
		:EventPublisher(sSubscribers)
	{
		mMessage = &message;
	}

	template<typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		sSubscribers.PushBack(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		sSubscribers.Remove(&subscriber);
	}

	template<typename T>
	void  Event<T>::UnsubscribeAll()
	{
		sSubscribers.Clear();
	}

	template<typename T>
	T&  Event<T>::Message()
	{
		assert(mMessage != nullptr);
		return *mMessage;
	}
}