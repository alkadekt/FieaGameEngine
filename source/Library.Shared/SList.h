#pragma once
#include "pch.h"
#include <cstddef>
#include <stdexcept>
#include <functional>

namespace FieaGameEngine
{
	/// <summary>
	/// Templated singly linked list.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template <typename T> 
	class SList final
	{
	private:
		struct Node final
		{
			Node(const T& data, Node* next = nullptr);
			Node(const Node&) = delete;
			Node(Node&& other) noexcept;
			Node& operator=(const Node&) = delete;
			Node& operator=(Node&&) noexcept = delete;
			~Node() = default;
			T data;
			Node* next{ nullptr };
		};

	public:
		using value_type = T;

		/// <summary>
		/// Used to traverse and mutate items in a non const SList.
		/// </summary>
		class Iterator
		{
			friend class SList;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor for Iterator.
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor for Iterator.
			/// </summary>
			/// <param name="other">The Iterator to be copied.</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// Default destructor for Iterator.
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Default Assignment Operator for Iterator.
			/// </summary>
			/// <param name="other"></param>
			/// <returns></returns>
			Iterator& operator=(const Iterator& other) = default;

			/// <summary>
			/// Equivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator==(const Iterator& rhs) const;

			/// <summary>
			/// UnEquivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator!=(const Iterator& rhs) const;
			
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
			/// Dereference operator.
			/// </summary>
			/// <returns>T reference.</returns>
			T& operator*() const;

		private:
			Iterator(const SList& owner, Node* node = nullptr);
			Node* node{ nullptr };
			const SList* owner{ nullptr };
		};

		/// <summary>
		/// Used to traverse but not mutate items in an SList.
		/// </summary>
		class ConstIterator
		{
			friend class SList;

		public:
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using reference = T;
			using iterator_category = std::forward_iterator_tag;

			/// <summary>
			/// Default constructor for ConstIterator.
			/// </summary>
			ConstIterator() = default;
			
			/// <summary>
			/// Default copy constructor for ConstIterator.
			/// </summary>
			/// <param name="other">ConstIterator to be copied.</param>
			ConstIterator(const ConstIterator& other) = default;
			
			/// <summary>
			/// Conversion of Iterator to ConstIterator.
			/// </summary>
			/// <param name="other">Given Iterator.</param>
			ConstIterator(const Iterator& other);
			
			/// <summary>
			/// Default ConstIterator destructor.
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Equivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator==(const ConstIterator& rhs) const;
			
			/// <summary>
			/// UnEquivalence operator.
			/// </summary>
			/// <param name="rhs">Right hand side of operator.</param>
			/// <returns>Boolean.</returns>
			bool operator!=(const ConstIterator& rhs) const;
			
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
			/// Dereference operator.
			/// </summary>
			/// <returns>const T reference.</returns>
			const T& operator*() const;

		private:
			ConstIterator(const SList& owner, Node* node = nullptr);
			const Node* node{ nullptr };
			const SList* owner{ nullptr };
		};

		/// <summary>
		/// Instantiate default SList.
		/// </summary>
		SList();

		/// <summary>
		/// Instantiate SList from existing SList instance.
		/// </summary>
		/// <typeparam name="other">The source instance.</typeparam>
		SList(const SList& other);

		/// <summary>
		/// Instantiate SList from initializer list.
		/// </summary>
		/// <param name="list">Init list of data to insert.</param>
		SList(const std::initializer_list<T> list);

		/// <summary>
		/// Move constructor. Shallow copy memory of given into this instance.
		/// </summary>
		/// <param name="other">Given R value SList.</param>
		SList(SList&& other) noexcept = default;

		/// <summary>
		/// Delete data in list and destruct SList.
		/// </summary>
		~SList();

		/// <summary>
		/// Deep copy given list into "this" list.
		/// </summary>
		/// <typeparam name="other">The given list.</typeparam>
		/// <returns>The left hand side instance.</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// Assignment operator that uses an initializer list.
		/// </summary>
		/// <typeparam name="other">The given init list.</typeparam>
		/// <returns>The left hand side instance.</returns>
		SList& operator=(const std::initializer_list<T> list);

