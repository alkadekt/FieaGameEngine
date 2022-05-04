#include "pch.h"
#include "CppUnitTest.h"
#include "HashFunctor.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace LibraryDesktopTests
{
	TEST_CLASS(HashFunctorTests)
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

		TEST_METHOD(AdditiveHashStdString)
		{
			HashFunctor<std::string> myFunctor;
			const std::string myName = "Andrew"s;
			const std::string myName2 = "Andrew"s;
			const std::string myName3 = "Drew"s;

			std::size_t result = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName.c_str()), myName.size());
			std::size_t result2 = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName2.c_str()), myName2.size());
			std::size_t result3 = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName3.c_str()), myName3.size());
			
			Assert::AreEqual(result, result2);
			Assert::AreNotEqual(result, result3);
		}

		TEST_METHOD(HashFunctorStdString)
		{
			{
				std::string myName = "Andrew"s;
				std::string myName2 = "Andrew"s;
				std::string myName3 = "Drew"s;

				std::size_t result = HashFunctor<std::string>()(myName);
				std::size_t result2 = HashFunctor<std::string>()(myName2);
				std::size_t result3 = HashFunctor<std::string>()(myName3);

				Assert::AreEqual(result, result2);
				Assert::AreNotEqual(result, result3);
			}

			{
				const std::string myName = "Andrew"s;
				const std::string myName2 = myName;
				const std::string myName3 = "Drew"s;

				std::size_t result = HashFunctor<const std::string>()(myName);
				std::size_t result2 = HashFunctor<const std::string>()(myName2);
				std::size_t result3 = HashFunctor<const std::string>()(myName3);

				Assert::AreEqual(result, result2);
				Assert::AreNotEqual(result, result3);
			}
		}

		TEST_METHOD(AdditiveHashCharStar)
		{
			HashFunctor<char*> myFunctor;
			const char* myName = "Andrew";
			const char* myName2 = "Andrew";
			const char* myName3 = "Drew";

			std::size_t result = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName), strlen(myName));
			std::size_t result2 = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName2), strlen(myName2));
			std::size_t result3 = myFunctor.AdditiveHash(reinterpret_cast<const std::uint8_t*>(myName3), strlen(myName3));

			Assert::AreEqual(result, result2);
			Assert::AreNotEqual(result, result3);
		}

		TEST_METHOD(HashFunctorCharStar)
		{
			{
				char* myName = "Andrew";
				char* myName2 = "Andrew";
				char* myName3 = "Drew";
			
				std::size_t result = HashFunctor<char*>()(myName);
				std::size_t result2 = HashFunctor<char*>()(myName2);
				std::size_t result3 = HashFunctor<char*>()(myName3);
			
				Assert::AreEqual(result, result2);
				Assert::AreNotEqual(result, result3);
			}
			
			{
				const char* myName = "Andrew";
				const char* myName2 = "Andrew";
				const char* myName3 = "Drew";
			
				std::size_t result = HashFunctor<const char*>()(myName);
				std::size_t result2 = HashFunctor<const char*>()(myName2);
				std::size_t result3 = HashFunctor<const char*>()(myName3);
			
				Assert::AreEqual(result, result2);
				Assert::AreNotEqual(result, result3);
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState HashFunctorTests::_startMemState;
}
