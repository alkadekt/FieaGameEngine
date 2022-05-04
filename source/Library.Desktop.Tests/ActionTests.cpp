#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "ActionListWhile.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace LibraryDesktopTests
{
	TEST_CLASS(ActionTests)
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

		TEST_METHOD(ActionListWhileUpdate)
		{
			ActionListWhile list;

			const std::string aKey = "A";
			Datum& datum = list.AppendAuxiliaryAttribute(aKey);
			datum = 0.0f;

			auto nestedAction = list.CreateAction(ActionIncrement::TypeName(), "Increment Action"s);

			Assert::IsNotNull(nestedAction);
			auto actionIncrement = nestedAction->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual(std::size_t(1), list.Find("Actions")->Size());

			actionIncrement->SetTarget(aKey);

			Assert::AreEqual(aKey, actionIncrement->Target());
			Assert::AreEqual(1.0f, actionIncrement->Step());
			Assert::AreEqual(0.0f, datum.GetFloat());

			GameTime time;
			list.Update(time);
			Assert::AreEqual(0.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(0.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(0.0f, datum.GetFloat());

			list.SetCondition(true);
			list.Update(time);
			Assert::AreEqual(1.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(2.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(3.0f, datum.GetFloat());
		}

		TEST_METHOD(ActionIncrementUpdate)
		{
			ActionList list;

			const std::string aKey = "A";
			Datum& datum = list.AppendAuxiliaryAttribute(aKey);
			datum = 0.0f;

			auto nestedAction = list.CreateAction(ActionIncrement::TypeName(), "Increment Action"s);
			
			Assert::IsNotNull(nestedAction);
			auto actionIncrement = nestedAction->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual(std::size_t(1), list.Find("Actions")->Size());
			
			actionIncrement->SetTarget(aKey);
			
			Assert::AreEqual(aKey, actionIncrement->Target());
			Assert::AreEqual(1.0f, actionIncrement->Step());
			Assert::AreEqual(0.0f, datum.GetFloat());
			
			GameTime time;
			list.Update(time);
			Assert::AreEqual(1.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(2.0f, datum.GetFloat());
			list.Update(time);
			Assert::AreEqual(3.0f, datum.GetFloat());
		}

		TEST_METHOD(ActionCreateActionUpdate)
		{
			ActionList list;

			auto nestedAction = list.CreateAction(ActionCreateAction::TypeName(), "Create Action"s);

			Assert::IsNotNull(nestedAction);
			auto actionCreate = nestedAction->As<ActionCreateAction>();
			Assert::IsNotNull(actionCreate);
			Assert::AreEqual(std::size_t(1), list.Find("Actions")->Size());

			actionCreate->SetClassName("ActionIncrement");
			actionCreate->SetInstanceName("Increment Action");

			Assert::AreEqual("ActionIncrement"s, actionCreate->ClassName());
			Assert::AreEqual("Increment Action"s, actionCreate->InstanceName());

			GameTime time;
			list.Update(time);
			Assert::AreEqual(std::size_t(2), list.Find("Actions")->Size());
			Action* actionPtr = &list.GetAction(1);
			ActionIncrement* incrementAction = actionPtr->As<ActionIncrement>();
			Assert::IsNotNull(incrementAction);
			list.Update(time);
			Assert::AreEqual(std::size_t(3), list.Find("Actions")->Size());
		}

		TEST_METHOD(ActionDestroyActionUpdate)
		{
			ActionList list;

			auto nestedAction = list.CreateAction(ActionDestroyAction::TypeName(), "Destroy Action"s);
			auto nestedAction2 = list.CreateAction(ActionIncrement::TypeName(), "Increment Action"s);

			Assert::IsNotNull(nestedAction);
			auto actionDestroy = nestedAction->As<ActionDestroyAction>();
			Assert::IsNotNull(actionDestroy);
			Assert::AreEqual(std::size_t(2), list.Find("Actions")->Size());

			actionDestroy->SetClassName("ActionIncrement"s);
			actionDestroy->SetInstanceName("Increment Action"s);
			Assert::AreEqual("ActionIncrement"s, actionDestroy->ClassName());
			Assert::AreEqual("Increment Action"s, actionDestroy->InstanceName());

			Assert::IsNotNull(nestedAction2);
			auto actionIncrement = nestedAction2->As<ActionIncrement>();
			Assert::IsNotNull(actionIncrement);
			Assert::AreEqual(actionDestroy->InstanceName(), actionIncrement->Name());

			GameTime time;
			list.Update(time);
			Assert::AreEqual(std::size_t(1), list.Find("Actions")->Size());
			Action* actionPtr = &list.GetAction(0);
			ActionDestroyAction* destroyAction = actionPtr->As<ActionDestroyAction>();
			Assert::IsNotNull(destroyAction);
		}

		TEST_METHOD(ActionParse)
		{
			GameObject scope;
			JsonTableParseHelper::ParsedDataContainer container(&scope);
			JsonParseCoordinator parseCoord(&container);

			auto tableHelper = std::make_shared<JsonTableParseHelper>();
			parseCoord.AddHelper(tableHelper);

			parseCoord.ParseFromFile(R"(Content\inputAction.json)"s);

			Assert::AreEqual(std::size_t(0), container.Depth());

			Assert::AreEqual(std::size_t(8), scope.Size());
			Assert::IsNotNull(scope.Find("this"));
			Assert::IsNotNull(scope.Find("A"));
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

			Assert::AreEqual(Datum::DatumType::Table, scope.Find("Actions")->Type());
			Assert::AreEqual(std::size_t(1), scope.Find("Actions")->Size());
			Assert::IsNotNull(scope.Find("Actions")->GetTable(0).Find("Name"));
			Scope* action = &scope.Find("Actions")->GetTable(0);
			ActionIncrement* actionIncrement = action->As<ActionIncrement>();

			Assert::AreEqual("Increment A"s, actionIncrement->Name());
			Assert::AreEqual("A"s, actionIncrement->Target());
			Assert::AreEqual(2.5f, actionIncrement->Step());

			Datum* datum = scope.Find("A");
			Assert::AreEqual(Datum::DatumType::Float, scope.Find("A")->Type());
			Assert::AreEqual(0.0f, scope.Find("A")->FrontFloat());

			GameTime time;
			scope.Update(time);
			Assert::AreEqual(2.5f, datum->GetFloat());
			scope.Update(time);
			Assert::AreEqual(5.0f, datum->GetFloat());
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ActionTests::_startMemState;
}