		/// <summary>
		/// Move Assignment Operator. Shallow copies rhs to lhs.
		/// </summary>
		/// <param name="other">R value SList.</param>
		/// <returns>L value SList reference.</returns>
		SList& operator=(SList&& other) noexcept = default;

		/// <summary>
		/// Returns an ConstIterator pointing to the start of the SList.
		/// </summary>
		/// <returns>ConstIterator pointing to the start of the SList.</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns an Iterator pointing to the start of the SList.
		/// </summary>
		/// <returns>Iterator pointing to the start of the SList.</returns>
		Iterator begin();

		/// <summary>
		/// Returns a ConstIterator pointing to the start of the SList.
		/// </summary>
		/// <returns>ConstIterator pointing to the start of the SList.</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Returns a ConstIterator pointing to the end of the SList.
		/// </summary>
		/// <returns>ConstIterator pointing to the end of the SList.</returns>
		ConstIterator cend() const;

		/// <summary>
		/// Returns a Iterator pointing to the end of the SList.
		/// </summary>
		/// <returns>Iterator pointing to the end of the SList.</returns>
		Iterator end();

		/// <summary>
		/// Returns a ConstIterator pointing to the end of the SList.
		/// </summary>
		/// <returns>ConstIterator pointing to the end of the SList.</returns>
		ConstIterator end() const;

		/// <summary>
		/// Inserts the given data after the given Iterator in the SList.
		/// </summary>
		/// <param name="it">Given Iterator.</param>
		/// <param name="data">Data to insert.</param>
		/// <returns>Iterator holding inserted item.</returns>
		Iterator InsertAfter(const Iterator & it, const T & data);

		/// <summary>
		/// Searches through the list for a value, then returns it within an Iterator.
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional function for determining equality between data.</typeparam>
		/// <param name="value">The value to be searched for.</param>
		/// <returns>Iterator holding node with value if it was found, otherwise holding end().</returns>
		template <typename EqualityFunctor = std::equal_to<T>>
		Iterator Find(const T & value);

		/// <summary>
		/// Searches through the list for a value, then returns it within a ConstIterator.
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional function for determining equality between data.</typeparam>
		/// <param name="value">The value to be searched for.</param>
		/// <returns>ConstIterator holding node with value if it was found, otherwise holding end().</returns>
		template <typename EqualityFunctor = std::equal_to<T>>
		ConstIterator Find(const T & value) const;

		/// <summary>
		/// Removes the data held within given Iterator from the SList.
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional equality function for items without equivalence operators.</typeparam>
		template <typename EqualityFunctor = std::equal_to<T>>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes the given data from the SList.
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional equality function for items without equivalence operators.</typeparam>
		template <typename EqualityFunctor = std::equal_to<T>>
		bool Remove(const T & data);

		/// <summary>
		/// Returns data at front of list.
		/// </summary>
		/// <returns>The data as a reference.</returns>
		T& Front();

		/// <summary>
		/// Returns data at front of list as const.
		/// </summary>
		/// <returns>The const data as a reference.</returns>
		const T& Front() const;

		/// <summary>
		/// Returns data at back of list.
		/// </summary>
		/// <returns>The data as a reference.</returns>
		T& Back();

		/// <summary>
		/// Returns data at back of list as const.
		/// </summary>
		/// <returns>The const data as a reference.</returns>
		const T& Back() const;

		/// <summary>
		/// Pushes given data into front of list.
		/// </summary>
		/// <param name="entry">The given data reference.</param>
		Iterator PushFront(const T& entry);

		/// <summary>
		/// Takes front data element off list.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Pushes given data onto back of list.
		/// </summary>
		/// <param name="entry">The given data reference.</param>
		Iterator PushBack(const T& entry);

		/// <summary>
		/// Takes back data element off list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Returns boolean whether list is empty or not.
		/// </summary>
		/// <returns>A boolean.</returns>
		bool IsEmpty() const;

		/// <summary>
		/// Returns the current number of items in list.
		/// </summary>
		/// <returns>A const size.</returns>
		size_t Size() const;

		/// <summary>
		/// Removes all data items from the list.
		/// </summary>
		void Clear();

	private:
		size_t size{ 0 };
		Node* front{ nullptr };
		Node* back{ nullptr };
	};
}

#include "SList.inl"