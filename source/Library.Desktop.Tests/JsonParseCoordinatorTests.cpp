#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "JsonParseCoordinator.h"
#include "TypeRegistry.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<JsonParseCoordinator>(JsonParseCoordinator* d)
	{
		try
		{
			d;
			return L"Parse Coordinator";
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonParseCoordinatorTests)
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

		TEST_METHOD(Constructor)
		{
			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);
			
			Assert::IsFalse(parseCoord.IsClone());
			Assert::AreEqual(std::size_t(0), parseCoord.Helpers().Size());
			Assert::AreEqual(std::size_t(0), container->Data.Size());
			Assert::AreEqual(&parseCoord, container->GetJsonParseCoordinator());
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				std::string input = R"({ "integer": 101 })";

				JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
				auto container = &parsedDataContainer;
				JsonParseCoordinator parseCoord(container);
				std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
				parseCoord.AddHelper(integerHelper);

				parseCoord.Parse(input);
				Assert::IsFalse(parseCoord.IsClone());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());

				JsonParseCoordinator parseCoord2(std::move(parseCoord));
				Assert::IsFalse(parseCoord2.IsClone());
				Assert::AreEqual(std::size_t(1), parseCoord2.Helpers().Size());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());
			}

			{
				std::string input = R"({ "integer": 101 })";
				std::string input2 = R"({ "integer": 102 })";

				JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
				auto container = &parsedDataContainer;
				JsonParseCoordinator parseCoord(container);
				std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
				parseCoord.AddHelper(integerHelper);

				JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer3;
				auto container3 = &parsedDataContainer3;
				JsonParseCoordinator parseCoord3(container3);
				std::shared_ptr<JsonIntegerParseHelper> integerHelper3 = std::make_shared<JsonIntegerParseHelper>();
				parseCoord3.AddHelper(integerHelper3);

				parseCoord.Parse(input);
				Assert::IsFalse(parseCoord.IsClone());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());

				parseCoord3.Parse(input2);
				Assert::IsFalse(parseCoord3.IsClone());
				Assert::AreEqual(std::size_t(1), container3->Data.Size());
				Assert::AreEqual(102, container3->Data.Front());

				JsonParseCoordinator& parseCoord2 = *parseCoord.Clone();
				Assert::IsTrue(parseCoord2.IsClone());
				Assert::AreEqual(std::size_t(1), parseCoord2.Helpers().Size());
				Assert::AreEqual(std::size_t(0), parseCoord2.GetParsedDataContainer()->Depth());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());

				parseCoord3 = std::move(parseCoord2);
				
				Assert::IsTrue(parseCoord3.IsClone());
				Assert::AreEqual(std::size_t(1), parseCoord3.Helpers().Size());
				Assert::AreEqual(std::size_t(0), parseCoord3.GetParsedDataContainer()->Depth());
				Assert::AreEqual(std::size_t(1), container3->Data.Size());
			}			
		}

		TEST_METHOD(Clone)
		{
			std::string input = R"({ "integer": 101 })";

			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);
			std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			parseCoord.AddHelper(integerHelper);

			parseCoord.Parse(input);
			Assert::IsFalse(parseCoord.IsClone());
			Assert::AreEqual(std::size_t(1), container->Data.Size());
			Assert::AreEqual(101, container->Data.Front());

			JsonParseCoordinator* parseCoord2 = parseCoord.Clone();
			Assert::IsTrue(parseCoord2->IsClone());
			Assert::AreEqual(std::size_t(1), parseCoord2->Helpers().Size());
			Assert::AreEqual(std::size_t(0), parseCoord2->GetParsedDataContainer()->Depth());
			Assert::AreEqual(std::size_t(1), container->Data.Size());
			Assert::AreEqual(101, container->Data.Front());
			delete parseCoord2;
		}

		TEST_METHOD(AddRemoveHelper)
		{
			{
				JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
				auto container = &parsedDataContainer;
				JsonParseCoordinator parseCoord(container);
				std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();

				parseCoord.AddHelper(integerHelper);
				Assert::AreEqual(std::size_t(1), parseCoord.Helpers().Size());
				
				parseCoord.RemoveHelper(integerHelper);
				Assert::AreEqual(std::size_t(0), parseCoord.Helpers().Size());
			}

			{
				std::string input = R"({ "integer": 101 })";
				
				JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
				auto container = &parsedDataContainer;
				JsonParseCoordinator parseCoord(container);
				std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
				parseCoord.AddHelper(integerHelper);
				
				parseCoord.Parse(input);
				Assert::IsFalse(parseCoord.IsClone());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());
				
				JsonParseCoordinator& parseCoord2 = *parseCoord.Clone();
				Assert::IsTrue(parseCoord2.IsClone());
				Assert::AreEqual(std::size_t(1), parseCoord2.Helpers().Size());
				Assert::AreEqual(std::size_t(0), parseCoord2.GetParsedDataContainer()->Depth());
				Assert::AreEqual(std::size_t(1), container->Data.Size());
				Assert::AreEqual(101, container->Data.Front());
				
				std::shared_ptr<JsonIntegerParseHelper> otherIntegerHelper = 
					std::make_shared<JsonIntegerParseHelper>();
				parseCoord2.AddHelper(otherIntegerHelper);
				Assert::AreEqual(std::size_t(2), parseCoord2.Helpers().Size());
				
				std::shared_ptr<IJsonParseHelper> cloneIntegerHelper = *parseCoord2.Helpers().At(0);
				parseCoord2.RemoveHelper(cloneIntegerHelper);
				Assert::AreEqual(std::size_t(1), parseCoord2.Helpers().Size());
				
				delete& parseCoord2;
			}			
		}

		TEST_METHOD(SimpleIntegerParse)
		{
			std::string input = R"({ "integer": 101 })";

			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);
			std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			parseCoord.AddHelper(integerHelper);

			parseCoord.Parse(input);
			Assert::AreEqual(std::size_t(1), container->Data.Size());
			Assert::AreEqual(101, container->Data.Front());
		}

		TEST_METHOD(ParseFromString)
		{
			//std::string input = R"({ "integer": { "integer": [1, 2, 3] } })";
			//
			//JsonIntegerParseHelper::ParsedDataContainer* container = 
			//	new JsonIntegerParseHelper::ParsedDataContainer();
			//JsonParseCoordinator parseCoord(container);
			//std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			//parseCoord.AddHelper(integerHelper);
			//
			//Assert::AreEqual(std::size_t(0), container->Depth());
			//
			//parseCoord.Parse(input);
			//Assert::AreEqual(std::size_t(3), container->Data.Size());
			//Assert::AreEqual(1, container->Data.Front());
			//Assert::AreEqual(3, container->Data.Back());
			//Assert::AreEqual(std::size_t(0), container->Depth());
		}

		TEST_METHOD(ParseFromIStream)
		{
			//stringstream input;
			//input << R"({ "integer": { "integer": [1, 2, 3] } })";
			//
			//JsonIntegerParseHelper::ParsedDataContainer* container = 
			//	new JsonIntegerParseHelper::ParsedDataContainer();
			//JsonParseCoordinator parseCoord(container);
			//std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			//parseCoord.AddHelper(integerHelper);
			//
			//Assert::AreEqual(std::size_t(0), container->Depth());
			//
			//parseCoord.Parse(input);
			//Assert::AreEqual(std::size_t(3), container->Data.Size());
			//Assert::AreEqual(1, container->Data.Front());
			//Assert::AreEqual(3, container->Data.Back());
			//Assert::AreEqual(std::size_t(0), container->Depth());
		}

		TEST_METHOD(GetParsedDataContainer)
		{
			std::string input = R"({ "integer": 101 })";

			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);
			std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			parseCoord.AddHelper(integerHelper);

			parseCoord.Parse(input);
			Assert::AreEqual(std::size_t(1), container->Data.Size());
			Assert::AreEqual(101, container->Data.Front());

			//JsonParseCoordinator::ParsedDataContainer* containerPtr = parseCoord.GetParsedDataContainer();
			JsonParseCoordinator::ParsedDataContainer* containerPtr = parseCoord.GetParsedDataContainer();
			Assert::IsTrue(container == containerPtr);
		}

		TEST_METHOD(SetParsedDataContainer)
		{
			std::string input = R"({ "integer": 101 })";

			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer;
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);
			std::shared_ptr<JsonIntegerParseHelper> integerHelper = std::make_shared<JsonIntegerParseHelper>();
			parseCoord.AddHelper(integerHelper);

			parseCoord.Parse(input);
			Assert::AreEqual(std::size_t(1), container->Data.Size());
			Assert::AreEqual(101, container->Data.Front());

			Assert::ExpectException<std::runtime_error>([&parseCoord]() { parseCoord.SetParsedDataContainer(nullptr); });

			JsonIntegerParseHelper::ParsedDataContainer parsedDataContainer2;
			auto container2 = &parsedDataContainer2;
			parseCoord.SetParsedDataContainer(container2);
			JsonParseCoordinator::ParsedDataContainer* containerPtr = parseCoord.GetParsedDataContainer();
			Assert::IsTrue(container2 == containerPtr);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonParseCoordinatorTests::_startMemState;
}
