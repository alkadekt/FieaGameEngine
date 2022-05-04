#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "Foo.h"
#include <crtdbg.h>
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	
	template<>
	std::wstring ToString<RTTI>(RTTI* r)
	{
		try
		{
			return ToString(r->ToString());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
	template<>
	std::wstring ToString<RTTI>(const RTTI* r)
	{
		try
		{
			return ToString(r->ToString());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
	
}

namespace LibraryDesktopTests
{
	TEST_CLASS(DatumTests)
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
			Datum d;
			Assert::AreEqual(Datum::DatumType::Unknown, d.Type());
			Assert::AreEqual(std::size_t(0), d.Size());
			Assert::AreEqual(std::size_t(0), d.Capacity());
			Assert::IsFalse(d.IsExternal());
		}

		TEST_METHOD(CopyConstructor)
		{
			{
				Datum d1;
				d1.PushBackInteger(1);
				d1.PushBackInteger(2);
				d1.PushBackInteger(3);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2(d1);
				Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
			}
			
			{
				const std::string a = "wow"s;
				const std::string b = "woah"s;
				const std::string c = "whoop"s;
				Datum d1;
				d1.PushBackString(a);
				d1.PushBackString(b);
				d1.PushBackString(c);
				Assert::AreEqual(Datum::DatumType::String, d1.Type());
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2(d1);
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
			}	
				
			{	
				Datum d1;
				int random[] = { 1, 2, 3 };
				d1.SetStorageInteger(random, 3);
				Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
				Assert::IsTrue(d1.IsExternal());
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2(d1);
				Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
				Assert::IsTrue(d2.IsExternal());
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
			}
		}

		TEST_METHOD(AssignmentOperatorVectorDatum)
		{
			{
				Datum d1;
				d1.PushBackInteger(1);
				d1.PushBackInteger(2);
				d1.PushBackInteger(3);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2;
				Assert::ExpectException<std::runtime_error>([&d1, &d2]() { d1 = d2; });

				const std::string a = "wow"s;
				const std::string b = "woah"s;
				const std::string c = "whoop"s;
				const std::string d = "okthen"s;
				d2.PushBackString(a);
				d2.PushBackString(b);
				d2.PushBackString(c);
				d2.PushBackString(d);
				Assert::AreEqual(std::size_t(4), d2.Size());
				Assert::AreEqual(std::size_t(4), d2.Capacity());
				d1 = d2;
				Assert::AreEqual(Datum::DatumType::String, d1.Type());
				Assert::AreEqual(std::size_t(4), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
				Assert::IsFalse(d1.IsExternal());
			}
			
			{
				Datum d1;
				d1.PushBackFloat(1);
				d1.PushBackFloat(2);
				d1.PushBackFloat(3);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2;
				
				glm::vec4 a;
				glm::vec4 b;
				d2.PushBackVector(a);
				d2.PushBackVector(b);
				d1 = d2;
				Assert::AreEqual(Datum::DatumType::Vector, d1.Type());
				Assert::AreEqual(std::size_t(2), d1.Size());
				Assert::AreEqual(std::size_t(2), d1.Capacity());
				Assert::IsFalse(d1.IsExternal());
			}	

			{
				Datum d1;
				d1.PushBackFloat(1);
				d1.PushBackFloat(2);
				d1.PushBackFloat(3);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Datum d2;

				d2 = d1;
				Assert::AreEqual(Datum::DatumType::Float, d2.Type());
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Assert::IsFalse(d2.IsExternal());
			}
				
			{	
				Datum d1;
				int random[] = { 1, 2, 3 };
				d1.SetStorageInteger(random, 3);
				Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
				Assert::IsTrue(d1.IsExternal());
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				const std::string a = "wow"s;
				Datum d2;
				d2.PushBackString(a);
				Assert::IsFalse(d2.IsExternal());
				Assert::AreEqual(std::size_t(1), d2.Size());
				d2 = d1;
				Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
				Assert::IsTrue(d2.IsExternal());
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
			}
		}

		TEST_METHOD(AssignmentOperatorScalarDatum)
		{
			{
				Datum d1;
				d1.PushBackInteger(1);
				d1.PushBackInteger(2);
				d1.PushBackInteger(3);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Assert::ExpectException<std::runtime_error>([&d1]() { d1 = 0.2f; });
				Datum d2;
				int random[] = { 1, 2, 3 };
				d2.SetStorageInteger(random, 3);
				Assert::ExpectException<std::runtime_error>([&d2]() { d2 = 1; });
			}

			{
				Datum d1;
				d1 = 5;
				Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}

			{
				Datum d1;
				d1 = 5.0f;
				Assert::AreEqual(Datum::DatumType::Float, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}

			{
				Datum d1;
				d1 = "Bruh"s;
				Assert::AreEqual(Datum::DatumType::String, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}

			{
				Datum d1;
				glm::vec4 a;
				d1 = a;
				Assert::AreEqual(Datum::DatumType::Vector, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}

			{
				Datum d1;
				glm::mat4 a;
				d1 = a;
				Assert::AreEqual(Datum::DatumType::Matrix, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}

			{
				Foo f;
				RTTI* r = &f;
				Datum d1;
				d1 = r;
				Assert::AreEqual(Datum::DatumType::Pointer, d1.Type());
				Assert::AreEqual(std::size_t(1), d1.Size());
				Assert::AreEqual(std::size_t(1), d1.Capacity());
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			Datum d1;
			d1.PushBackInteger(1);
			d1.PushBackInteger(2);
			d1.PushBackInteger(3);
			Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
			Assert::AreEqual(std::size_t(3), d1.Size());
			Assert::AreEqual(std::size_t(3), d1.Capacity());
			Datum d2(std::move(d1));
			Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
			Assert::AreEqual(std::size_t(3), d2.Size());
			Assert::AreEqual(std::size_t(3), d2.Capacity());
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			Datum d1;
			d1.PushBackInteger(1);
			d1.PushBackInteger(2);
			d1.PushBackInteger(3);
			Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
			Assert::AreEqual(std::size_t(3), d1.Size());
			Assert::AreEqual(std::size_t(3), d1.Capacity());
			Datum d2;
			d2.PushBackFloat(1.0f);
			d2.PushBackFloat(2.0f);
			Assert::AreEqual(Datum::DatumType::Float, d2.Type());
			Assert::AreEqual(std::size_t(2), d2.Size());
			Assert::AreEqual(std::size_t(2), d2.Capacity());
			d1 = std::move(d2);
			Assert::AreEqual(Datum::DatumType::Float, d1.Type());
			Assert::AreEqual(std::size_t(2), d1.Size());
			Assert::AreEqual(std::size_t(2), d1.Capacity());
		}

		TEST_METHOD(EquivalenceOperators)
		{
			{
				Datum d1;
				d1.PushBackInteger(1);
				d1.PushBackInteger(3);
				Datum d2(d1);
				Assert::AreEqual(std::size_t(2), d1.Size());
				Assert::AreEqual(std::size_t(2), d1.Capacity());
				Assert::AreEqual(std::size_t(2), d2.Size());
				Assert::AreEqual(std::size_t(2), d2.Capacity());
				Assert::AreEqual(1, d1.FrontInteger());
				Assert::AreEqual(1, d2.FrontInteger());
				Assert::AreEqual(3, d1.BackInteger());
				Assert::AreEqual(3, d2.BackInteger());
				Assert::IsTrue(d1 == d2);
				Assert::IsFalse(d1 != d2);
			}

			{
				const std::string a = "wow"s;
				const std::string b = "woah"s;
				const std::string c = "whoop"s;
				const std::string d = "okthen"s;
				Datum d1;
				d1.PushBackString(a);
				d1.PushBackString(b);
				d1.PushBackString(c);
				Datum d2(d1);
				Assert::AreEqual("wow"s, d1.FrontString());
				Assert::AreEqual("wow"s, d2.FrontString());
				Assert::AreEqual("whoop"s, d1.BackString());
				Assert::AreEqual("whoop"s, d2.BackString());
				Assert::IsTrue(d1 == d2);
				Assert::IsFalse(d1 != d2);
				d2.PopBack();
				d2.PushBackString(d);
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
				Assert::AreEqual("wow"s, d1.FrontString());
				Assert::AreEqual("wow"s, d2.FrontString());
				Assert::IsFalse(d1 == d2);
				Assert::IsTrue(d1 != d2);
			}

			{
				Foo a = 1;
				Foo b = 2;
				RTTI* c = &a;
				RTTI* d = &b;
				Datum d1;
				d1.PushBackPointer(c);
				d1.PushBackPointer(d);
				Datum d2(d1);
				Assert::IsTrue(d1 == d2);
				Assert::IsFalse(d1 != d2);
				d2.PopBack();
				Assert::AreEqual(std::size_t(2), d1.Size());
				Assert::AreEqual(std::size_t(2), d1.Capacity());
				Assert::AreEqual(std::size_t(1), d2.Size());
				Assert::AreEqual(std::size_t(1), d2.Capacity());
				Assert::IsFalse(d1 == d2);
				Assert::IsTrue(d1 != d2);
			}

			{
				Datum d1, d2;
				int random[] = { 1, 2, 3 };
				d1.SetStorageInteger(random, 3);
				d2.SetStorageInteger(random, 3);
				Assert::AreEqual(Datum::DatumType::Integer, d1.Type());
				Assert::AreEqual(Datum::DatumType::Integer, d2.Type());
				Assert::IsTrue(d1.IsExternal());
				Assert::IsTrue(d2.IsExternal());
				Assert::AreEqual(std::size_t(3), d1.Size());
				Assert::AreEqual(std::size_t(3), d1.Capacity());
				Assert::AreEqual(std::size_t(3), d2.Size());
				Assert::AreEqual(std::size_t(3), d2.Capacity());
				Assert::IsTrue(d1 == d2);
				Assert::IsFalse(d1 != d2);
			}
		}

		TEST_METHOD(ScalarEquivalenceOperators)
		{
			{
				Datum d;
				d = 5;
				Assert::IsTrue(d == 5);
				Assert::IsFalse(d != 5);
			}
			{
				Datum d;
				d = 5.0f;
				Assert::IsTrue(d == 5.0f);
				Assert::IsFalse(d != 5.0f);
			}
			{
				Datum d;
				d = "hello"s;
				Assert::IsTrue(d == "hello"s);
				Assert::IsFalse(d != "hello"s);
			}
			{
				Datum d;
				glm::vec4 v;
				d = v;
				Assert::IsTrue(d == v);
				Assert::IsFalse(d != v);
			}
			{
				Datum d;
				glm::mat4 m;
				d = m;
				Assert::IsTrue(d == m);
				Assert::IsFalse(d != m);
			}
			{
				Datum d;
				Foo f;
				d = &f;
				Assert::IsTrue(d == &f);
				Assert::IsFalse(d != &f);
			}
		}

		TEST_METHOD(SetType)
		{
			Datum d;
			d.SetType(Datum::DatumType::Pointer);
			Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
			Assert::ExpectException<std::runtime_error>([&d] { d.SetType(Datum::DatumType::String); });
		}

		TEST_METHOD(Resize)
		{
			{
				Datum d1;
				int random[] = { 1, 2, 3 };
				d1.SetStorageInteger(random, 3);
				Assert::ExpectException<std::runtime_error>([&d1] { d1.Resize(5); });
				Datum d2;
				Assert::ExpectException<std::runtime_error>([&d2] { d2.Resize(5); });
				Datum d3;
				d3.PushBackInteger(1);
				d3.PushBackInteger(2);
				d3.Resize(0);
				Assert::AreEqual(std::size_t(0), d3.Size());
				Assert::AreEqual(std::size_t(0), d3.Capacity());
			}

			{
				const std::string a = "wow"s;
				const std::string b = "woah"s;
				const std::string c = "whoop"s;
				const std::string d = "okthen"s;
				Datum d1;
				d1.PushBackString(a);
				d1.PushBackString(b);
				d1.PushBackString(c);
				d1.PushBackString(d);
				Assert::AreEqual(std::size_t(4), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
				d1.Resize(2);
				Assert::AreEqual(std::size_t(2), d1.Size());
				Assert::AreEqual(std::size_t(2), d1.Capacity());
				d1.Resize(4);
				Assert::AreEqual(std::size_t(4), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
				d1.SetString(d, 3);
				Assert::AreEqual("wow"s, d1.FrontString());
				Assert::AreEqual("okthen"s, d1.BackString());
			}
		}

		TEST_METHOD(Clear)
		{
			{
				const std::string a = "wow"s;
				const std::string b = "woah"s;
				const std::string c = "whoop"s;
				const std::string d = "okthen"s;
				Datum d1;
				d1.PushBackString(a);
				d1.PushBackString(b);
				d1.PushBackString(c);
				d1.PushBackString(d);
				Assert::AreEqual(std::size_t(4), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
				d1.Clear();
				Assert::AreEqual(std::size_t(0), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
			}

			{
				Datum d1;
				d1.PushBackInteger(1);
				d1.PushBackInteger(2);
				d1.PushBackInteger(3);
				d1.PushBackInteger(4);
				Assert::AreEqual(std::size_t(4), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
				d1.Clear();
				Assert::AreEqual(std::size_t(0), d1.Size());
				Assert::AreEqual(std::size_t(4), d1.Capacity());
			}			
		}

		TEST_METHOD(SetStorage)
		{
			{
				int* badIdea = nullptr;
				int random[] = { 1, 2, 3 };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStorageInteger(badIdea, 3); });
				d.SetStorageInteger(random, 3);
				Assert::AreEqual(Datum::DatumType::Integer, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}

			{
				float* badIdea = nullptr;
				float random[] = { 1.0f, 2.0f, 3.0f };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStorageFloat(badIdea, 3); });
				d.SetStorageFloat(random, 3);
				Assert::AreEqual(Datum::DatumType::Float, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}

			{
				std::string* badIdea = nullptr;
				std::string random[] = { "bruh"s, "just"s, "stop"s };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStorageString(badIdea, 3); });
				d.SetStorageString(random, 3);
				Assert::AreEqual(Datum::DatumType::String, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}

			{
				glm::vec4* badIdea = nullptr;
				glm::vec4 a(1, 2, 3, 4);
				glm::vec4 b(1, 2, 3, 4);
				glm::vec4 c(1, 2, 3, 4);
				glm::vec4 random[] = { a, b, c };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStorageVector(badIdea, 3); });
				d.SetStorageVector(random, 3);
				Assert::AreEqual(Datum::DatumType::Vector, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}

			{
				glm::mat4* badIdea = nullptr;
				glm::mat4 a(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				glm::mat4 b(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				glm::mat4 c(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				glm::mat4 random[] = { a, b, c };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStorageMatrix(badIdea, 3); });
				d.SetStorageMatrix(random, 3);
				Assert::AreEqual(Datum::DatumType::Matrix, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}

			{
				RTTI** badIdea = nullptr;
				Foo a;
				Foo b;
				Foo c;
				RTTI* random[] = { &a, &b, &c };
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d, &badIdea] { d.SetStoragePointer(badIdea, 3); });
				d.SetStoragePointer(random, 3);
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::AreEqual(std::size_t(3), d.Size());
				Assert::AreEqual(std::size_t(3), d.Capacity());
				Assert::IsTrue(d.IsExternal());
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum d;
				d.PushBackInteger(1);
				Assert::ExpectException <std::runtime_error>([&d] { d.SetInteger(0, 3); });
				d.SetInteger(0, 0);
				Assert::AreEqual(0, d.FrontInteger());
			}

			{
				Datum d;
				d.PushBackFloat(1.0f);
				Assert::ExpectException <std::runtime_error>([&d] { d.SetFloat(0.0f, 3); });
				d.SetFloat(0.0f, 0);
				Assert::AreEqual(0.0f, d.FrontFloat());
			}

			{
				Datum d;
				d.PushBackString("hello"s);
				Assert::ExpectException <std::runtime_error>([&d] { d.SetString("bruh"s, 3); });
				d.SetString("bruh"s, 0);
				Assert::AreEqual("bruh"s, d.FrontString());
			}

			{
				Datum d;
				glm::vec4 v(1, 2, 3, 4);
				glm::vec4 b(1, 3, 3, 3);
				d.PushBackVector(v);
				Assert::ExpectException <std::runtime_error>([&d, &b] { d.SetVector(b, 3); });
				d.SetVector(b, 0);
				Assert::AreEqual(b, d.FrontVector());
			}

			{
				Datum d;
				glm::mat4 v(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				glm::mat4 b(2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				d.PushBackMatrix(v);
				Assert::ExpectException <std::runtime_error>([&d, &b] { d.SetMatrix(b, 3); });
				d.SetMatrix(b, 0);
				Assert::AreEqual(b, d.FrontMatrix());
			}

			{
				Datum d;
				Foo a;
				Foo b;
				RTTI* c = &a;
				RTTI* e = &b;
				d.PushBackPointer(c);
				Assert::ExpectException <std::runtime_error>([&d, &e] { d.SetPointer(e, 3); });
				d.SetPointer(e, 0);
				Assert::AreEqual(e, d.FrontPointer());
			}
		}

		TEST_METHOD(SetFromString)
		{
			{
				Datum d;
				Foo a;
				RTTI* b = &a;
				Assert::ExpectException <std::runtime_error>([&d] { d.SetFromString("3"s); });
				d.PushBackPointer(b);
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::ExpectException <std::runtime_error>([&d] { d.SetFromString("3"s); });
			}

			{
				Datum d;
				d.PushBackInteger(1);
				d.SetFromString("5"s);
				Assert::AreEqual(5, d.FrontInteger());
			}

			{
				Datum d;
				d.PushBackFloat(1.0f);
				d.SetFromString("5.0"s);
				Assert::AreEqual(5.0f, d.FrontFloat());
			}

			{
				Datum d;
				d.PushBackString("hello"s);
				d.SetFromString("bruh"s);
				Assert::AreEqual("bruh"s, d.FrontString());
			}

			{
				Datum d;
				glm::vec4 v(1, 2, 3, 4);
				glm::vec4 b(1, 3, 3, 4);
				d.PushBackVector(v);
				d.SetFromString("bvec4(1, 3, 3, 4)");
				Assert::AreEqual(b, d.FrontVector());
			}

			{
				Datum d;
				glm::mat4 m(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				glm::mat4 b(2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				d.PushBackMatrix(m);
				d.SetFromString("mat4x4((2, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1))");
				Assert::AreEqual(b, d.FrontMatrix());
			}
		}

		TEST_METHOD(ToString)
		{
			{
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d] { d.ToString(); });
			}

			{
				Datum d;
				d.PushBackInteger(1);
				Assert::AreEqual("1"s, d.ToString());
			}

			{
				Datum d;
				d.PushBackFloat(1.0f);
				Assert::AreEqual("1.000000"s, d.ToString());
			}

			{
				Datum d;
				d.PushBackString("wat"s);
				Assert::AreEqual("wat"s, d.ToString());
			}

			{
				Datum d;
				glm::vec4 v(1, 2, 3, 4);
				d.PushBackVector(v);
				Assert::AreEqual("bvec4(1.000000, 2.000000, 3.000000, 4.000000)"s, d.ToString());
			}

			{
				Datum d;
				glm::mat4 v(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				d.PushBackMatrix(v);
				Assert::AreEqual("mat4x4((1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000), (1.000000, 1.000000, 1.000000, 1.000000))"s, d.ToString());
			}

			{
				Datum d;
				Foo f;
				RTTI* rf = &f;
				RTTI* r = nullptr;
				d.PushBackPointer(r);
				Assert::AreEqual("nullptr"s, d.ToString());
				d.PopBack();
				d.PushBackPointer(rf);
				Assert::AreEqual("RTTI"s, d.ToString());
			}
		}

		TEST_METHOD(Get)
		{
			{
				Datum d;
				Assert::ExpectException<std::runtime_error>([&d] { d.GetInteger(); });
				d.PushBackInteger(1);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetInteger(1); });
				Assert::AreEqual(1, d.GetInteger());
			}

			{
				Datum d;
				Assert::ExpectException<std::runtime_error>([&d] { d.GetFloat(); });
				d.PushBackFloat(1.0f);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetFloat(1); });
				Assert::AreEqual(1.0f, d.GetFloat());
			}

			{
				Datum d;
				Assert::ExpectException<std::runtime_error>([&d] { d.GetString(); });
				d.PushBackString("Hello"s);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetString(1); });
				Assert::AreEqual("Hello"s, d.GetString());
			}

			{
				Datum d;
				glm::vec4 v(1, 2, 3, 4);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetVector(); });
				d.PushBackVector(v);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetVector(1); });
				Assert::AreEqual(v, d.GetVector());
			}

			{
				Datum d;
				glm::mat4 m(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetMatrix(); });
				d.PushBackMatrix(m);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetMatrix(1); });
				Assert::AreEqual(m, d.GetMatrix());
			}

			{
				Datum d;
				Foo f;
				RTTI* r = &f;
				Assert::ExpectException<std::runtime_error>([&d] { d.GetPointer(); });
				d.PushBackPointer(r);
				Assert::ExpectException<std::runtime_error>([&d] { d.GetPointer(1); });
				Assert::AreEqual(r, d.GetPointer());
			}
		}

		TEST_METHOD(PushBackFromString)
		{
			{
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d] { d.PushBackFromString("3"s); });
				d.SetType(Datum::DatumType::Pointer);
				Assert::AreEqual(Datum::DatumType::Pointer, d.Type());
				Assert::ExpectException <std::runtime_error>([&d] { d.PushBackFromString("3"s); });
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Integer);
				d.PushBackFromString("5"s);
				Assert::AreEqual(5, d.FrontInteger());
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::Float);
				d.PushBackFromString("5.0"s);
				Assert::AreEqual(5.0f, d.FrontFloat());
			}

			{
				Datum d;
				d.SetType(Datum::DatumType::String);
				d.PushBackFromString("bruh"s);
				Assert::AreEqual("bruh"s, d.FrontString());
			}

			{
				Datum d;
				glm::vec4 b(1, 3, 3, 4);
				d.SetType(Datum::DatumType::Vector);
				d.PushBackFromString("bvec4(1, 3, 3, 4)");
				Assert::AreEqual(b, d.FrontVector());
			}

			{
				Datum d;
				glm::mat4 b(2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				d.SetType(Datum::DatumType::Matrix);
				d.PushBackFromString("mat4x4((2, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1), (1, 1, 1, 1))");
				Assert::AreEqual(b, d.FrontMatrix());
			}
		}

		TEST_METHOD(Front)
		{
			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontInteger(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontInteger(); });
				Datum d;
				d.SetType(Datum::DatumType::Integer);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontInteger(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontInteger(); });
				d.PushBackInteger(1);
				Assert::AreEqual(1, d.FrontInteger());
				const Datum d2 = d;
				Assert::AreEqual(1, d2.FrontInteger());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Integer);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontFloat(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontFloat(); });
				Datum d;
				d.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontFloat(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontFloat(); });
				d.PushBackFloat(1.0f);
				Assert::AreEqual(1.0f, d.FrontFloat());
				const Datum d2 = d;
				Assert::AreEqual(1.0f, d2.FrontFloat());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontString(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontString(); });
				Datum d;
				d.SetType(Datum::DatumType::String);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontString(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontString(); });
				d.PushBackString("Hello"s);
				Assert::AreEqual("Hello"s, d.FrontString());
				const Datum d2 = d;
				Assert::AreEqual("Hello"s, d2.FrontString());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontVector(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontVector(); });
				Datum d;
				d.SetType(Datum::DatumType::Vector);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontVector(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontVector(); });
				glm::vec4 v(1, 2, 3, 4);
				d.PushBackVector(v);
				Assert::AreEqual(v, d.FrontVector());
				const Datum d2 = d;
				Assert::AreEqual(v, d2.FrontVector());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontMatrix(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontMatrix(); });
				Datum d;
				d.SetType(Datum::DatumType::Matrix);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontMatrix(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontMatrix(); });
				glm::mat4 m(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
				d.PushBackMatrix(m);
				Assert::AreEqual(m, d.FrontMatrix());
				const Datum d2 = d;
				Assert::AreEqual(m, d2.FrontMatrix());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.FrontPointer(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.FrontPointer(); });
				Datum d;
				d.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException <std::runtime_error>([&d] { d.FrontPointer(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.FrontPointer(); });
				Foo f;
				RTTI* a = &f;
				const RTTI* e = a;
				d.PushBackPointer(a);
				Assert::AreEqual(a, d.FrontPointer());
				const Datum d2 = d;
				Assert::AreEqual(e, d2.FrontPointer());
			}
		}

		TEST_METHOD(Back)
		{
			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackInteger(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackInteger(); });
				Datum d;
				d.SetType(Datum::DatumType::Integer);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackInteger(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackInteger(); });
				d.PushBackInteger(1);
				Assert::AreEqual(1, d.BackInteger());
				const Datum d2 = d;
				Assert::AreEqual(1, d2.BackInteger());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Integer);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackFloat(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackFloat(); });
				Datum d;
				d.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackFloat(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackFloat(); });
				d.PushBackFloat(1.0f);
				Assert::AreEqual(1.0f, d.BackFloat());
				const Datum d2 = d;
				Assert::AreEqual(1.0f, d2.BackFloat());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackString(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackString(); });
				Datum d;
				d.SetType(Datum::DatumType::String);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackString(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackString(); });
				d.PushBackString("Hello"s);
				Assert::AreEqual("Hello"s, d.BackString());
				const Datum d2 = d;
				Assert::AreEqual("Hello"s, d2.BackString());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackVector(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackVector(); });
				Datum d;
				d.SetType(Datum::DatumType::Vector);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackVector(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackVector(); });
				glm::vec4 v(1, 2, 3, 4);
				d.PushBackVector(v);
				Assert::AreEqual(v, d.BackVector());
				const Datum d2 = d;
				Assert::AreEqual(v, d2.BackVector());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackMatrix(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackMatrix(); });
				Datum d;
				d.SetType(Datum::DatumType::Matrix);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackMatrix(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackMatrix(); });
				glm::mat4 m(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
				d.PushBackMatrix(m);
				Assert::AreEqual(m, d.BackMatrix());
				const Datum d2 = d;
				Assert::AreEqual(m, d2.BackMatrix());
			}

			{
				Datum b;
				b.SetType(Datum::DatumType::Float);
				Assert::ExpectException <std::runtime_error>([&b] { b.BackPointer(); });
				const Datum d3(b);
				Assert::ExpectException <std::runtime_error>([&d3] { d3.BackPointer(); });
				Datum d;
				d.SetType(Datum::DatumType::Pointer);
				Assert::ExpectException <std::runtime_error>([&d] { d.BackPointer(); });
				const Datum d4(d);
				Assert::ExpectException <std::runtime_error>([&d4] { d4.BackPointer(); });
				Foo f;
				RTTI* a = &f;
				const RTTI* e = a;
				d.PushBackPointer(a);
				Assert::AreEqual(a, d.BackPointer());
				const Datum d2 = d;
				Assert::AreEqual(e, d2.BackPointer());
			}
		}

		TEST_METHOD(Remove)
		{
			{
				Datum e;
				e.PushBackFloat(1);
				Assert::ExpectException <std::runtime_error>([&e] { e.RemoveInteger(1); });
				Datum d;
				d.PushBackInteger(1);
				Assert::IsTrue(d.RemoveInteger(1));
				Assert::IsFalse(d.RemoveInteger(1));
				Assert::AreEqual(std::size_t(0), d.Size());
			}

			{
				Datum e;
				e.PushBackInteger(1);
				Assert::ExpectException <std::runtime_error>([&e] { e.RemoveFloat(1); });
				Datum d;
				d.PushBackFloat(1);
				Assert::IsTrue(d.RemoveFloat(1));
				Assert::IsFalse(d.RemoveFloat(1));
				Assert::AreEqual(std::size_t(0), d.Size());
			}

			{
				Datum e;
				e.PushBackFloat(1);
				Assert::ExpectException <std::runtime_error>([&e] { e.RemoveString("Hello"s); });
				Datum d;
				d.PushBackString("Hello"s);
				Assert::IsTrue(d.RemoveString("Hello"s));
				Assert::IsFalse(d.RemoveString("Hello"s));
				Assert::AreEqual(std::size_t(0), d.Size());
			}

			{
				Datum e;
				glm::vec4 v(1, 2, 3, 4);
				e.PushBackFloat(1);
				Assert::ExpectException <std::runtime_error>([&e, &v] { e.RemoveVector(v); });
				Datum d;
				d.PushBackVector(v);
				Assert::IsTrue(d.RemoveVector(v));
				Assert::IsFalse(d.RemoveVector(v));
				Assert::AreEqual(std::size_t(0), d.Size());
			}

			{
				Datum e;
				glm::mat4 m(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
				e.PushBackFloat(1);
				Assert::ExpectException <std::runtime_error>([&e, &m] { e.RemoveMatrix(m); });
				Datum d;
				d.PushBackMatrix(m);
				Assert::IsTrue(d.RemoveMatrix(m));
				Assert::IsFalse(d.RemoveMatrix(m));
				Assert::AreEqual(std::size_t(0), d.Size());
			}

			{
				Datum e;
				Foo f;
				RTTI* r = &f;
				e.PushBackFloat(1);
				Assert::ExpectException <std::runtime_error>([&e, &r] { e.RemovePointer(r); });
				Datum d;
				d.PushBackPointer(r);
				Assert::IsTrue(d.RemovePointer(r));
				Assert::IsFalse(d.RemovePointer(r));
				Assert::AreEqual(std::size_t(0), d.Size());
			}
		}

		TEST_METHOD(RemoveAt)
		{
			Datum e;
			Assert::ExpectException <std::runtime_error>([&e] { e.RemoveAt(0); });
			Datum d;
			d.PushBackString("bruh"s);
			Assert::ExpectException <std::runtime_error>([&d] { d.RemoveAt(1); });
			d.PushBackString("woah"s);
			d.PushBackString("wow"s);
			d.RemoveAt(0);
			Assert::AreEqual("woah"s, d.FrontString());
			Datum c;
			c.PushBackInteger(1);
			c.PushBackInteger(2);
			c.PushBackInteger(3);
			c.RemoveAt(0);
			Assert::AreEqual(std::size_t(2), c.Size());
			Assert::AreEqual(2, c.FrontInteger());
			Assert::AreEqual(3, c.BackInteger());
		}

		TEST_METHOD(Find)
		{
			{
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d] { d.FindInteger(1); });
			}
			
			{
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d] { d.FindFloat(1.0f); });
			}

			{
				Datum d;
				Assert::ExpectException <std::runtime_error>([&d] { d.FindString("wassup"s); });
			}

			{
				Datum d;
				glm::vec4 v(1, 2, 3, 4);
				Assert::ExpectException <std::runtime_error>([&d, &v] { d.FindVector(v); });
			}

			{
				Datum d;
				glm::mat4 m(1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4);
				Assert::ExpectException <std::runtime_error>([&d, &m] { d.FindMatrix(m); });
			}

			{
				Datum d;
				Foo f;
				RTTI* a = &f;
				Assert::ExpectException <std::runtime_error>([&d, &a] { d.FindPointer(a); });
			}
		}

	private:
		static _CrtMemState _startMemState;
	};

	_CrtMemState DatumTests::_startMemState;
}
