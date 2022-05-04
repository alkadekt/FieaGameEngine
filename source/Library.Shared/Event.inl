#include "Event.h"

namespace FieaGameEngine
{
	template<typename TMessage>
	RTTI_DEFINITIONS(Event<TMessage>)

	template <typename TMessage>
	Event<TMessage>::Event(const TMessage& message) :
		EventPublisher(_subscribers), _message(message)
	{
	}

	template <typename TMessage>
	Event<TMessage>::Event(TMessage&& message) :
		EventPublisher(_subscribers), _message(std::move(message))
	{
	}

	template <typename TMessage>
	void Event<TMessage>::Subscribe(EventSubscriber& subscriber)
	{
		if (!_pendingAdditions.Contains(&subscriber))
		{
			_pendingAdditions.PushBack(&subscriber);
		}
		//if (!_subscribers.Contains(&subscriber))
		//{
		//	_subscribers.PushBack(&subscriber);
		//}
	}

	template <typename TMessage>
	void Event<TMessage>::Unsubscribe(EventSubscriber& subscriber)
	{
		if (!_pendingRemoves.Contains(&subscriber))
		{
			_pendingRemoves.PushBack(&subscriber);
		}
		//if (_subscribers.Remove(&subscriber))
		//{
		//	_subscribers.ShrinkToFit();
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}
	}

	template <typename TMessage>
	void Event<TMessage>::UnsubscribeAll()
	{
		_subscribers.Clear();
		_subscribers.Resize(0);
	}

	template <typename TMessage>
	const TMessage& Event<TMessage>::Message() const
	{
		return _message;
	}

	template <typename TMessage>
	std::string Event<TMessage>::ToString() const
	{
		return "Event";
	}

	template <typename TMessage>
	bool Event<TMessage>::Equals(const RTTI* rhs) const
	{
		const auto rhsEvent = rhs->As<Event<TMessage>>();
		if (rhsEvent == nullptr)
		{
			return false;
		}

		return _message == rhsEvent->_message;
	}

	template <typename TMessage>
	void Event<TMessage>::HandlePendingSubscribers()
	{
		for (auto& subscriber : _pendingRemoves)
		{
			_subscribers.Remove(subscriber);
		}

		for (auto& subscriber : _pendingAdditions)
		{
			_subscribers.PushBack(subscriber);
		}

		_pendingRemoves.Clear();
		_pendingAdditions.Clear();
	}
}