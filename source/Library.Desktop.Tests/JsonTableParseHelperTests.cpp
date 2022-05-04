#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseHelper.h"
#include "JsonTableParseHelper.h"
#include "JsonParseCoordinator.h"
#include "TypeRegistry.h"
#include <glm\glm.hpp>
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
}

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonTableParseHelperTests)
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

		TEST_METHOD(ParseFromFile)
		{
			Scope myScope;
			auto scope = &myScope;
			JsonTableParseHelper::ParsedDataContainer parsedDataContainer(scope);
			auto container = &parsedDataContainer;
			JsonParseCoordinator parseCoord(container);

			auto tableHelper = std::make_shared<JsonTableParseHelper>();
			parseCoord.AddHelper(tableHelper);

			parseCoord.ParseFromFile(R"(Content\input.json)"s);

			Assert::IsNotNull(scope->Find("favoritenumbers"));
			Assert::AreEqual(Datum::DatumType::Integer, scope->Find("favoritenumbers")->Type());
			Assert::AreEqual(7, scope->Find("favoritenumbers")->FrontInteger());
			Assert::AreEqual(777, scope->Find("favoritenumbers")->BackInteger());

			Assert::IsNotNull(scope->Find("name"));
			Assert::AreEqual(Datum::DatumType::String, scope->Find("name")->Type());
			Assert::AreEqual("Andrew"s, scope->Find("name")->FrontString());
			
			Assert::IsNotNull(scope->Find("nestedobjects"));
			Assert::AreEqual(Datum::DatumType::Table, scope->Find("nestedobjects")->Type());
			Assert::IsNotNull(scope->Find("nestedobjects")->GetTable().Find("powercount"));
			Assert::IsNotNull(scope->Find("nestedobjects")->GetTable().Find("powerlevel"));
			
			Assert::IsNotNull(scope->Find("transform"));
			Assert::AreEqual(Datum::DatumType::Vector, scope->Find("transform")->Type());
			Assert::AreEqual(glm::vec4(1, 2, 3, 4), scope->Find("transform")->GetVector());
			
			Assert::IsNotNull(scope->Find("velocity"));
			Assert::AreEqual(Datum::DatumType::Vector, scope->Find("velocity")->Type());
			Assert::AreEqual(glm::vec4(1, 2, 3, 4), scope->Find("velocity")->GetVector());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState JsonTableParseHelperTests::_startMemState;
}
