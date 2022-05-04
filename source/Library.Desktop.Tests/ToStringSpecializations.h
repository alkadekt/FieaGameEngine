#pragma once
#include "CppUnitTest.h"
#include "SList.h"
#include "Datum.h"
#include "RTTI.h"
#include "Foo.h"
#include <stdexcept>

using namespace FieaGameEngine;
using namespace std;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
#pragma warning(push)
#pragma warning(disable : 4505)

	template<> inline
	std::wstring ToString<Foo>(const Foo& foo)
	{
		RETURN_WIDE_STRING(foo.Data());
	}

	template<>
	std::wstring ToString<Datum::DatumType>(const Datum::DatumType& d)
	{
		try
		{
			switch (d)
			{
			case FieaGameEngine::Datum::DatumType::Unknown:
				return L"Unknown"s;
				break;
			case FieaGameEngine::Datum::DatumType::Integer:
				return L"Integer"s;
				break;
			case FieaGameEngine::Datum::DatumType::Float:
				return L"Float"s;
				break;
			case FieaGameEngine::Datum::DatumType::String:
				return L"String"s;
				break;
			case FieaGameEngine::Datum::DatumType::Vector:
				return L"Vector"s;
				break;
			case FieaGameEngine::Datum::DatumType::Matrix:
				return L"Matrix"s;
				break;
			case FieaGameEngine::Datum::DatumType::Pointer:
				return L"Pointer"s;
				break;
			case FieaGameEngine::Datum::DatumType::Table:
				return L"Table"s;
				break;
			default:
				return L"error"s;
				break;
			}
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}

	template<>
	std::wstring ToString<Datum>(const Datum& d)
	{
		try
		{
			return ToString(d.Type());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}

	template<>
	std::wstring ToString<Datum>(Datum* d)
	{
		try
		{
			return ToString((*d).Type());
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}

	template<>
	std::wstring ToString<glm::vec4>(const glm::vec4& d)
	{
		try
		{
			return L"bvec4("s + ToString(d.x) + L", "s + ToString(d.y) + L", "s + ToString(d.z) + L", "s + ToString(d.w) + L")"s;
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
	template<>
	std::wstring ToString<glm::mat4>(const glm::mat4& m)
	{
		try
		{
			return L"mat4x4(("s +
				ToString(m[0][0]) + L", "s + ToString(m[0][1]) + L", "s +
				ToString(m[0][2]) + L", "s + ToString(m[0][3]) + L"), ("s +
				ToString(m[1][0]) + L", "s + ToString(m[1][1]) + L", "s +
				ToString(m[1][2]) + L", "s + ToString(m[1][3]) + L"), ("s +
				ToString(m[2][0]) + L", "s + ToString(m[2][1]) + L", "s +
				ToString(m[2][2]) + L", "s + ToString(m[2][3]) + L"), ("s +
				ToString(m[3][0]) + L", "s + ToString(m[3][1]) + L", "s +
				ToString(m[3][2]) + L", "s + ToString(m[3][3]) + L"))"s;
		}
		catch (const std::exception&)
		{
			return L"error"s;
		}
	}
#pragma warning(pop)
}