#include "pch.h"
#include"EventQueue.h"

namespace FieaGameEngine
{
	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publisher, GameTime time, std::chrono::milliseconds delay)
	{
		std::pair<std::shared_ptr<EventPublisher>, EventInfo> entry = 
			std::make_pair(publisher, EventInfo{ time.CurrentTime(), delay });
		_events.PushBack(entry);
	}

	void EventQueue::Send(std::shared_ptr<EventPublisher> publisher)
	{
		publisher->Deliver();
	}

	void EventQueue::Update(GameTime time)
	{
		auto expression = [&, this](std::pair<std::shared_ptr<EventPublisher>, EventInfo> pair)
		{ return IsExpired(time.CurrentTime(), pair.second); };

		auto it = std::partition(_events.begin(), _events.end(), expression);

		for (auto iterator = _events.begin(); iterator != it; ++iterator)
		{
			Send((*iterator).first);
		}
		_events.Remove(_events.begin(), it);
	}

	bool EventQueue::IsExpired(std::chrono::high_resolution_clock::time_point currentTime, const EventInfo& info) const
	{
		return currentTime - info.TimeEnqueued >= info.Delay;
	}
	
	void EventQueue::Clear()
	{
		_events.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return _events.IsEmpty();
	}

	std::size_t EventQueue::Size() const
	{
		return _events.Size();
	}
}