#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& it)
	{
		try
		{
			return ToString(*it);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}

	template<>
	std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& it)
	{
		try
		{
			return ToString(*it);
		}
		catch (const std::exception&)
		{
			return L"cend()"s;
		}
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(SListTests)
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
				Assert::Fail(L"There was a memory leak.");
			}
#endif
		}

		TEST_METHOD(IteratorConstructors)
		{
			SList<Foo>::Iterator it;

			auto expression1 = [&it]() { *it; };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&it]() { ++it; };
			Assert::ExpectException<std::runtime_error>(expression2);

			SList<Foo> myList;
			SList<Foo>::Iterator it2 = myList.begin();

			auto expression3 = [&it2]() { *it2; };
			Assert::ExpectException<std::runtime_error>(expression3);

			++it2;
		}

		TEST_METHOD(ConstIteratorConstructors)
		{
			SList<Foo>::ConstIterator it;

			auto expression1 = [&it]() { *it; };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&it]() { ++it; };
			Assert::ExpectException<std::runtime_error>(expression2);

			const SList<Foo> myList;
			SList<Foo>::ConstIterator it2 = myList.begin();

			auto expression3 = [&it2]() { *it2; };
			Assert::ExpectException<std::runtime_error>(expression3);

			++it2;
		}

		TEST_METHOD(IteratorCopyConstructors)
		{
			const Foo a{ 10 };

			SList<Foo> myList;
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::Iterator it1 = myList.begin();
			Assert::AreEqual(*it1, a);

			SList<Foo>::Iterator it2{ it1 };
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorCopyConstructors)
		{
			const Foo a{ 10 };

			SList<Foo> myList;
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::ConstIterator it1 = myList.cbegin();
			Assert::AreEqual(*it1, a);

			SList<Foo>::ConstIterator it2{ it1 };
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(IteratorAssignmentOperators)
		{
			const Foo a{ 10 };

			SList<Foo> myList;
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::Iterator it1 = myList.begin();
			Assert::AreEqual(*it1, a);

			SList<Foo>::Iterator it2;
			it2 = it1;
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorAssignmentOperators)
		{
			const Foo a{ 10 };

			SList<Foo> myList;
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::ConstIterator it1 = myList.cbegin();
			Assert::AreEqual(*it1, a);

			SList<Foo>::ConstIterator it2;
			it2 = it1;
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(IteratorEquivalenceOperators)
		{

			SList<Foo> myList1;
			SList<Foo>::Iterator it1 = myList1.begin();
			SList<Foo>::Iterator it2 = it1;
			Assert::IsTrue(it1 == it2);

			SList<Foo> myList2;
			it2 = myList2.begin();
			Assert::IsFalse(it1 == it2);

			const Foo a{ 10 };
			const Foo b{ 20 };
			myList2.PushBack(a);
			myList2.PushBack(b);
			Assert::AreEqual(myList2.Size(), size_t(2));

			SList<Foo>::Iterator it3 = myList2.begin();
			SList<Foo>::Iterator it4 = myList2.begin();
			Assert::AreEqual(*it3, a);
			Assert::IsTrue(it3 == it4);

			++it3;
			Assert::IsFalse(it3 == it4);
		}

		TEST_METHOD(ConstIteratorEquivalenceOperators)
		{

			SList<Foo> myList1;
			SList<Foo>::ConstIterator it1 = myList1.begin();
			SList<Foo>::ConstIterator it2 = it1;
			Assert::IsTrue(it1 == it2);

			SList<Foo> myList2;
			it2 = myList2.begin();
			Assert::IsFalse(it1 == it2);

			const Foo a{ 10 };
			const Foo b{ 20 };
			myList2.PushBack(a);
			myList2.PushBack(b);
			Assert::AreEqual(myList2.Size(), size_t(2));

			SList<Foo>::ConstIterator it3 = myList2.begin();
			SList<Foo>::ConstIterator it4 = myList2.begin();
			Assert::AreEqual(*it3, a);
			Assert::IsTrue(it3 == it4);

			++it3;
			Assert::IsFalse(it3 == it4);
		}

		TEST_METHOD(IteratorIncrementOperators)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			myList.PushBack(a);
			myList.PushBack(b);
			myList.PushBack(c);
			Assert::AreEqual(myList.Size(), size_t(3));

			SList<Foo>::Iterator it1 = myList.begin();
			Assert::AreEqual(*it1, a);
			++it1;
			Assert::AreEqual(*it1, b);
			it1++;
			Assert::AreEqual(*it1, c);
		}

		TEST_METHOD(ConstIteratorIncrementOperators)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			myList.PushBack(a);
			myList.PushBack(b);
			myList.PushBack(c);
			Assert::AreEqual(myList.Size(), size_t(3));
			const SList<Foo> myList2 = myList;

			SList<Foo>::ConstIterator it1 = myList2.begin();
			Assert::AreEqual(*it1, a);
			++it1;
			Assert::AreEqual(*it1, b);
			it1++;
			Assert::AreEqual(*it1, c);
			++it1;
			Assert::AreEqual(myList2.cend(), it1);
			Assert::AreEqual(myList2.end(), it1);
		}

		TEST_METHOD(IteratorDereferenceOperators)
		{
			SList<Foo> myList;
			SList<Foo>::Iterator it1 = myList.begin();

			auto expression1 = [&it1]() { *it1; };
			Assert::ExpectException<std::runtime_error>(expression1);

			const Foo a{ 10 };
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::Iterator it2 = myList.begin();
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorDereferenceOperators)
		{
			SList<Foo> myList;
			SList<Foo>::ConstIterator it1 = myList.begin();

			auto expression1 = [&it1]() { *it1; };
			Assert::ExpectException<std::runtime_error>(expression1);

			const Foo a{ 10 };
			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));

			SList<Foo>::ConstIterator it2 = myList.begin();
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(InsertAfter)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			SList<Foo> myList1;
			SList<Foo>::Iterator it1;

			auto expression1 = [&myList1, &it1, &a]() { myList1.InsertAfter(it1, a); };
			Assert::ExpectException<std::runtime_error>(expression1);

			SList<Foo>::Iterator it2 = myList1.begin();
			myList1.InsertAfter(it2, a);
			Assert::AreEqual(myList1.Size(), size_t(1));

			myList1.InsertAfter(it2, b);
			Assert::AreEqual(myList1.Size(), size_t(2));
			Assert::AreEqual(myList1.Back(), b);

			myList1.PushBack(c);
			SList<Foo>::Iterator it3 = myList1.Find(b);
			myList1.InsertAfter(it3, d);
			Assert::AreEqual(myList1.Size(), size_t(4));
			Assert::AreEqual(myList1.Back(), c);
		}

		TEST_METHOD(Find)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			SList<Foo> myList;
			myList = { a,b,c };
			Assert::AreEqual(myList.Size(), size_t(3));

			SList<Foo>::Iterator it1 = myList.Find(d);
			Assert::AreEqual(it1, myList.end());

			SList<Foo>::Iterator it2 = myList.Find(b);
			Assert::AreEqual(*it2, b);

			const SList<Foo> myList2 = myList;
			SList<Foo>::ConstIterator it3 = myList2.Find(d);
			Assert::AreEqual(it3, myList2.cend());

			SList<Foo>::ConstIterator it4 = myList2.Find(b);
			Assert::AreEqual(*it4, b);
		}

		TEST_METHOD(Remove)
		{
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };
				const Foo d{ 40 };

				SList<Foo> myList;
				myList.PushBack(a);
				myList.PushBack(b);
				myList.PushBack(c);
				Assert::AreEqual(size_t(3), myList.Size());

				myList.Remove(d);
				Assert::AreEqual(size_t(3), myList.Size());

				myList.Remove(a);
				Assert::AreEqual(size_t(2), myList.Size());
				Assert::AreEqual(b, myList.Front());

				myList.Remove(c);
				Assert::AreEqual(size_t(1), myList.Size());
				Assert::AreEqual(myList.Back(), b);

				myList.Remove(b);
				Assert::AreEqual(size_t(0), myList.Size());
				auto expression1 = [&myList]() { myList.Front(); };
				Assert::ExpectException<std::runtime_error>(expression1);

				auto expression2 = [&myList]() { myList.Back(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
			
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };
				const Foo d{ 40 };

				SList<Foo> myList;
				myList.PushBack(a);
				myList.PushBack(b);
				myList.PushBack(c);
				Assert::AreEqual(size_t(3), myList.Size());

				SList<Foo>::Iterator it = myList.Find(d);
				myList.Remove(it);
				Assert::AreEqual(size_t(3), myList.Size());

				it = myList.Find(a);
				myList.Remove(it);
				Assert::AreEqual(size_t(2), myList.Size());
				Assert::AreEqual(b, myList.Front());
				
				it = myList.Find(c);
				myList.Remove(it);
				Assert::AreEqual(size_t(1), myList.Size());
				Assert::AreEqual(myList.Back(), b);
				
				it = myList.Find(b);
				myList.Remove(it);
				Assert::AreEqual(size_t(0), myList.Size());
				auto expression1 = [&myList]() { myList.Front(); };
				Assert::ExpectException<std::runtime_error>(expression1);
				
				auto expression2 = [&myList]() { myList.Back(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}

			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };
				const Foo d{ 40 };

				SList<Foo> myList;
				myList.PushBack(a);
				myList.PushBack(b);
				myList.PushBack(c);
				Assert::AreEqual(size_t(3), myList.Size());

				myList.Remove(myList.Find(d));
				Assert::AreEqual(size_t(3), myList.Size());

				myList.Remove(myList.Find(a));
				Assert::AreEqual(size_t(2), myList.Size());
				Assert::AreEqual(b, myList.Front());

				myList.Remove(myList.Find(c));
				Assert::AreEqual(size_t(1), myList.Size());
				Assert::AreEqual(myList.Back(), b);

				myList.Remove(myList.Find(b));
				Assert::AreEqual(size_t(0), myList.Size());
				auto expression1 = [&myList]() { myList.Front(); };
				Assert::ExpectException<std::runtime_error>(expression1);

				auto expression2 = [&myList]() { myList.Back(); };
				Assert::ExpectException<std::runtime_error>(expression2);
			}
		}

		TEST_METHOD(Constructor)
		{
			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(0));

			auto expression1 = [&myList]() { myList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&myList]() { myList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(CopyConstructor)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList1;
			myList1.PushFront(a);
			myList1.PushFront(b);
			myList1.PushFront(c);

			SList<Foo> myList2(myList1);
			Assert::AreEqual(myList1.Size(), myList2.Size());
			while (!myList1.IsEmpty())
			{
				Assert::AreEqual(myList1.Front(), myList2.Front());
				myList1.PopFront();
				myList2.PopFront();
			}
		}

		TEST_METHOD(AssignmentOperator)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList1{ a, b, c };

			SList<Foo> myList2; 
			myList2 = myList1;
			Assert::AreEqual(myList1.Size(), myList2.Size());
			while (!myList1.IsEmpty())
			{
				Assert::AreEqual(myList1.Front(), myList2.Front());
				myList1.PopFront();
				myList2.PopFront();
			}
		}

		TEST_METHOD(Front)
		{
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				SList<Foo> myList;

				auto expression = [&myList]() { myList.Front(); };
				Assert::ExpectException<std::runtime_error>(expression);

				myList.PushFront(a);
				Assert::AreEqual(myList.Front(), a);
				Assert::AreEqual(myList.Back(), a);

				myList.PushFront(b);
				Assert::AreEqual(myList.Front(), b);
				Assert::AreEqual(myList.Back(), a);

				myList.PushFront(c);
				Assert::AreEqual(myList.Front(), c);
				Assert::AreEqual(myList.Back(), a);
			}

			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				SList<Foo> myList;

				const SList<Foo> myList4 = myList;
				auto expression = [&myList4]() { myList4.Front(); };
				Assert::ExpectException<std::runtime_error>(expression);

				myList.PushFront(a);
				const SList<Foo> myList1 = myList;
				Assert::AreEqual(myList1.Front(), a);
				Assert::AreEqual(myList1.Back(), a);

				myList.PushFront(b);
				const SList<Foo> myList2 = myList;
				Assert::AreEqual(myList2.Front(), b);
				Assert::AreEqual(myList2.Back(), a);

				myList.PushFront(c);
				const SList<Foo> myList3 = myList;
				Assert::AreEqual(myList3.Front(), c);
				Assert::AreEqual(myList3.Back(), a);
			}			
		}

		TEST_METHOD(Back)
		{
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				SList<Foo> myList;

				auto expression = [&myList]() { myList.Back(); };
				Assert::ExpectException<std::runtime_error>(expression);

				myList.PushBack(a);
				Assert::AreEqual(myList.Front(), a);
				Assert::AreEqual(myList.Back(), a);

				myList.PushBack(b);
				Assert::AreEqual(myList.Front(), a);
				Assert::AreEqual(myList.Back(), b);

				myList.PushBack(c);
				Assert::AreEqual(myList.Front(), a);
				Assert::AreEqual(myList.Back(), c);
			}			

			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };

				SList<Foo> myList;

				const SList<Foo> myList4 = myList;
				auto expression = [&myList4]() { myList4.Back(); };
				Assert::ExpectException<std::runtime_error>(expression);

				myList.PushBack(a);
				const SList<Foo> myList1 = myList;
				Assert::AreEqual(myList1.Front(), a);
				Assert::AreEqual(myList1.Back(), a);

				myList.PushBack(b);
				const SList<Foo> myList2 = myList;
				Assert::AreEqual(myList2.Front(), a);
				Assert::AreEqual(myList2.Back(), b);

				myList.PushBack(c);
				const SList<Foo> myList3 = myList;
				Assert::AreEqual(myList3.Front(), a);
				Assert::AreEqual(myList3.Back(), c);
			}
		}

		TEST_METHOD(PushFront)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());

			myList.PushFront(a);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());

			myList.PushFront(b);
			Assert::AreEqual(myList.Size(), size_t(2));
			Assert::AreEqual(myList.Front(), b);
			Assert::AreEqual(myList.Back(), a);

			myList.PushFront(c);
			Assert::AreEqual(myList.Size(), size_t(3));
			Assert::AreEqual(myList.Front(), c);
			Assert::AreEqual(myList.Back(), a);
		}

		TEST_METHOD(PopFront)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());
			auto expression = [&myList]() { myList.PopFront(); };
			Assert::ExpectException<std::runtime_error>(expression);

			myList.PushFront(a);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());

			myList.PushFront(b);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(2));
			Assert::AreEqual(myList.Front(), b);
			Assert::AreEqual(myList.Back(), a);

			myList.PopFront();
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());

			myList.PopFront();
			Assert::IsTrue(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(0));

			auto expression1 = [&myList]() { myList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);
			auto expression2 = [&myList]() { myList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(PushBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(0));

			myList.PushBack(a);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());

			myList.PushBack(b);
			Assert::AreEqual(myList.Size(), size_t(2));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), b);

			myList.PushBack(c);
			Assert::AreEqual(myList.Size(), size_t(3));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), c);
		}

		TEST_METHOD(PopBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());
			auto expression = [&myList]() { myList.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			myList.PushBack(a);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());
			
			myList.PushBack(b);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(2));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), b);

			myList.PushBack(c);
			Assert::IsFalse(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(3));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), c);

			myList.PopBack();
			Assert::AreEqual(myList.Size(), size_t(2));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), b);
			
			myList.PopBack();
			Assert::AreEqual(myList.Size(), size_t(1));
			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), a);
			Assert::AreSame(myList.Front(), myList.Back());
			
			myList.PopBack();
			Assert::IsTrue(myList.IsEmpty());
			Assert::AreEqual(myList.Size(), size_t(0));
			
			auto expression1 = [&myList]() { myList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);
			auto expression2 = [&myList]() { myList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a{ 10 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());

			myList.PushFront(a);
			Assert::IsFalse(myList.IsEmpty());

			myList.PopFront();
			Assert::IsTrue(myList.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };

			SList<Foo> myList;
			Assert::AreEqual(myList.Size(), size_t(0));

			myList.PushBack(a);
			Assert::AreEqual(myList.Size(), size_t(1));
			
			myList.PushBack(b);
			Assert::AreEqual(myList.Size(), size_t(2));
			
			myList.PopBack();
			Assert::AreEqual(myList.Size(), size_t(1));
		}

		TEST_METHOD(Clear)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> myList;
			Assert::IsTrue(myList.IsEmpty());

			myList.PushBack(a);
			myList.PushBack(b);
			myList.PushBack(c);

			Assert::AreEqual(myList.Front(), a);
			Assert::AreEqual(myList.Back(), c);
			Assert::AreEqual(myList.Size(), size_t(3));
			Assert::IsFalse(myList.IsEmpty());

			myList.Clear();
			Assert::AreEqual(myList.Size(), size_t(0));
			Assert::IsTrue(myList.IsEmpty());

			auto expression1 = [&myList]() { myList.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&myList]() { myList.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState SListTests::_startMemState;
}
