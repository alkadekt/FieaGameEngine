#include "pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& it)
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
	std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& it)
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
}

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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
			Vector<Foo>::Iterator it;

			auto expression1 = [&it]() { *it; };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&it]() { ++it; };
			Assert::ExpectException<std::runtime_error>(expression2);

			Vector<Foo> myVector;
			Vector<Foo>::Iterator it2 = myVector.begin();

			auto expression3 = [&it2]() { *it2; };
			Assert::ExpectException<std::out_of_range>(expression3);

			++it2;
		}

		TEST_METHOD(ConstIteratorConstructors)
		{
			Vector<Foo>::ConstIterator it;

			auto expression1 = [&it]() { *it; };
			Assert::ExpectException<std::runtime_error>(expression1);
			
			auto expression2 = [&it]() { ++it; };
			Assert::ExpectException<std::runtime_error>(expression2);
			
			const Vector<Foo> myVector;
			Vector<Foo>::ConstIterator it2 = myVector.begin();
			
			auto expression3 = [&it2]() { *it2; };
			Assert::ExpectException<std::out_of_range>(expression3);
			
			++it2;
		}

		TEST_METHOD(IteratorCopyConstructors)
		{
			const Foo a{ 10 };

			Vector<Foo> myVector;
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::Iterator it1 = myVector.begin();
			Assert::AreEqual(*it1, a);

			Vector<Foo>::Iterator it2{ it1 };
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorCopyConstructors)
		{
			const Foo a{ 10 };

			Vector<Foo> myVector;
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::ConstIterator it1 = myVector.cbegin();
			Assert::AreEqual(*it1, a);

			Vector<Foo>::ConstIterator it2{ it1 };
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(IteratorAssignmentOperators)
		{
			const Foo a{ 10 };

			Vector<Foo> myVector;
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::Iterator it1 = myVector.begin();
			Assert::AreEqual(*it1, a);

			Vector<Foo>::Iterator it2;
			it2 = it1;
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorAssignmentOperators)
		{
			const Foo a{ 10 };

			Vector<Foo> myVector;
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::ConstIterator it1 = myVector.cbegin();
			Assert::AreEqual(*it1, a);

			Vector<Foo>::ConstIterator it2;
			it2 = it1;
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(IteratorEquivalenceOperators)
		{

			Vector<Foo> myVector1;
			Vector<Foo>::Iterator it1 = myVector1.begin();
			Vector<Foo>::Iterator it2 = it1;
			Assert::IsTrue(it1 == it2);

			Vector<Foo> myVector2;
			it2 = myVector2.begin();
			Assert::IsFalse(it1 == it2);

			const Foo a{ 10 };
			const Foo b{ 20 };
			myVector2.PushBack(a);
			myVector2.PushBack(b);
			Assert::AreEqual(myVector2.Size(), size_t(2));

			Vector<Foo>::Iterator it3 = myVector2.begin();
			Vector<Foo>::Iterator it4 = myVector2.begin();
			Assert::AreEqual(*it3, a);
			Assert::IsTrue(it3 == it4);

			++it3;
			Assert::IsFalse(it3 == it4);
		}

		TEST_METHOD(ConstIteratorEquivalenceOperators)
		{

			Vector<Foo> myVector1;
			Vector<Foo>::ConstIterator it1 = myVector1.begin();
			Vector<Foo>::ConstIterator it2 = it1;
			Assert::IsTrue(it1 == it2);

			Vector<Foo> myVector2;
			it2 = myVector2.begin();
			Assert::IsFalse(it1 == it2);

			const Foo a{ 10 };
			const Foo b{ 20 };
			myVector2.PushBack(a);
			myVector2.PushBack(b);
			Assert::AreEqual(myVector2.Size(), size_t(2));

			Vector<Foo>::ConstIterator it3 = myVector2.begin();
			Vector<Foo>::ConstIterator it4 = myVector2.begin();
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

			Vector<Foo> myVector;
			myVector.PushBack(a);
			myVector.PushBack(b);
			myVector.PushBack(c);
			Assert::AreEqual(myVector.Size(), size_t(3));

			Vector<Foo>::Iterator it1 = myVector.begin();
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

			Vector<Foo> myVector;
			myVector.PushBack(a);
			myVector.PushBack(b);
			myVector.PushBack(c);
			Assert::AreEqual(myVector.Size(), size_t(3));
			const Vector<Foo> myVector2 = myVector;

			Vector<Foo>::ConstIterator it1 = myVector2.begin();
			Assert::AreEqual(*it1, a);
			++it1;
			Assert::AreEqual(*it1, b);
			it1++;
			Assert::AreEqual(*it1, c);
			++it1;
			Assert::AreEqual(myVector2.cend(), it1);
			Assert::AreEqual(myVector2.end(), it1);
		}

		TEST_METHOD(IteratorDereferenceOperators)
		{
			Vector<Foo> myVector;
			Vector<Foo>::Iterator it1 = myVector.begin();

			auto expression1 = [&it1]() { *it1; };
			Assert::ExpectException<std::out_of_range>(expression1);

			const Foo a{ 10 };
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::Iterator it2 = myVector.begin();
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(ConstIteratorDereferenceOperators)
		{
			Vector<Foo> myVector;
			Vector<Foo>::ConstIterator it1 = myVector.begin();

			auto expression1 = [&it1]() { *it1; };
			Assert::ExpectException<std::out_of_range>(expression1);

			const Foo a{ 10 };
			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			Vector<Foo>::ConstIterator it2 = myVector.begin();
			Assert::AreEqual(*it2, a);
		}

		TEST_METHOD(Find)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			Vector<Foo> myVector;
			myVector.PushBack(a);
			myVector.PushBack(b);
			myVector.PushBack(c);
			Assert::AreEqual(myVector.Size(), size_t(3));

			Vector<Foo>::Iterator it1 = myVector.Find(d);
			Assert::AreEqual(it1, myVector.end());

			Vector<Foo>::Iterator it2 = myVector.Find(b);
			Assert::AreEqual(*it2, b);

			const Vector<Foo> myConstVector = myVector;
			Assert::AreEqual(myConstVector.Size(), size_t(3));

			Vector<Foo>::ConstIterator it3 = myConstVector.Find(d);
			Assert::AreEqual(it3, myConstVector.end());

			Vector<Foo>::ConstIterator it4 = myConstVector.Find(b);
			Assert::AreEqual(*it4, b);
		}

		TEST_METHOD(Contains)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			Vector<Foo> myVector{ a, b, c };
			Assert::AreEqual(myVector.Size(), size_t(3));

			Assert::IsTrue(myVector.Contains(a));
			Assert::IsTrue(myVector.Contains(b));
			Assert::IsTrue(myVector.Contains(c));
			Assert::IsFalse(myVector.Contains(d));
		}

		TEST_METHOD(Reserve)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector{ a,b,c };

			myVector.Reserve(1);
			Assert::AreEqual(std::size_t(3), myVector.Capacity());
		}

		TEST_METHOD(Remove)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> myVector{ a,b,c,d,e };
			Assert::AreEqual(size_t(5), myVector.Size());
			Assert::AreEqual(size_t(5), myVector.Capacity());

			Vector<Foo>::Iterator badIt;
			auto expression1 = [&myVector, &badIt]() { myVector.Remove(badIt); };
			Assert::ExpectException<std::runtime_error>(expression1);
			
			Vector<Foo>::Iterator it = myVector.Find(b);
			bool worked = myVector.Remove(it);
			Assert::IsTrue(worked);
			Assert::AreEqual(myVector.Size(), size_t(4));
			Assert::AreEqual(myVector.Back(), e);
			
			myVector.Remove(myVector.Find(c));
			Assert::AreEqual(myVector.Size(), size_t(3));
			Assert::AreEqual(myVector.Back(), e);
			
			Vector<Foo>::Iterator it3 = myVector.Find(e);
			myVector.Remove(it3);
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Back(), d);
			
			Vector<Foo>::Iterator it4 = myVector.Find(d);
			myVector.Remove(it4);
			Assert::AreEqual(myVector.Size(), size_t(1));
			Assert::AreEqual(myVector.Back(), a);
			
			Vector<Foo>::Iterator it5 = myVector.Find(a);
			myVector.Remove(it5);
			Assert::AreEqual(myVector.Size(), size_t(0));
			
			Assert::AreEqual(size_t(5), myVector.Capacity());
		}

		TEST_METHOD(RemoveRange)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> myVector{ a,b,c,d,e };
			Assert::AreEqual(myVector.Size(), size_t(5));
			Assert::AreEqual(myVector.Capacity(), size_t(5));

			Vector<Foo>::Iterator badIt;
			Vector<Foo>::Iterator badIt2;
			auto expression1 = [&myVector, &badIt, &badIt2]() { myVector.Remove(badIt, badIt2); };
			Assert::ExpectException<std::runtime_error>(expression1);

			Vector<Foo>::Iterator it1 = myVector.Find(b);
			Vector<Foo>::Iterator it2 = myVector.Find(e);

			auto expression2 = [&myVector, &it1, &badIt2]() { myVector.Remove(it1, badIt2); };
			Assert::ExpectException<std::runtime_error>(expression2);

			auto expression3 = [&myVector, &it1, &it2]() { myVector.Remove(it2, it1); };
			Assert::ExpectException<std::runtime_error>(expression3);

			bool worked = myVector.Remove(it1, it2);
			Assert::IsTrue(worked);
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Back(), e);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Capacity(), size_t(5));
		}

		TEST_METHOD(Constructor)
		{
			Vector<Foo> myVector;
			Assert::IsTrue(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(0));

			auto expression1 = [&myVector]() { myVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);

			auto expression2 = [&myVector]() { myVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(CopyConstructor)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector1;
			myVector1.PushBack(a);
			myVector1.PushBack(b);
			myVector1.PushBack(c);

			Vector<Foo> myVector2(myVector1);
			Assert::AreEqual(myVector1.Size(), myVector2.Size());
			while (!myVector1.IsEmpty())
			{
				Assert::AreEqual(myVector1.Front(), myVector2.Front());
				myVector1.PopBack();
				myVector2.PopBack();
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector1;
			myVector1 = { a,b,c };

			Vector<Foo> myVector2 = std::move(myVector1);
			Assert::AreEqual(size_t(0), myVector1.Size());
			Assert::AreEqual(size_t(3), myVector2.Size());
			Assert::AreEqual(size_t(0), myVector1.Capacity());
			Assert::AreEqual(size_t(3), myVector2.Capacity());

		}

		TEST_METHOD(AssignmentOperator)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector1{ a,b,c };

			Vector<Foo> myVector2;
			myVector2 = myVector1;
			Assert::AreEqual(myVector1.Size(), myVector2.Size());

			while (!myVector1.IsEmpty())
			{
				Assert::AreEqual(myVector1.Back(), myVector2.Back());
				myVector1.PopBack();
				myVector2.PopBack();
			}
			Assert::AreEqual(myVector1.Size(), myVector2.Size());
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector1;
			{
				Vector<Foo> myVector2 = { a,b,c };
				myVector1 = std::move(myVector2);
			}

			Assert::AreEqual(size_t(3), myVector1.Size());
			Assert::AreEqual(size_t(3), myVector1.Capacity());
		}

		TEST_METHOD(AccessOperators)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector1;

			auto expression1 = [&myVector1]() { myVector1[0]; };
			Assert::ExpectException<std::out_of_range>(expression1);
			
			auto expression2 = [&myVector1]() { myVector1.At(0); };
			Assert::ExpectException<std::out_of_range>(expression2);
			
			myVector1 = { a,b,c };
			Assert::AreEqual(a, myVector1[0]);
			Assert::AreEqual(b, myVector1[1]);
			Assert::AreEqual(c, myVector1[2]);
			Assert::AreEqual(a, *myVector1.At(0));
			Assert::AreEqual(b, *myVector1.At(1));
			Assert::AreEqual(c, *myVector1.At(2));
			Assert::AreEqual(size_t(3), myVector1.Size());
			
			auto expression3 = [&myVector1]() { myVector1[3]; };
			Assert::ExpectException<std::out_of_range>(expression3);
			
			auto expression4 = [&myVector1]() { myVector1.At(3); };
			Assert::ExpectException<std::out_of_range>(expression4);
			
			const Vector<Foo> myVector2 = myVector1;
			Assert::AreEqual(a, myVector2[0]);
			Assert::AreEqual(b, myVector2[1]);
			Assert::AreEqual(c, myVector2[2]);
			Assert::AreEqual(a, *myVector2.At(0));
			Assert::AreEqual(b, *myVector2.At(1));
			Assert::AreEqual(c, *myVector2.At(2));
			
			auto expression5 = [&myVector2]() { myVector2[3]; };
			Assert::ExpectException<std::out_of_range>(expression5);
			
			auto expression6 = [&myVector2]() { myVector2.At(3); };
			Assert::ExpectException<std::out_of_range>(expression6);
			
			const Vector<Foo> myVector3;
			auto expression7 = [&myVector3]() { myVector3[0]; };
			Assert::ExpectException<std::out_of_range>(expression7);
			
			auto expression8 = [&myVector3]() { myVector3.At(0); };
			Assert::ExpectException<std::out_of_range>(expression8);
		}

		TEST_METHOD(Front)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector;
			const Vector<Foo> myConstVector;

			auto expression = [&myVector]() { myVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression1 = [&myConstVector]() { myConstVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);

			myVector.PushBack(a);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), a);

			myVector.PushBack(b);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), b);

			myVector.PushBack(c);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), c);

			const Vector<Foo> myVector2 = myVector;
			Assert::AreEqual(myVector2.Front(), a);
		}

		TEST_METHOD(Back)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector;
			const Vector<Foo> myConstVector;

			auto expression = [&myVector]() { myVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression);

			auto expression1 = [&myConstVector]() { myConstVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression1);

			myVector.PushBack(a);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), a);

			myVector.PushBack(b);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), b);

			myVector.PushBack(c);
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), c);

			const Vector<Foo> myVector2 = myVector;
			Assert::AreEqual(myVector2.Back(), c);
		}

		TEST_METHOD(PushBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector;
			Assert::IsTrue(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(0));

			myVector.PushBack(a);
			Assert::IsFalse(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(1));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), a);
			Assert::AreSame(myVector.Front(), myVector.Back());

			myVector.PushBack(b);
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), b);

			myVector.PushBack(c);
			Assert::AreEqual(myVector.Size(), size_t(3));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), c);
		}

		TEST_METHOD(PopBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector;
			Assert::IsTrue(myVector.IsEmpty());
			auto expression = [&myVector]() { myVector.PopBack(); };
			Assert::ExpectException<std::runtime_error>(expression);

			myVector.PushBack(a);
			Assert::IsFalse(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(1));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), a);
			Assert::AreSame(myVector.Front(), myVector.Back());

			myVector.PushBack(b);
			Assert::IsFalse(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), b);

			myVector.PushBack(c);
			Assert::IsFalse(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(3));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), c);

			myVector.PopBack();
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), b);

			myVector.PopBack();
			Assert::AreEqual(myVector.Size(), size_t(1));
			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), a);
			Assert::AreSame(myVector.Front(), myVector.Back());

			myVector.PopBack();
			Assert::IsTrue(myVector.IsEmpty());
			Assert::AreEqual(myVector.Size(), size_t(0));

			auto expression1 = [&myVector]() { myVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);
			auto expression2 = [&myVector]() { myVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a{ 10 };

			Vector<Foo> myVector;
			Assert::IsTrue(myVector.IsEmpty());

			myVector.PushBack(a);
			Assert::IsFalse(myVector.IsEmpty());

			myVector.PopBack();
			Assert::IsTrue(myVector.IsEmpty());
		}

		TEST_METHOD(Size)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };

			Vector<Foo> myVector;
			Assert::AreEqual(myVector.Size(), size_t(0));

			myVector.PushBack(a);
			Assert::AreEqual(myVector.Size(), size_t(1));

			myVector.PushBack(b);
			Assert::AreEqual(myVector.Size(), size_t(2));

			myVector.PopBack();
			Assert::AreEqual(myVector.Size(), size_t(1));
		}

		TEST_METHOD(Capacity)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 20 };

			Vector<Foo> myVector;
			Assert::AreEqual(myVector.Capacity(), size_t(0));

			myVector.PushBack(a);
			Assert::AreEqual(myVector.Capacity(), size_t(1));

			myVector.PushBack(b);
			Assert::AreEqual(myVector.Capacity(), size_t(2));

			myVector.PushBack(c);
			Assert::AreEqual(myVector.Capacity(), size_t(4));

			myVector.PopBack();
			Assert::AreEqual(myVector.Size(), size_t(2));
			Assert::AreEqual(myVector.Capacity(), size_t(4));
		}

		TEST_METHOD(Resize)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			{
				Vector<Foo> myVector;
				Assert::IsTrue(myVector.IsEmpty());

				myVector.PushBack(a);
				myVector.PushBack(b);
				myVector.PushBack(c);

				Assert::AreEqual(myVector.Front(), a);
				Assert::AreEqual(myVector.Back(), c);
				Assert::AreEqual(size_t(3), myVector.Size());
				Assert::IsFalse(myVector.IsEmpty());

				myVector.Resize(1);
				Assert::AreEqual(myVector.Front(), a);
				Assert::AreEqual(myVector.Back(), a);
				Assert::AreEqual(size_t(1), myVector.Size());
			}

			{
				Vector<Foo> myVector;
				Assert::IsTrue(myVector.IsEmpty());
				
				myVector.PushBack(a);
				myVector.PushBack(b);
				myVector.PushBack(c);
				
				Assert::AreEqual(myVector.Front(), a);
				Assert::AreEqual(myVector.Back(), c);
				Assert::AreEqual(size_t(3), myVector.Size());
				Assert::IsFalse(myVector.IsEmpty());
				
				myVector.Resize(6);
				Assert::AreEqual(myVector.Front(), a);
				Assert::AreEqual(myVector.Back(), Foo());
				Assert::AreEqual(size_t(6), myVector.Size());
				Assert::AreEqual(size_t(6), myVector.Capacity());
			}
		}

		TEST_METHOD(Clear)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> myVector;
			Assert::IsTrue(myVector.IsEmpty());

			myVector.PushBack(a);
			myVector.PushBack(b);
			myVector.PushBack(c);

			Assert::AreEqual(myVector.Front(), a);
			Assert::AreEqual(myVector.Back(), c);
			Assert::AreEqual(myVector.Size(), size_t(3));
			Assert::IsFalse(myVector.IsEmpty());

			myVector.Clear();
			Assert::AreEqual(myVector.Size(), size_t(0));
			Assert::IsTrue(myVector.IsEmpty());
			
			auto expression1 = [&myVector]() { myVector.Front(); };
			Assert::ExpectException<std::runtime_error>(expression1);
			
			auto expression2 = [&myVector]() { myVector.Back(); };
			Assert::ExpectException<std::runtime_error>(expression2);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState VectorTests::_startMemState;
}
