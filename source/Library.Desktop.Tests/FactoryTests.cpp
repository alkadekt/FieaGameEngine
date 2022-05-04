#include "pch.h"
#include "CppUnitTest.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"
#include "Foo.h"
#include "AttributedFoo.h"
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
	TEST_CLASS(FactoryTests)
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
		
		TEST_METHOD(CreateFoo)
		{
			RTTI* rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
			
			Factory<RTTI>::Add(std::make_shared<FooFactory>());
			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNotNull(rtti);
			
			Foo* foo = rtti->As<Foo>();
			Assert::IsNotNull(foo);

			delete foo;
			
			Factory<RTTI>::Remove(std::make_shared<FooFactory>());
			rtti = Factory<RTTI>::Create("Foo"s);
			Assert::IsNull(rtti);
		}

		TEST_METHOD(CreateGameObjects)
		{
			//RTTI* rtti = Factory<RTTI>::Create("GameObject"s);
			//Assert::IsNull(rtti);

			RTTI* rtti = Factory<RTTI>::Create("GameObject"s);
			Assert::IsNotNull(rtti);

			GameObject* foo = rtti->As<GameObject>();
			Assert::IsNotNull(foo);

			Assert::IsNotNull(foo->Find("Name"));
			Assert::IsNotNull(foo->Find("Position"));
			Assert::IsNotNull(foo->Find("Rotation"));
			Assert::IsNotNull(foo->Find("Scale"));
			Assert::IsNotNull(foo->Find("Children"));

			delete foo;

			Factory<RTTI>::Remove(std::make_shared<GameObjectFactory>());
			rtti = Factory<RTTI>::Create("GameObject"s);
			Assert::IsNull(rtti);

			Factory<RTTI>::Add(std::make_shared<GameObjectFactory>());

		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FactoryTests::_startMemState;
}
