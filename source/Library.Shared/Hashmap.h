#pragma once
#include <cstddef>
#include <stdexcept>
#include <functional>
#include <cassert>
#include "SList.h"
#include "Vector.h"
#include "HashFunctor.h"

namespace FieaGameEngine
{
	template<typename TKey, typename TData>
	class Hashmap final
	{
	public:
		using pair_type = std::pair<const TKey, TData>;
		using chain_type = SList<pair_type>;
		using map_type = Vector<chain_type>;

		class Iterator
		{
			friend class Hashmap;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = TData;
			using reference = TData;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor for Iterator.
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor for Iterator.
			/// </summary>
			/// <param name="other">The Iterator to be copied.</param>
			Iterator(const Iterator & other) = default;

			/// <summary>
			/// Default destructor for Iterator.
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Default Assignment Operator for Iterator.
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			Iterator& operator=(const Iterator & other) = default;

			/// <summary>
			/// Equivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator==(const Iterator & rhs) const;

			/// <summary>
			/// UnEquivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator!=(const Iterator & rhs) const;

			/// <summary>
			/// PreIncrement operator.
			/// </summary>
			/// <returns>Iterator reference.</returns>
			Iterator& operator++();

			/// <summary>
			/// PostIncrement operator.
			/// </summary>
			/// <returns>Iterator.</returns>
			Iterator operator++(int);

			/// <summary>
			/// Dereference operator, returns key data pair reference.
			/// </summary>
			pair_type& operator*() const;

		private:
			Iterator(Hashmap& owner, 
				typename map_type::Iterator bucketIt, 
				typename chain_type::Iterator chainIt);
			Hashmap* owner{ nullptr };
			typename map_type::Iterator bucketIt{ };
			typename chain_type::Iterator chainIt{ };
		};

		class ConstIterator
		{
			friend class Hashmap;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = TData;
			using reference = TData;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor for ConstIterator.
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default copy constructor for ConstIterator.
			/// </summary>
			/// <param name="other">ConstIterator to be copied.</param>
			ConstIterator(const ConstIterator & other) = default;

			/// <summary>
			/// Conversion of Iterator to ConstIterator.
			/// </summary>
			/// <param name="other">Given Iterator.</param>
			ConstIterator(const Iterator & other);

			/// <summary>
			/// Default ConstIterator destructor.
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Equivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator==(const ConstIterator & rhs) const;

			/// <summary>
			/// UnEquivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator!=(const ConstIterator & rhs) const;

			/// <summary>
			/// PreIncrement operator.
			/// </summary>
			/// <returns>ConstIterator reference.</returns>
			ConstIterator& operator++();

			/// <summary>
			/// PostIncrement operator.
			/// </summary>
			/// <returns>ConstIterator.</returns>
			ConstIterator operator++(int);

			/// <summary>
			/// Dereference operator, returns key data pair reference.
			/// </summary>
			const pair_type& operator*() const;

		private:
			ConstIterator(const Hashmap& owner, 
				typename map_type::ConstIterator bucketIt, 
				typename chain_type::ConstIterator chainIt);
			const Hashmap* owner{ nullptr };
			typename map_type::ConstIterator bucketIt{ };
			typename chain_type::ConstIterator chainIt{ };
		};
		
		/// <summary>
		/// Constructor with three parameters, each with defaults. bucketCount cannot be < 2.
		/// </summary>
		/// <param name="HashFunction">Function for generating index for any key.</param>
		/// <param name="EqualityFunction">Function for determining equality between keys.</param>
		explicit Hashmap(std::size_t bucketCount = 11, 
			std::function<std::size_t(const TKey&)> hashFunction = HashFunctor<TKey>{},
			std::function<bool(const TKey&, const TKey&)> equalityFunction = std::equal_to<TKey>{});

		/// <summary>
		/// Constructor that takes an initializer list of pair entries.
		/// </summary>
		/// <param name="HashFunction">Function for generating index for any key.</param>
		/// <param name="EqualityFunction">Function for determining equality between keys.</param>
		explicit Hashmap(std::initializer_list<pair_type> list, 
			std::function<std::size_t(const TKey&)> hashFunction = HashFunctor<TKey>{},
			std::function<bool(const TKey&, const TKey&)> equalityFunction = std::equal_to<TKey>{});

		/// <summary>
		/// Copy constructor, produces Hashmap from given Hashmap instance.
		/// </summary>
		Hashmap(const Hashmap& other) = default;

		/// <summary>
		/// Move constructor, produces Hashmap from given R value Hashmap, then zeroes out given map.
		/// </summary>
		/// <param name="other">R value Hashmap.</param>
		Hashmap(Hashmap&& other) noexcept = default;

		/// <summary>
		/// Copy assignment operator, sets lhs equal to given rhs Hashmap.
		/// </summary>
		Hashmap& operator=(const Hashmap& other) = default;

		/// <summary>
		/// Move assignment operator, sets lhs L value equal to rhs R value, then zeroes out rhs.
		/// </summary>
		/// <param name="other">R value Hashmap.</param>
		Hashmap& operator=(Hashmap&& other) noexcept = default;

		/// <summary>
		/// Destructor, clears out map.
		/// </summary>
		~Hashmap() = default;

		/// <summary>
		/// Returns Iterator pointing to data associated with given key.
		/// If unfound, adds entry, which requires that TData have default constructor.
		/// </summary>
		Iterator Find(const TKey& key);

		/// <summary>
		/// Returns ConstIterator pointing to data associated with given key.
		/// If unfound, adds entry, which requires that TData have default constructor.
		/// </summary>
		ConstIterator Find(const TKey& key) const;

		/// <summary>
		/// Inserts Key-Data pair into Hashmap then returns Iterator pointing to data.
		/// </summary>
		/// <param name="entry">Key-Data pair.</param>
		std::pair<Iterator, bool> Insert(const pair_type& entry);

		/// <summary>
		/// Returns the data reference associated with given key. If key does not exist in map, 
		/// an entry for it is created.
		/// </summary>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Removes entry associated with given key, if it exists.
		/// </summary>
		bool Remove(const TKey& key);

		/// <summary>
		/// Removes entry associated with given iterator.
		/// </summary>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes all entries from the Hashmap.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns number of entries currently in Hashmap.
		/// </summary>
		std::size_t Size() const;

		/// <summary>
		/// Number of avaiable buckets in map.
		/// </summary>
		std::size_t BucketCount() const;

		/// <summary>
		/// Ratio of number of entries to available buckets.
		/// </summary>
		float LoadFactor() const;

		/// <summary>
		/// Returns whether given key exists in this map.
		/// </summary>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Returns data reference associated with given key.
		/// </summary>
		TData& At(const TKey& key);
		
		/// <summary>
		/// Returns const data reference associated with given key.
		/// </summary>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Returns Iterator pointing to first found entry in map.
		/// </summary>
		Iterator begin();
		
		/// <summary>
		/// Returns ConstIterator pointing to first found entry in map.
		/// </summary>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns Iterator pointing to first invalid after final bucket in list.
		/// </summary>
		Iterator end();
		
		/// <summary>
		/// Returns ConstIterator pointing to first invalid after final bucket in list.
		/// </summary>
		ConstIterator cend() const;

	private:
		map_type _map{ };
		std::function<std::size_t(const TKey&)> _hashFunction;
		std::function<bool(const TKey&, const TKey&)> _equalityFunction;
		std::size_t _size{ 0 };
	};
}

#include "Hashmap.inl"