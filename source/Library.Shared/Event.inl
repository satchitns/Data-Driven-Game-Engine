#pragma once


namespace FieaGameEngine
{

	template<typename T>
	RTTI_DEFINITIONS(Event<T>)

	template<typename T>
	Vector<gsl::not_null<EventSubscriber*>> Event<T>::sSubscribers;

	template<typename T>
	std::mutex Event<T>::sMutex;

	template<typename T>
	Event<T>::Event(T& message)
		:EventPublisher(sSubscribers, sMutex), mMessage(message)
	{
	}

	template<typename T>
	void Event<T>::Subscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sMutex);
		sSubscribers.PushBack(&subscriber);
	}

	template<typename T>
	void Event<T>::Unsubscribe(EventSubscriber& subscriber)
	{
		std::lock_guard<std::mutex> lock(sMutex);
		sSubscribers.Remove(&subscriber);
	}

	template<typename T>
	void  Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> lock(sMutex);
		sSubscribers.Clear();
	}

	template<typename T>
	const T&  Event<T>::Message() const
	{
		return mMessage;
	}
}