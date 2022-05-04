#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "GameObject.h"
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
	TEST_CLASS(GameObjectTests)
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
			GameObject gameObject;
			Assert::IsNotNull(gameObject.Find("this"));
			Assert::IsNotNull(gameObject.Find("Name"));
			Assert::IsNotNull(gameObject.Find("Position"));
			Assert::IsNotNull(gameObject.Find("Rotation"));
			Assert::IsNotNull(gameObject.Find("Scale"));
			Assert::IsNotNull(gameObject.Find("Children"));
		}

		TEST_METHOD(TableParseGameObject)
		{
			GameObject scope;
			JsonTableParseHelper::ParsedDataContainer container(&scope);
			JsonParseCoordinator parseCoord(&container);

			auto tableHelper = std::make_shared<JsonTableParseHelper>();
			parseCoord.AddHelper(tableHelper);

			parseCoord.ParseFromFile(R"(Content\inputGameObject.json)"s);

			Assert::AreEqual(std::size_t(0), container.Depth());

			Assert::AreEqual(std::size_t(7), scope.Size());
			Assert::IsNotNull(scope.Find("this"));
			Assert::IsNotNull(scope.Find("Name"));
			Assert::IsNotNull(scope.Find("Position"));
			Assert::IsNotNull(scope.Find("Rotation"));
			Assert::IsNotNull(scope.Find("Scale"));
			Assert::IsNotNull(scope.Find("Children"));
			Assert::IsNotNull(scope.Find("Actions"));

			Assert::AreEqual(Datum::DatumType::String, scope.Find("Name")->Type());
			Assert::AreEqual("Andrew"s, scope.Find("Name")->FrontString());
			Assert::AreEqual("Andrew"s, scope.Name);

			Assert::AreEqual(Datum::DatumType::Vector, scope.Find("Position")->Type());
			glm::vec4 a(1, 2, 3, 4);
			Assert::AreEqual(a, scope.Find("Position")->FrontVector());
			Assert::AreEqual(a, scope.Position);

			Assert::AreEqual(Datum::DatumType::Vector, scope.Find("Rotation")->Type());
			glm::vec4 b(2, 2, 3, 4);
			Assert::AreEqual(b, scope.Find("Rotation")->FrontVector());
			Assert::AreEqual(b, scope.Rotation);

			Assert::AreEqual(Datum::DatumType::Vector, scope.Find("Scale")->Type());
			glm::vec4 c(3, 2, 3, 4);
			Assert::AreEqual(c, scope.Find("Scale")->FrontVector());
			Assert::AreEqual(c, scope.Scale);

			Assert::AreEqual(Datum::DatumType::Table, scope.Find("Children")->Type());
			Assert::AreEqual(std::size_t(2), scope.Find("Children")->Size());

			Assert::IsNotNull(scope.Find("Children")->GetTable(0).Find("Name"));
			Assert::AreEqual("What"s, scope.GetChild(0).Name);
			Assert::IsNotNull(scope.Find("Children")->GetTable(1).Find("Name"));
			Assert::AreEqual("Bruh"s, scope.GetChild(1).Name);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState GameObjectTests::_startMemState;
}
