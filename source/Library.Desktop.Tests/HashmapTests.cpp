#include "pch.h"
#include "CppUnitTest.h"
#include "Hashmap.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Hashmap<std::string, Foo>::Iterator>(const Hashmap<std::string, Foo>::Iterator& it)
	{
		try
		{
			return ToString((*it).first);
		}
		catch (const std::exception&)
		{
			return L"end()"s;
		}
	}
	
	template<>
	std::wstring ToString<Hashmap<std::string, Foo>::ConstIterator>(const Hashmap<std::string, Foo>::ConstIterator& it)
	{
		try
		{
			return ToString((*it).first);
		}
		catch (const std::exception&)
		{
			return L"cend()"s;
		}
	}

	template<>
	std::wstring ToString<std::pair<const std::string, Foo>>(const std::pair<const std::string, Foo>& pair)
	{
		return ToString(pair.first);
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(HashmapTests)
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
			Hashmap<std::string, Foo>::Iterator it;

			Assert::ExpectException<std::runtime_error>([&it]() { *it; });
			Assert::ExpectException<std::runtime_error>([&it]() { ++it; });
			Assert::ExpectException<std::runtime_error>([&it]() { it++; });

			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(11), myMap.BucketCount());
			it = myMap.begin();

			Assert::ExpectException<std::runtime_error>([&it]() { *it; });
			++it;
		}

		TEST_METHOD(ConstIteratorConstructors)
		{
			Hashmap<std::string, Foo>::ConstIterator it;

			Assert::ExpectException<std::runtime_error>([&it]() { *it; });
			Assert::ExpectException<std::runtime_error>([&it]() { ++it; });

			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(11), myMap.BucketCount());
			it = myMap.cbegin();

			Assert::ExpectException<std::runtime_error>([&it]() { *it; });
			++it;
		}

		TEST_METHOD(IteratorCopyConstructors)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair pair1 = std::make_pair("Andrew", Foo(1));
			myMap.Insert(pair1);
			Assert::AreEqual(std::size_t(1), myMap.Size());

			Hashmap<std::string, Foo>::Iterator it1 = myMap.begin();
			Hashmap<std::string, Foo>::Iterator it2 = it1;
			
			Assert::AreEqual(it1, it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(ConstIteratorCopyConstructors)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair pair1 = std::make_pair("Andrew", Foo(1));
			myMap.Insert(pair1);
			Assert::AreEqual(std::size_t(1), myMap.Size());

			Hashmap<std::string, Foo>::ConstIterator it1 = myMap.begin();
			Hashmap<std::string, Foo>::ConstIterator it2 = it1;

			Assert::AreEqual(it1, it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(IteratorAssignmentOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			Assert::AreEqual(std::size_t(2), myMap.Size());

			Hashmap<std::string, Foo>::Iterator it1 = myMap.begin();
			Hashmap<std::string, Foo>::Iterator it2 = myMap.Find("Drew");
			
			Assert::AreEqual(pair1, *it1);
			Assert::AreEqual(pair2, *it2);

			it2 = it1;

			Assert::AreEqual(it1, it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(ConstIteratorAssignmentOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);

			const Hashmap<std::string, Foo> myConstMap = myMap;
			Assert::AreEqual(std::size_t(2), myConstMap.Size());

			Hashmap<std::string, Foo>::ConstIterator it1 = myConstMap.cbegin();
			Hashmap<std::string, Foo>::ConstIterator it2 = myConstMap.Find("Drew");

			Assert::AreEqual(pair1, *it1);
			Assert::AreEqual(pair2, *it2);

			it2 = it1;

			Assert::AreEqual(it1, it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(IteratorEquivalenceOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			Assert::AreEqual(std::size_t(2), myMap.Size());

			Hashmap<std::string, Foo>::Iterator it1 = myMap.begin();
			Hashmap<std::string, Foo>::Iterator it2 = myMap.Find("Drew");

			Assert::AreEqual(pair1, *it1);
			Assert::AreEqual(pair2, *it2);

			it2 = it1;

			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(ConstIteratorEquivalenceOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);

			const Hashmap<std::string, Foo> myConstMap = myMap;
			Assert::AreEqual(std::size_t(2), myConstMap.Size());

			Hashmap<std::string, Foo>::ConstIterator it1 = myConstMap.cbegin();
			Hashmap<std::string, Foo>::ConstIterator it2 = myConstMap.Find("Drew");

			Assert::AreEqual(pair1, *it1);
			Assert::AreEqual(pair2, *it2);

			it2 = it1;

			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
			Assert::AreEqual(*it1, *it2);
		}

		TEST_METHOD(IteratorIncrementOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<const std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Hashmap<std::string, Foo>::Iterator it1 = myMap.begin();
			Hashmap<std::string, Foo>::Iterator it2 = myMap.begin();

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair1, *it1);

			++it1;
			++it2;

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair2, *it1);

			it1++;
			it2++;

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair3, *it1);			

			++it1;
			++it2;

			Assert::AreEqual(myMap.end(), it1);
			Assert::AreEqual(myMap.end(), it2);
		}

		TEST_METHOD(ConstIteratorIncrementOperators)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<const std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);

			Hashmap<std::string, Foo> myConstMap = myMap;
			Assert::AreEqual(std::size_t(3), myConstMap.Size());

			Hashmap<std::string, Foo>::ConstIterator it1 = myConstMap.cbegin();
			Hashmap<std::string, Foo>::ConstIterator it2 = myConstMap.cbegin();

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair1, *it1);

			++it1;
			++it2;

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair2, *it1);

			it1++;
			it2++;

			Assert::AreEqual(*it1, *it2);
			Assert::AreEqual(pair3, *it1);

			++it1;
			++it2;

			Assert::AreEqual(myConstMap.cend(), it1);
			Assert::AreEqual(myConstMap.cend(), it2);
		}

		TEST_METHOD(IteratorDereferenceOperators)
		{
			Hashmap<std::string, Foo>::Iterator it1;
			Assert::ExpectException<std::runtime_error>([&it1]() { *it1; });
		}

		TEST_METHOD(ConstIteratorDereferenceOperators)
		{
			Hashmap<std::string, Foo>::ConstIterator it1;
			Assert::ExpectException<std::runtime_error>([&it1]() { *it1; });
		}

		TEST_METHOD(Constructor)
		{
			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(0), myMap.Size());
			Assert::AreEqual(std::size_t(11), myMap.BucketCount());
			Assert::AreEqual(float(0), myMap.LoadFactor());

			Assert::ExpectException<std::runtime_error>([]() { Hashmap<std::string, Foo> newMap(1); });
		}

		TEST_METHOD(CopyConstructor)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Hashmap<std::string, Foo> myMap2(myMap);
			Assert::AreEqual(std::size_t(3), myMap2.Size());
			Assert::IsTrue(myMap2.ContainsKey("AJ"));
		}

		TEST_METHOD(MoveConstructor)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Hashmap<std::string, Foo> myMap2 = std::move(myMap);
			Assert::AreEqual(std::size_t(3), myMap2.Size());
			Assert::IsTrue(myMap2.ContainsKey("AJ"));
		}

		TEST_METHOD(AssignmentOperator)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Hashmap<std::string, Foo> myMap2 = myMap;
			Assert::AreEqual(std::size_t(3), myMap2.Size());
			Assert::IsTrue(myMap2.ContainsKey("AJ"));
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Hashmap<std::string, Foo> myMap2;
			myMap2 = std::move(myMap);
			Assert::AreEqual(std::size_t(3), myMap2.Size());
			Assert::IsTrue(myMap2.ContainsKey("AJ"));
		}

		TEST_METHOD(Find)
		{
			{
				Hashmap<std::string, Foo> myMap;
				std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
				std::pair<const std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);
				Assert::AreEqual(std::size_t(3), myMap.Size());

				Hashmap<std::string, Foo>::Iterator it1 = myMap.Find("Andrew");
				Assert::AreEqual(pair1, *it1);

				Hashmap<std::string, Foo>::Iterator it2 = myMap.Find("Myself");
				Assert::AreEqual(std::size_t(3), myMap.Size());
			}

			{
				Hashmap<std::string, Foo> myMap;
				std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<const std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
				std::pair<const std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);
				const Hashmap<std::string, Foo> myConstMap = myMap;
				Assert::AreEqual(std::size_t(3), myConstMap.Size());

				Hashmap<std::string, Foo>::ConstIterator it1 = myConstMap.Find("Andrew");
				Assert::AreEqual(pair1, *it1);

				Hashmap<std::string, Foo>::ConstIterator it2 = myConstMap.Find("Myself");
				Assert::AreEqual(myConstMap.cend(), it2);
				Assert::AreEqual(std::size_t(3), myMap.Size());
			}			
		}

		TEST_METHOD(Insert)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(1));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(1));
			myMap.Insert(pair1);
			Assert::IsFalse(myMap.Insert(pair1).second);
			myMap.Insert(pair2);
			myMap.Insert(pair3);

			Assert::AreEqual(std::size_t(3), myMap.Size());
			Assert::IsTrue(myMap.ContainsKey("Andrew"));
			Assert::IsTrue(myMap.ContainsKey("Drew"));
			Assert::IsTrue(myMap.ContainsKey("AJ"));
		}

		TEST_METHOD(BracketOperator)
		{
			Hashmap<std::string, Foo> myMap;
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			Assert::AreEqual(Foo(1), myMap["Andrew"]);
			Assert::AreEqual(Foo(2), myMap["Drew"]);
			Assert::AreEqual(Foo(3), myMap["AJ"]);

			Assert::AreEqual(Foo(), myMap["Myself"]);
			Assert::AreEqual(std::size_t(4), myMap.Size());
		}

		TEST_METHOD(Remove)
		{
			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(0), myMap.Size());
			std::pair<const std::string, Foo> pair1 = std::make_pair("Andrew"s, Foo(1));
			std::pair<const std::string, Foo> pair2 = std::make_pair("Drew"s, Foo(2));
			std::pair<const std::string, Foo> pair3 = std::make_pair("AJ"s, Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			myMap.Remove("Andrew"s);
			Assert::ExpectException<std::runtime_error>([&myMap]() { myMap.At("Andrew"); });
			Assert::AreEqual(std::size_t(2), myMap.Size());

			myMap.Remove("Jacob"s);
			Assert::AreEqual(std::size_t(2), myMap.Size());
		}

		TEST_METHOD(Clear)
		{
			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(0), myMap.Size());
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());

			myMap.Clear();
			Assert::ExpectException<std::runtime_error>([&myMap]() { myMap.At("Andrew"); });
			Assert::ExpectException<std::runtime_error>([&myMap]() { myMap.At("Drew"); });
			Assert::ExpectException<std::runtime_error>([&myMap]() { myMap.At("AJ"); });
			Assert::AreEqual(std::size_t(0), myMap.Size());
		}

		TEST_METHOD(Size)
		{
			Hashmap<std::string, Foo> myMap;
			Assert::AreEqual(std::size_t(0), myMap.Size());
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());
		}

		TEST_METHOD(BucketCount)
		{
			Hashmap<std::string, Foo> myMap(15);
			Assert::AreEqual(std::size_t(0), myMap.Size());
			Assert::AreEqual(std::size_t(15), myMap.BucketCount());
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());
			Assert::AreEqual(std::size_t(15), myMap.BucketCount());
		}

		TEST_METHOD(LoadFactor)
		{
			Hashmap<std::string, Foo> myMap(15);
			Assert::AreEqual(std::size_t(0), myMap.Size());
			Assert::AreEqual(std::size_t(15), myMap.BucketCount());
			Assert::AreEqual(float(0), myMap.LoadFactor());
			std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
			std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
			std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
			myMap.Insert(pair1);
			myMap.Insert(pair2);
			myMap.Insert(pair3);
			Assert::AreEqual(std::size_t(3), myMap.Size());
			Assert::AreEqual(std::size_t(15), myMap.BucketCount());
			Assert::AreEqual(float(0.2), myMap.LoadFactor());
		}

		TEST_METHOD(ContainsKey)
		{
			{
				Hashmap<std::string, Foo> myMap(15);
				Assert::AreEqual(std::size_t(0), myMap.Size());
				std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
				std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);

				Assert::IsTrue(myMap.ContainsKey("Andrew"));
				Assert::IsTrue(myMap.ContainsKey("Drew"));
				Assert::IsTrue(myMap.ContainsKey("AJ"));
				Assert::IsFalse(myMap.ContainsKey("Myself"));
				Assert::AreEqual(std::size_t(3), myMap.Size());
			}

			{
				Hashmap<std::string, Foo> myMap(15);
				Assert::AreEqual(std::size_t(0), myMap.Size());
				std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
				std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);

				const Hashmap<std::string, Foo> myConstMap(myMap);
				Assert::AreEqual(std::size_t(3), myConstMap.Size());

				Assert::IsTrue(myConstMap.ContainsKey("Andrew"));
				Assert::IsTrue(myConstMap.ContainsKey("Drew"));
				Assert::IsTrue(myConstMap.ContainsKey("AJ"));
				Assert::IsFalse(myConstMap.ContainsKey("Myself"));
				Assert::AreEqual(std::size_t(3), myConstMap.Size());
			}
		}

		TEST_METHOD(At)
		{
			{
				Hashmap<std::string, Foo> myMap(15);
				Assert::AreEqual(std::size_t(0), myMap.Size());
				std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
				std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);

				Assert::AreEqual(Foo(1), myMap.At("Andrew"));
				Assert::AreEqual(Foo(2), myMap.At("Drew"));
				Assert::AreEqual(Foo(3), myMap.At("AJ"));
				Assert::ExpectException<std::runtime_error>([&myMap]() { myMap.At("Myself"); });
			}

			{
				Hashmap<std::string, Foo> myMap(15);
				Assert::AreEqual(std::size_t(0), myMap.Size());
				std::pair<std::string, Foo> pair1 = std::make_pair("Andrew", Foo(1));
				std::pair<std::string, Foo> pair2 = std::make_pair("Drew", Foo(2));
				std::pair<std::string, Foo> pair3 = std::make_pair("AJ", Foo(3));
				myMap.Insert(pair1);
				myMap.Insert(pair2);
				myMap.Insert(pair3);

				const Hashmap<std::string, Foo> myConstMap(myMap);

				Assert::AreEqual(Foo(1), myConstMap.At("Andrew"));
				Assert::AreEqual(Foo(2), myConstMap.At("Drew"));
				Assert::AreEqual(Foo(3), myConstMap.At("AJ"));
				Assert::ExpectException<std::runtime_error>([&myConstMap]() { myConstMap.At("Myself"); });
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashmapTests::_startMemState;
}
