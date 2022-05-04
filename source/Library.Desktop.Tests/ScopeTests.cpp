#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Scope>(const Scope& d)
	{
		try
		{
			return ToString(d.ToString());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}

	template<>
	std::wstring ToString<Scope>(Scope* d)
	{
		try
		{
			return ToString((*d).ToString());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
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
			Scope scope;
			Assert::IsTrue(scope.GetParent() == nullptr);
		}

		TEST_METHOD(CopySemantics)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			glm::vec4 v = glm::vec4(1);

			Scope scope;
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Datum& bDatum = scope.Append(b);
			bDatum = v;
			Scope& cScope = scope.AppendScope(c);

			{
				Assert::AreEqual(std::size_t(3), scope.Size());
				Assert::IsTrue(scope[0] == "bruh"s);
				Assert::IsTrue(scope[1] == v);
				auto [datum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreSame(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}
			
			{
				Scope scopeCopy(scope);
				Assert::AreEqual(scope[0], scopeCopy[0]);
				Assert::AreEqual(scope[1], scopeCopy[1]);
				Assert::AreEqual(scope[2], scopeCopy[2]);
				Assert::AreEqual(scope, scopeCopy);
				Assert::AreEqual(std::size_t(3), scope.Size());
				Assert::IsTrue(scopeCopy[0] == "bruh"s);
				Assert::IsTrue(scopeCopy[1] == bDatum);
				Assert::AreEqual(cScope, scopeCopy[2].GetTable());
				auto [datum, datumIndex] = scopeCopy.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreEqual(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}

			{
				const std::string d = "D"s;
				const std::string e = "E"s;
				Scope otherScope;
				Datum& dDatum = otherScope.Append(d);
				dDatum = 5;
				otherScope.AppendScope(e);
				Assert::AreEqual(std::size_t(2), otherScope.Size());

				otherScope = scope;
				Assert::AreEqual(std::size_t(3), otherScope.Size());
				Assert::IsTrue(otherScope[0] == "bruh"s);
				Assert::IsTrue(otherScope[1] == bDatum);
				Assert::AreEqual(cScope, otherScope[2].GetTable());
				auto [datum, datumIndex] = otherScope.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreEqual(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			glm::vec4 v = glm::vec4(1);

			{
				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "bruh"s;
				Datum& bDatum = scope.Append(b);
				bDatum = v;
				Scope& cScope = scope.AppendScope(c);

				Assert::AreEqual(std::size_t(3), scope.Size());
				Assert::IsTrue(scope[0] == "bruh"s);
				Assert::IsTrue(scope[1] == v);
				auto [datum, datumIndex] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreSame(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}

			{
				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "bruh"s;
				Datum& bDatum = scope.Append(b);
				bDatum = v;
				Scope& cScope = scope.AppendScope(c);
				
				Scope scopeCopy(std::move(scope));
				Assert::AreEqual(std::size_t(3), scopeCopy.Size());
				Assert::IsTrue(scopeCopy[0] == "bruh"s);
				Assert::IsTrue(scopeCopy[1] == bDatum);
				Assert::AreEqual(cScope, scopeCopy[2].GetTable());
				auto [datum, datumIndex] = scopeCopy.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreEqual(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}

			{
				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "bruh"s;
				Datum& bDatum = scope.Append(b);
				bDatum = v;
				Scope& cScope = scope.AppendScope(c);
				
				const std::string d = "D"s;
				const std::string e = "E"s;
				Scope otherScope;
				Datum& dDatum = otherScope.Append(d);
				dDatum = 5;
				otherScope.AppendScope(e);
				Assert::AreEqual(std::size_t(2), otherScope.Size());
				
				otherScope = std::move(scope);
				Assert::AreEqual(std::size_t(3), otherScope.Size());
				Assert::IsTrue(otherScope[0] == "bruh"s);
				Assert::IsTrue(otherScope[1] == bDatum);
				Assert::AreEqual(cScope, otherScope[2].GetTable());
				auto [datum, datumIndex] = otherScope.FindContainedScope(cScope);
				Assert::IsNotNull(datum);
				Assert::AreEqual(cScope, datum->GetTable());
				Assert::AreEqual(std::size_t(0), datumIndex);
			}
		}

		TEST_METHOD(EquivalenceOperators)
		{
			{
				Scope scope;
				Scope scopeCopy(scope);
				Assert::AreEqual(scope, scopeCopy);
			}
			
			{
				const std::string a = "A"s;
				const std::string b = "B"s;
				const std::string c = "C"s;
				glm::vec4 v = glm::vec4(1);

				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "bruh"s;
				Datum& bDatum = scope.Append(b);
				bDatum = v;
				scope.AppendScope(c);

				Scope otherScope;
				Assert::IsTrue(scope != otherScope);
			}
		}

		TEST_METHOD(Size)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			const std::string d = "C"s;
			glm::vec4 v = glm::vec4(1);

			Scope scope;
			Assert::AreEqual(std::size_t(0), scope.Size());
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Assert::AreEqual(std::size_t(1), scope.Size());
			Datum& bDatum = scope.Append(b);
			bDatum = v;
			Assert::AreEqual(std::size_t(2), scope.Size());
			scope.AppendScope(c);
			Assert::AreEqual(std::size_t(3), scope.Size());
			scope.AppendScope(d);
			Assert::AreEqual(std::size_t(3), scope.Size());
		}

		TEST_METHOD(Clone)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			glm::vec4 v = glm::vec4(1);

			Scope scope;
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Datum& bDatum = scope.Append(b);
			bDatum = v;
			scope.AppendScope(c);

			Scope* clone = scope.Clone();
			Assert::AreEqual(std::size_t(3), clone->Size());
			delete clone;
		}

		TEST_METHOD(Find)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;

			Scope scope;
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Datum& bDatum = scope.Append(b);
			bDatum = glm::vec4(1);
			Assert::AreEqual(std::size_t(2), scope.Size());

			{
				Assert::AreEqual(aDatum, *scope.Find(a));
				Assert::AreEqual(bDatum, *scope.Find(b));
				Assert::IsNull(scope.Find(c));
			}

			{
				const Scope scopeCopy(scope);
				Assert::AreEqual(aDatum, *scopeCopy.Find(a));
				Assert::AreEqual(bDatum, *scopeCopy.Find(b));
				Assert::IsNull(scopeCopy.Find(c));
			}
		}

		TEST_METHOD(Search)
		{
			const std::string item = "Item"s;
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			const std::string d = "D"s;

			Scope scope;
			Datum& itemDatum = scope.Append(item);
			itemDatum = 5;
			Scope& aScope = scope.AppendScope(a);
			Scope& bScope = aScope.AppendScope(b);
			Scope& cScope = bScope.AppendScope(c);

			{
				Scope* output;
				Assert::IsNull(cScope.Search(d));
				Assert::IsNull(cScope.Search(d, output));
				Assert::AreEqual(itemDatum, *scope.Find(item));
				Assert::AreEqual(itemDatum, *cScope.Search(item));
			}

			{
				const Scope* output;
				Assert::IsNull(const_cast<const Scope&>(cScope).Search(d));
				Assert::IsNull(const_cast<const Scope&>(cScope).Search(d, output));
				Assert::AreEqual(itemDatum, *scope.Find(item));
				Assert::AreEqual(itemDatum, *const_cast<const Scope&>(cScope).Search(item));
			}
		}

		TEST_METHOD(Append)
		{
			const std::string a = "A"s;

			Scope scope;
			Datum& itemDatum = scope.Append(a);
			itemDatum = 1.4f;
			Assert::AreEqual(1.4f, scope[0].GetFloat());
			Datum& itemDatum2 = scope.Append(a);
			Assert::AreEqual(itemDatum, itemDatum2);
		}

		TEST_METHOD(AppendScope)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			Scope otherScope;
			Datum& itemDatum = otherScope.Append(b);
			itemDatum = 3.4f;

			Scope scope;
			Scope& itemScope = scope.AppendScope(a);
			itemScope = otherScope;
			Assert::AreEqual(itemScope, scope[0].GetTable());
			scope.AppendScope(a);
			Assert::AreEqual(std::size_t(1), scope.Size());
			Assert::AreNotEqual(scope[0].GetTable(0), scope[0].GetTable(1));
		}

		TEST_METHOD(Adopt)
		{
			{
				const std::string a = "A"s;
				const std::string b = "B"s;
				Scope scope;

				Scope otherScope;
				Scope& childScope = otherScope.AppendScope(a);

				scope.Adopt(childScope, b);
				Assert::AreEqual(std::size_t(1), scope.Size());
				Assert::AreEqual(childScope, scope[0].GetTable());
			}
			
			{
				const std::string a = "A"s;
				const std::string b = "B"s;
				Scope scope;
				scope.AppendScope(b);

				Scope otherScope;
				Scope& childScope = otherScope.AppendScope(a);

				scope.Adopt(childScope, b);
				Assert::AreEqual(std::size_t(1), scope.Size());
				Assert::AreEqual(childScope, scope[0].GetTable(1));
			}
		}

		TEST_METHOD(GetParent)
		{
			const std::string a = "A"s;
			Scope scope;
			Assert::IsNull(scope.GetParent());
			Scope& child = scope.AppendScope(a);
			Assert::AreEqual(&scope, child.GetParent());
		}

		TEST_METHOD(BracketOperators)
		{
			{
				const std::string a = "A"s;
				Scope scope;

				Datum& aDatum = scope.Append(a);
				aDatum = 3.3f;

				Assert::AreEqual(aDatum, scope[a]);
			}

			{
				const std::string a = "A"s;
				Scope scope;

				Datum& aDatum = scope.Append(a);
				aDatum = 3.3f;

				Assert::AreEqual(aDatum, scope[0]);
			}

			{
				const std::string a = "A"s;
				Scope scope;
				Scope* ptr = &scope;

				Datum& aDatum = scope.Append(a);
				aDatum = 3.3f;

				Assert::AreEqual(aDatum, (*const_cast<const Scope*>(ptr))[0]);
			}
		}

		TEST_METHOD(FindContainedScope)
		{
			const std::string a = "A"s;
			Scope scope;
			Scope& child = scope.AppendScope(a);

			Assert::AreEqual(&scope[0], scope.FindContainedScope(child).first);
		}

		TEST_METHOD(Clear)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			glm::vec4 v = glm::vec4(1);

			Scope scope;
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Datum& bDatum = scope.Append(b);
			bDatum = v;
			scope.AppendScope(c);
			Assert::AreEqual(std::size_t(3), scope.Size());

			scope.Clear();
			Assert::AreEqual(std::size_t(0), scope.Size());
			Assert::IsNull(scope.Find(a));
			Assert::IsNull(scope.Find(b));
			Assert::IsNull(scope.Find(c));
		}

		TEST_METHOD(Orphan)
		{
			const std::string a = "A"s;
			const std::string b = "B"s;
			const std::string c = "C"s;
			const std::string d = "D"s;
			const std::string e = "E"s;
			glm::vec4 v = glm::vec4(1);

			Scope otherScope;
			Scope& otherChild = otherScope.AppendScope(e);
			Datum& dDatum = otherChild.Append(d);
			dDatum = 5;

			Scope scope;
			Datum& aDatum = scope.Append(a);
			aDatum = "bruh"s;
			Datum& bDatum = scope.Append(b);
			bDatum = v;
			scope.Adopt(otherChild, c);
			Scope& child = scope.Find(c)->GetTable();
			Scope& child2 = scope.AppendScope(c);
			//Scope* childPtr = &child;
			//Scope* child2Ptr = &child2;
			Assert::AreEqual(std::size_t(3), scope.Size());
			Assert::AreEqual(std::size_t(2), scope[2].Size());

			scope.Orphan(child);
			delete &child;
			Assert::AreEqual(std::size_t(3), scope.Size());
			Assert::IsNull(scope.FindContainedScope(child).first);
			//Assert::IsNotNull(scope.FindContainedScope(child2).first);
			Assert::AreEqual(std::size_t(1), scope[2].Size());
			scope.Orphan(child2);
			Assert::AreEqual(std::size_t(3), scope.Size());
			Assert::IsNotNull(scope.Find(c));
			//delete &child2Ptr;
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState ScopeTests::_startMemState;
}
