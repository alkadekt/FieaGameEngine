#include "pch.h"
#include "FooSubscriber.h"
#include "Event.h"
#include "Foo.h"

namespace FieaGameEngine
{
	FooSubscriber::FooSubscriber(std::int32_t data) :
		Data(data)
	{
	}

	void FooSubscriber::Notify(const EventPublisher& publisher)
	{
		const Event<Foo>* e = publisher.As<Event<Foo>>();
		if (e != nullptr)
		{
			Data = e->Message().Data();
		}
		WasNotified = true;
	}
}