#pragma once
#include "RTTI.h"
#include "Datum.h"
#include "Signature.h"
#include "Attributed.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class AttributedFoo : public Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)

	public:
		static const std::size_t ArraySize = 5;

		AttributedFoo();
		AttributedFoo(const AttributedFoo& other) = default;
		AttributedFoo& operator=(const AttributedFoo& other) = default;
		AttributedFoo(AttributedFoo&& other) noexcept = default;
		AttributedFoo& operator=(AttributedFoo&& other) noexcept = default;
		virtual ~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0;
		std::string ExternalString;
		glm::vec4 ExternalVector;
		glm::mat4 ExternalMatrix;

		int ExternalIntegerArray[ArraySize];
		float ExternalFloatArray[ArraySize];
		std::string ExternalStringArray[ArraySize];
		glm::vec4 ExternalVectorArray[ArraySize];
		glm::mat4 ExternalMatrixArray[ArraySize];

		gsl::owner<AttributedFoo*> Clone() const override;
		bool Equals(const RTTI* rhs) const override;
		std::string ToString() const override;

		static const Vector<Signature> Signatures();
	};

	ConcreteFactory(AttributedFoo, RTTI)
}