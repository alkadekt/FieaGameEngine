#pragma once
#include "EventPublisher.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Templated class for subscribing to and informing event subscribers of 
	/// events that deliver various message types.
	/// </summary>
	template <typename TMessage>
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher)

	public:
		/// <summary>
		/// Constructor that takes a typeargument message and 
		/// whether this event should be deleted following it's publishing.
		/// </summary>
		Event(const TMessage& message = TMessage());

		/// <summary>
		/// Constructor that accepts message as r value reference.
		/// </summary>
		Event(TMessage&& message);

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		Event(const Event& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		Event& operator=(const Event & other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		Event(Event && other) noexcept = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		Event& operator=(Event && other) noexcept = default;

		/// <summary>
		/// Default virtual destructor.
		/// </summary>
		virtual ~Event() = default;

		/// <summary>
		/// Adds given subscriber to this event class subscriber list.
		/// </summary>
		static void Subscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Removes given subscriber from this event class subscriber list.
		/// </summary>
		static void Unsubscribe(EventSubscriber& subscriber);

		/// <summary>
		/// Unsubscribes every subscriber from this event class.
		/// </summary>
		static void UnsubscribeAll();

		/// <summary>
		/// Returns the message held by this event.
		/// </summary>
		const TMessage& Message() const;

		/// <summary>
		/// ToString override for Event.
		/// </summary>
		virtual std::string ToString() const;

		/// <summary>
		/// Equals override for Event.
		/// </summary>
		virtual bool Equals(const RTTI* rhs) const;

		/// <summary>
		/// Adds pending subscribers and removes pending removes.
		/// </summary>
		static void HandlePendingSubscribers();

	private:
		inline static Vector<EventSubscriber*> _subscribers;
		inline static Vector<EventSubscriber*> _pendingRemoves;
		inline static Vector<EventSubscriber*> _pendingAdditions;
		TMessage _message;
	};
}

#include "Event.inl"