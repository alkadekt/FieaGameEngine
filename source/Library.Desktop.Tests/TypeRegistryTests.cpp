#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "TypeRegistry.h"
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

	TEST_CLASS(TypeRegistryTests)
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

		TEST_METHOD(GetSignaturesOfType)
		{
			TypeRegistry& registry = TypeRegistry::GetInstance();
			const Vector<Signature>& signatures = registry.GetSignaturesOfType(AttributedFoo::TypeIdClass());
			Assert::AreEqual("ExternalInteger"s, signatures[0]._name);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState TypeRegistryTests::_startMemState;
}
