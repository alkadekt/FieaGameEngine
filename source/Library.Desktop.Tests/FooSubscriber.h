#pragma once
#include "EventSubscriber.h"
#include "Foo.h"

namespace FieaGameEngine
{
	class FooSubscriber final : public EventSubscriber
	{
	public:
		FooSubscriber(std::int32_t data = 0);
		FooSubscriber(const FooSubscriber& other) = default;
		FooSubscriber& operator=(const FooSubscriber& other) = default;
		FooSubscriber(FooSubscriber&& other) noexcept = default;
		FooSubscriber& operator=(FooSubscriber&& other) noexcept = default;
		virtual ~FooSubscriber() = default;

		void Notify(const EventPublisher& publisher) override;

		std::uint32_t Data;
		bool WasNotified{ false };
	};
}
