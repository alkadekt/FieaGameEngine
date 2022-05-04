#pragma once
#include "Hashmap.h"
#include "Vector.h"
#include "RTTI.h"
#include "Signature.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Container of Vectors of Signatures for registered types.
	/// </summary>
	class TypeRegistry final
	{
	public:
		TypeRegistry(const TypeRegistry& other) = delete;

		/// <summary>
		/// Returns single instance of this class, or creates it if it doesn't yet exist.
		/// </summary>
		/// <returns></returns>
		static TypeRegistry& GetInstance();

		/// <summary>
		/// Adds all signatures of given type to the registry. Type must have 
		/// a Signatures() method that gives Vector of Signature objects
		/// </summary>
		template<typename TType>
		void RegisterSignaturesOfType()
		{
			std::pair<RTTI::IdType, const Vector<Signature>> entry = 
				std::make_pair(TType::TypeIdClass(), TType::Signatures());
			_types.Insert(entry);
		}

		/// <summary>
		/// Returns Vector of Signature ref for the type. Throws exception if type ID
		/// does not exist in registry.
		/// </summary>
		const Vector<Signature>& GetSignaturesOfType(RTTI::IdType id) const;

	private:
		Hashmap<RTTI::IdType, const Vector<Signature>> _types;
		TypeRegistry() = default;
	};
}