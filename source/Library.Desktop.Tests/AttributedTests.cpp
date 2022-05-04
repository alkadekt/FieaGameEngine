#include "pch.h"
#include "CppUnitTest.h"
#include "Attributed.h"
#include "AttributedFoo.h"
#include "TypeRegistry.h"
#include "GameObject.h"
#include "ActionListWhile.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionIncrement.h"
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
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeRegistry& registry = TypeRegistry::GetInstance();
		registry.RegisterSignaturesOfType<AttributedFoo>();
		registry.RegisterSignaturesOfType<GameObject>();
		registry.RegisterSignaturesOfType<Action>();
		registry.RegisterSignaturesOfType<ActionList>();
		registry.RegisterSignaturesOfType<ActionListWhile>();
		registry.RegisterSignaturesOfType<ActionCreateAction>();
		registry.RegisterSignaturesOfType<ActionDestroyAction>();
		registry.RegisterSignaturesOfType<ActionIncrement>();

		Factory<RTTI>::Add(std::make_shared<GameObjectFactory>());
		Factory<RTTI>::Add(std::make_shared<ActionListFactory>());
		Factory<RTTI>::Add(std::make_shared<ActionListWhileFactory>());
		Factory<RTTI>::Add(std::make_shared<ActionIncrementFactory>());
		Factory<RTTI>::Add(std::make_shared<ActionCreateActionFactory>());
		Factory<RTTI>::Add(std::make_shared<ActionDestroyActionFactory>());
	}

	TEST_CLASS(AttributedTests)
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
			AttributedFoo foo;
			Assert::AreEqual(std::size_t(13), foo.Size());
		}

		TEST_METHOD(CopySemantics)
		{
			{
				AttributedFoo foo;
				Assert::AreEqual(std::size_t(13), foo.Size());

				foo.AppendAuxiliaryAttribute("bruh");
				Assert::AreEqual(std::size_t(14), foo.Size());

				AttributedFoo otherFoo(foo);
				otherFoo;
				Assert::AreEqual(std::size_t(14), otherFoo.Size());
				Assert::IsTrue(foo.Equals(&otherFoo));
			}

			{
				AttributedFoo foo;
				Assert::AreEqual(std::size_t(13), foo.Size());
				
				foo.AppendAuxiliaryAttribute("bruh");
				Assert::AreEqual(std::size_t(14), foo.Size());
				
				AttributedFoo otherFoo;
				otherFoo.AppendAuxiliaryAttribute("what");
				Assert::AreEqual(std::size_t(14), otherFoo.Size());

				otherFoo = foo;
				Assert::IsTrue(otherFoo.Equals(&foo));
				Assert::AreEqual(foo.Size(), otherFoo.Size());
				Assert::IsNull(otherFoo.Find("what"));
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			{
				AttributedFoo foo;
				Assert::AreEqual(std::size_t(13), foo.Size());
				
				foo.AppendAuxiliaryAttribute("bruh");
				Assert::AreEqual(std::size_t(14), foo.Size());
				
				AttributedFoo otherFoo(std::move(foo));
				Assert::AreEqual(std::size_t(14), otherFoo.Size());
			}

			{
				AttributedFoo foo;
				Assert::AreEqual(std::size_t(13), foo.Size());
				
				foo.AppendAuxiliaryAttribute("bruh");
				Assert::AreEqual(std::size_t(14), foo.Size());
				
				AttributedFoo otherFoo;
				otherFoo.AppendAuxiliaryAttribute("what");
				Assert::AreEqual(std::size_t(14), otherFoo.Size());
				
				otherFoo = std::move(foo);
				Assert::IsNull(otherFoo.Find("what"));
			}
		}

		TEST_METHOD(AttributeChecks)
		{
			AttributedFoo foo;

			Assert::IsTrue(foo.IsAttribute("this"));
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsAttribute("NestedScope"));
			Assert::IsTrue(foo.IsAttribute("NestedScopeArray"));
			
			Assert::IsTrue(foo.IsPrescribedAttribute("this"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("NestedScopeArray"));

			Assert::IsFalse(foo.IsAuxiliaryAttribute("this"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("NestedScopeArray"));

			foo.AppendAuxiliaryAttribute("bruh");
			Assert::IsTrue(foo.IsAttribute("bruh"));
			Assert::IsFalse(foo.IsPrescribedAttribute("bruh"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("bruh"));
		}

		TEST_METHOD(AppendAuxiliaryAttribute)
		{
			AttributedFoo foo;
			
			Assert::ExpectException<std::runtime_error>([&foo]() { foo.AppendAuxiliaryAttribute("ExternalInteger"); });

			foo.AppendAuxiliaryAttribute("bruh");
			Assert::IsTrue(foo.IsAttribute("bruh"));
			Assert::IsFalse(foo.IsPrescribedAttribute("bruh"));
			Assert::IsTrue(foo.IsAuxiliaryAttribute("bruh"));
		}

		TEST_METHOD(Clone)
		{
			AttributedFoo foo;

			RTTI* newFoo = foo.Clone();
			Assert::IsTrue(foo.Equals(newFoo));
			delete newFoo;
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState AttributedTests::_startMemState;
}
