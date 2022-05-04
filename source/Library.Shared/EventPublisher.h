#pragma once
#include <chrono>
#include "RTTI.h"
#include "EventSubscriber.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Base class for events.
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI)

	protected:
		/// <summary>
		/// Constructor taking a list of EventSubscribers.
		/// </summary>
		EventPublisher(Vector<EventSubscriber*>& subscriberList);
		
	public:
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		EventPublisher(const EventPublisher& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		EventPublisher& operator=(const EventPublisher& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		EventPublisher(EventPublisher&& other) noexcept = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		EventPublisher& operator=(EventPublisher&& other) noexcept = default;

		/// <summary>
		/// Default virtual destructor.
		/// </summary>
		virtual ~EventPublisher() = default;

		/// <summary>
		/// Delivers this event to its subscribers.
		/// </summary>
		void Deliver();

		/// <summary>
		/// ToString override for EventPublisher.
		/// </summary>
		virtual std::string ToString() const;

		/// <summary>
		/// Equals override for EventPublisher.
		/// </summary>
		virtual bool Equals(const RTTI* rhs) const;

	private:
		Vector<EventSubscriber*>* _subscriberList;
	};
}
