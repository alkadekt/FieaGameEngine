#include "pch.h"
#include "CppUnitTest.h"
#include <chrono>
#include "Event.h"
#include "EventQueue.h"
#include "GameTime.h"
#include "Foo.h"
#include "FooSubscriber.h"
#include "RemoveSubscriber.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				//Assert::Fail(L"There was a memory leak.");
			}
#endif
		}

		TEST_METHOD(EventRTTI)
		{
			Assert::AreEqual("EventPublisher"s, EventPublisher::TypeName());
			Assert::AreEqual("Event"s, Event<Foo>::TypeName());

			Event<Foo> event;
			EventPublisher* publisher = &event;
			Assert::IsTrue(publisher->Is(Event<Foo>::TypeName()));
			Assert::IsTrue(publisher->Is(Event<Foo>::TypeIdClass()));
			Assert::AreEqual("Event"s, publisher->ToString());
			EventPublisher* publisherCopy = &event;
			Assert::IsTrue(publisherCopy->Equals(publisher));
			Foo foo;
			Foo* fooPtr = &foo;
			Assert::IsFalse(publisherCopy->Equals(fooPtr));

			Event<Foo>* eventPtr = publisher->As<Event<Foo>>();
			Assert::IsNotNull(eventPtr);
			
			const Event<Foo>* constEventPtr = publisher->As<Event<Foo>>();
			Assert::IsNotNull(constEventPtr);
		}
		
		TEST_METHOD(EventSubscribe)
		{
			FooSubscriber fooSubscriber;

			Event<Foo>::Subscribe(fooSubscriber);
			Event<Foo>::HandlePendingSubscribers();

			Foo foo;
			auto event = std::make_shared<Event<Foo>>(std::move(foo));
			event->Deliver();

			Assert::IsTrue(fooSubscriber.WasNotified);

			fooSubscriber.WasNotified = false;
			Event<Foo>::Unsubscribe(fooSubscriber);
			Event<Foo>::HandlePendingSubscribers();
			event->Deliver();
			Assert::IsFalse(fooSubscriber.WasNotified);
		}

		TEST_METHOD(EventPendingUnsubscribe)
		{
			RemoveSubscriber removeSub;
			FooSubscriber fooSub;
			Event<Foo>::Subscribe(removeSub);
			Event<Foo>::Subscribe(fooSub);
			Event<Foo>::HandlePendingSubscribers();
			
			Foo foo;
			auto event = std::make_shared<Event<Foo>>(std::move(foo));
			event->Deliver();
			
			Assert::IsTrue(removeSub.WasNotified);
			Assert::IsTrue(fooSub.WasNotified);
			
			Event<Foo>::HandlePendingSubscribers();

			removeSub.WasNotified = false;
			fooSub.WasNotified = false;
			event->Deliver();
			Assert::IsFalse(removeSub.WasNotified);
			Assert::IsTrue(fooSub.WasNotified);
			
			Event<Foo>::Unsubscribe(fooSub);
			Event<Foo>::HandlePendingSubscribers();
		}

		TEST_METHOD(EventQueueEnqueue)
		{
			GameTime gametime;
			Foo foo;
			FooSubscriber fooSubscriber;
			EventQueue queue;

			auto event = std::make_shared<Event<Foo>>();
			Assert::AreEqual(foo, event->Message());

			Assert::AreEqual(std::size_t(0), queue.Size());
			queue.Enqueue(event, gametime);
			Assert::AreEqual(std::size_t(1), queue.Size());

			Event<Foo>::Subscribe(fooSubscriber);
			Event<Foo>::HandlePendingSubscribers();
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			Assert::IsFalse(fooSubscriber.WasNotified);
			queue.Update(gametime);
			Assert::IsTrue(fooSubscriber.WasNotified);
			Assert::AreEqual(std::size_t(0), queue.Size());
			
			fooSubscriber.WasNotified = false;
			queue.Update(gametime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			Assert::AreEqual(std::size_t(0), queue.Size());
			
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point());
			queue.Enqueue(event, gametime, 1s);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(500ms));
			queue.Update(gametime);
			Assert::IsFalse(fooSubscriber.WasNotified);
			gametime.SetCurrentTime(std::chrono::high_resolution_clock::time_point(1s));
			queue.Update(gametime);
			Assert::IsTrue(fooSubscriber.WasNotified);
			
			Assert::IsTrue(queue.IsEmpty());
			Event<Foo>::UnsubscribeAll();
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState EventTests::_startMemState;
}
