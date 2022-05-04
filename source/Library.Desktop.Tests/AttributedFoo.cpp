#include "pch.h"
#include "AttributedFoo.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	const Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumType::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat", Datum::DatumType::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", Datum::DatumType::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", Datum::DatumType::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", Datum::DatumType::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", Datum::DatumType::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", Datum::DatumType::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", Datum::DatumType::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", Datum::DatumType::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
			{ "NestedScope", Datum::DatumType::Table, 0, 0 },
			{ "NestedScopeArray", Datum::DatumType::Table, ArraySize, 0 }
		};
	}

	gsl::owner<AttributedFoo*> AttributedFoo::Clone() const
	{
		return new AttributedFoo(*this);
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const auto rhsAttributedFoo = rhs->As<AttributedFoo>();
		if (rhsAttributedFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsAttributedFoo->ExternalInteger &&
			ExternalFloat == rhsAttributedFoo->ExternalFloat &&
			ExternalString == rhsAttributedFoo->ExternalString &&
			ExternalVector == rhsAttributedFoo->ExternalVector &&
			ExternalMatrix == rhsAttributedFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsAttributedFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalFloatArray), std::end(ExternalFloatArray), std::begin(rhsAttributedFoo->ExternalFloatArray)) &&
			std::equal(std::begin(ExternalStringArray), std::end(ExternalStringArray), std::begin(rhsAttributedFoo->ExternalStringArray)) &&
			std::equal(std::begin(ExternalVectorArray), std::end(ExternalVectorArray), std::begin(rhsAttributedFoo->ExternalVectorArray)) &&
			std::equal(std::begin(ExternalMatrixArray), std::end(ExternalMatrixArray), std::begin(rhsAttributedFoo->ExternalMatrixArray)) &&
			(*this)["NestedScope"s] == (*rhsAttributedFoo)["NestedScope"s];
	}

	std::string AttributedFoo::ToString() const
	{
		return "AttributedFoo"s;
	}

}