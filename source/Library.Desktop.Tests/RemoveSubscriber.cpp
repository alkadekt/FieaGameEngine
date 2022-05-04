#include "pch.h"
#include "RemoveSubscriber.h"
#include "Event.h"
#include "Foo.h"

namespace FieaGameEngine
{
	RemoveSubscriber::RemoveSubscriber(std::int32_t data) :
		Data(data)
	{
	}

	void RemoveSubscriber::Notify(const EventPublisher& publisher)
	{
		const Event<Foo>* e = publisher.As<Event<Foo>>();
		if (e != nullptr)
		{
			Data = e->Message().Data();
			Event<Foo>::Unsubscribe(*this);
		}
		WasNotified = true;
	}
}