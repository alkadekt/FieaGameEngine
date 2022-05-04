#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher)

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscriberList) :
		_subscriberList(&subscriberList)
	{
	}
		
	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *_subscriberList)
		{
			subscriber->Notify(*this);
		}
	}

	std::string EventPublisher::ToString() const
	{
		return "EventPublisher";
	}

	bool EventPublisher::Equals(const RTTI* rhs) const
	{
		const auto rhsEventPublisher = rhs->As<EventPublisher>();
		if (rhsEventPublisher == nullptr)
		{
			return false;
		}

		return _subscriberList == rhsEventPublisher->_subscriberList;
	}
}