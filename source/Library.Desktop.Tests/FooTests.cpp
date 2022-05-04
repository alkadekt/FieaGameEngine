#include "pch.h"
#include "Foo.h"
#include "CppUnitTest.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;

namespace LibraryDesktopTests
{
	TEST_CLASS(FooTests)
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
			Foo foo{ 1 };
			Assert::AreEqual(foo.Data(), 1);
		}

		TEST_METHOD(CopyConstructor)
		{
			Foo a{ 1 };
			Foo b{ a };
			Assert::AreEqual(a.Data(), b.Data());
		}

		TEST_METHOD(AssignmentOperator)
		{
			Foo a{ 1 };
			Foo b = a;
			Assert::AreEqual(a.Data(), b.Data());
		}

		TEST_METHOD(EqualCompareOperator)
		{
			Foo a{ 1 };
			Foo b = a;
			Assert::IsTrue(a == b);
		}

		TEST_METHOD(UnequalCompareOperator)
		{
			Foo a{ 1 };
			Foo b{ 2 };
			Assert::IsTrue(a != b);
		}

		TEST_METHOD(Data)
		{
			Foo a{ 1 };
			Assert::AreEqual(a.Data(), 1);
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState FooTests::_startMemState;
}
