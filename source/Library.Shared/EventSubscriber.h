#pragma once

namespace FieaGameEngine
{
	class EventPublisher;

	/// <summary>
	/// Abstract class to allow derived classes to accept message from event publishers.
	/// </summary>
	class EventSubscriber
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		EventSubscriber() = default;

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		EventSubscriber(const EventSubscriber& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		EventSubscriber& operator=(const EventSubscriber& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		EventSubscriber(EventSubscriber&& other) noexcept = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		EventSubscriber& operator=(EventSubscriber&& other) noexcept = default;

		/// <summary>
		/// Default virtual destructor.
		/// </summary>
		virtual ~EventSubscriber() = default;

		/// <summary>
		/// Notifies this subscriber of an event.
		/// </summary>
		virtual void Notify(const EventPublisher& publisher) = 0;
	};
}