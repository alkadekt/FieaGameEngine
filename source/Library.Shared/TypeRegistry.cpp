#include "pch.h"
#include "TypeRegistry.h"

namespace FieaGameEngine
{
	TypeRegistry& TypeRegistry::GetInstance()
	{
		static TypeRegistry instance;
		return instance;
	}

	const Vector<Signature>& TypeRegistry::GetSignaturesOfType(RTTI::IdType id) const
	{
		return _types.At(id);
	}
}