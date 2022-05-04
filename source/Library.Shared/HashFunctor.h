#pragma once
#include <cstddef>
#include "RTTI.h"

namespace FieaGameEngine
{
	template<typename T>
	struct HashBase
	{
		/// <summary>
		/// Performs an additive hash on the given byte data.
		/// </summary>
		/// <param name="data">Pointer to start of byte data in memory.</param>
		/// <param name="size">Size of the object to hash.</param>
		/// <returns>Unsigned int.</returns>
		inline std::size_t AdditiveHash(const std::uint8_t* data, std::size_t size)
		{
			std::size_t hash = 11;

			for (std::size_t i = 0; i < size; ++i)
			{
				hash += data[i];
			}
			return hash;
		}
	};

	/// <summary>
	/// Functor used to produce random number from given key.
	/// MAY NOT WORK FOR GIVEN TYPE IF GIVEN TYPE CANNOT BE MEMCOMPARED.
	/// </summary>
	template<typename T>
	struct HashFunctor : HashBase<T>
	{
		/// <summary>
		/// Produces random unsigned int from given key.
		/// </summary>
		std::size_t operator()(const T& key)
		{
			return AdditiveHash(reinterpret_cast<std::uint8_t*>(&key), sizeof(T));
		}
	};

	/// <summary>
	/// Specialization of HashFunctor for std::string.
	/// </summary>
	template<>
	struct HashFunctor<RTTI::IdType> : HashBase<RTTI::IdType>
	{
		std::size_t operator()(const RTTI::IdType& key)
		{
			return key;
		}
	};

	/// <summary>
	/// Specialization of HashFunctor for std::string.
	/// </summary>
	template<>
	struct HashFunctor<std::string> : HashBase<std::string>
	{
		std::size_t operator()(const std::string& key)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key.c_str()), key.size());
		}
	};

	/// <summary>
	/// Specialization of HashFunctor for const std::string.
	/// </summary>
	template<>
	struct HashFunctor<const std::string> : HashBase<const std::string>
	{
		std::size_t operator()(const std::string& key)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key.c_str()), key.size());
		}
	};

	/// <summary>
	/// Specialization of HashFunctor for char*.
	/// </summary>
	template<>
	struct HashFunctor<char*> : HashBase<char*>
	{
		std::size_t operator()(char* key)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key), strlen(key));
		}
	};

	/// <summary>
	/// Specialization of HashFunctor for const char*.
	/// </summary>
	template<>
	struct HashFunctor<const char*> : HashBase<const char*>
	{
		std::size_t operator()(const char* key)
		{
			return AdditiveHash(reinterpret_cast<const std::uint8_t*>(key), strlen(key));
		}
	};
}