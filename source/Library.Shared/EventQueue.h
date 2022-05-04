#pragma once
#include <chrono>
#include "GameTime.h"
#include "EventPublisher.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Holds a queue of events. Publishes expired events.
	/// </summary>
	class EventQueue final
	{
	private:
		struct EventInfo final
		{
			std::chrono::high_resolution_clock::time_point TimeEnqueued;
			std::chrono::milliseconds Delay;
		};

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		EventQueue() = default;

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		EventQueue(const EventQueue& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		EventQueue& operator=(const EventQueue& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		EventQueue(EventQueue&& other) noexcept = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		EventQueue& operator=(EventQueue&& other) noexcept = default;

		/// <summary>
		/// Default virtual destructor.
		/// </summary>
		~EventQueue() = default;

		/// <summary>
		/// Add this event to the queue.
		/// </summary>
		void Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime time, 
			std::chrono::milliseconds delay = std::chrono::milliseconds(0));

		/// <summary>
		/// Send given event immediately.
		/// </summary>
		void Send(std::shared_ptr<EventPublisher> publisher);

		/// <summary>
		/// Called during game update, sends expired events.
		/// </summary>
		void Update(GameTime time);

		/// <summary>
		/// Returns whether given event info is expired based on current time.
		/// </summary>
		bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime, const EventInfo& info) const;

		/// <summary>
		/// Clears queue.
		/// </summary>
		void Clear();

		/// <summary>
		/// If queue is empty.
		/// </summary>
		bool IsEmpty() const;

		/// <summary>
		/// Size of the queue.
		/// </summary>
		std::size_t Size() const;

	private:
		Vector<std::pair<std::shared_ptr<EventPublisher>, EventInfo>> _events;
	};
}