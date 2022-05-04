#pragma once
#include "EventSubscriber.h"

namespace FieaGameEngine
{
	class RemoveSubscriber final : public EventSubscriber
	{
	public:
		RemoveSubscriber(std::int32_t data = 0);
		RemoveSubscriber(const RemoveSubscriber& other) = default;
		RemoveSubscriber& operator=(const RemoveSubscriber & other) = default;
		RemoveSubscriber(RemoveSubscriber && other) noexcept = default;
		RemoveSubscriber& operator=(RemoveSubscriber && other) noexcept = default;
		virtual ~RemoveSubscriber() = default;

		void Notify(const EventPublisher & publisher) override;

		std::uint32_t Data;
		bool WasNotified{ false };
	};
}