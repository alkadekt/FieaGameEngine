#pragma once
#include <cstddef>
#include <stdexcept>
#include <functional>
#include <cassert>
#include "IncrementFunctor.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Resizable container of items stored in contiguous memory.
	/// </summary>
	/// <typeparam name="T">Data type.</typeparam>
	template <typename T>
	class Vector final
	{
	public:
		using value_type = T;

		/// <summary>
		/// Used to traverse and mutate items in a non const Vector.
		/// </summary>
		class Iterator
		{
			friend class Vector<T>;

		public:
			//using iterator_category = std::random_access_iterator_tag;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using size_type = std::size_t;

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
			/// PreDecrement operator.
			/// </summary>
			/// <returns>Iterator reference.</returns>
			Iterator& operator--();

			/// <summary>
			/// PostDecrement operator.
			/// </summary>
			/// <returns>Iterator.</returns>
			Iterator operator--(int);

			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>T reference.</returns>
			T& operator*() const;

		private:
			Iterator(Vector& owner, std::size_t index = 0);
			Vector* owner{ nullptr };
			size_t index{ 0 };
		};

		/// <summary>
		/// Used to traverse but not mutate items in a Vector.
		/// </summary>
		class ConstIterator
		{
			friend class Vector<T>;

		public:
			//using iterator_category = std::random_access_iterator_tag;
			using iterator_category = std::bidirectional_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using size_type = std::size_t;

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
			/// PreDecrement operator.
			/// </summary>
			/// <returns>ConstIterator reference.</returns>
			ConstIterator& operator--();

			/// <summary>
			/// PostDecrement operator.
			/// </summary>
			/// <returns>ConstIterator.</returns>
			ConstIterator operator--(int);

			/// <summary>
			/// Dereference operator.
			/// </summary>
			/// <returns>const T reference.</returns>
			const T& operator*() const;

		private:
			ConstIterator(const Vector& owner, std::size_t index = 0);
			const Vector* owner{ nullptr };
			size_t index{ 0 };
		};

		/// <summary>
		/// Default Vector constructor. Capacity set to 0 as default.
		/// User can supply a functor for determining how capacity is increased based on current size and capacity
		/// </summary>
		Vector(std::size_t capacity = 0, 
			std::function<std::size_t(std::size_t size, std::size_t capacity)> reserveFunctor = IncrementFunctor{});

		/// <summary>
		/// Copy constructor. Create Vector instance from other Vector instance.
		/// </summary>
		/// <param name="other">The given Vector instance.</param>
		Vector(const Vector& other);
		
		/// <summary>
		/// Instantiate Vector from initializer list.
		/// </summary>
		/// <param name="list">Init list of data to insert.</param>
		Vector(std::initializer_list<T> list, 
			std::function<std::size_t(std::size_t size, std::size_t capacity)> reserveFunctor = IncrementFunctor{});
		
		/// <summary>
		/// Move constructor. Shallow copy memory of given into this instance.
		/// </summary>
		/// <param name="other">Given R value vector.</param>
		Vector(Vector&& other) noexcept;
		
		/// <summary>
		/// Deconstructor. Clears list.
		/// </summary>
		~Vector();
		
		/// <summary>
		/// Copy Assignment Operator. Deep copies given vector into lhs vector.
		/// </summary>
		/// <param name="other">Given vector.</param>
		/// <returns>Vector reference.</returns>
		Vector& operator=(const Vector& other);

		/// <summary>
		/// Assignment operator that takes in initializer list.
		/// </summary>
		Vector& operator=(const std::initializer_list<T> list);
		
		/// <summary>
		/// Move Assignment Operator. Shallow copies given vector into lhs vector.
		/// </summary>
		/// <param name="other">Given R value vector.</param>
		/// <returns>Vector reference.</returns>
		Vector& operator=(Vector&& other) noexcept;

		/// <summary>
		/// Equivalence operator, returns whether two vectors are equal.
		/// </summary>
		bool operator==(const Vector& rhs) const;

		/// <summary>
		/// UnEquivalence operator, returns whether two vectors are unequal.
		/// </summary>
		bool operator!=(const Vector& rhs) const;
		
		/// <summary>
		/// Bracket Operator. Returns reference to item at given index.
		/// </summary>
		/// <returns>Reference.</returns>
		T& operator[](std::size_t index);
		
		/// <summary>
		/// Bracket Operator. Returns const reference to item at given index.
		/// </summary>
		/// <returns>Const reference.</returns>
		const T& operator[](std::size_t index) const;
		
		/// <summary>
		/// Returns a reference to data at given index.
		/// </summary>
		/// <returns>Reference to data.</returns>
		Iterator At(std::size_t index);
		
		/// <summary>
		/// Returns a const reference to data at given index.
		/// </summary>
		/// <returns>Const reference to data.</returns>
		ConstIterator At(std::size_t index) const;
		
		/// <summary>
		/// Removes last element in Vector.
		/// </summary>
		void PopBack();
		
		/// <summary>
		/// Returns boolean of whether Vector has no elements.
		/// </summary>
		bool IsEmpty() const;
		
		/// <summary>
		/// Returns first data element in Vector.
		/// </summary>
		/// <returns>Data reference.</returns>
		T& Front();
		
		/// <summary>
		/// Returns first data element in Vector.
		/// </summary>
		/// <returns>Const data reference.</returns>
		const T& Front() const;
		
		/// <summary>
		/// Returns last data element in Vector.
		/// </summary>
		/// <returns>Data reference.</returns>
		T& Back();
		
		/// <summary>
		/// Returns last data element in Vector.
		/// </summary>
		/// <returns>Const data reference.</returns>
		const T& Back() const;
		
		/// <summary>
		/// Returns number of data items in Vector.
		/// </summary>
		size_t Size() const;

		/// <summary>
		/// Returns number of data spots in memory currently reserved for this Vector.
		/// </summary>
		size_t Capacity() const;

		/// <summary>
		/// Returns Iterator pointing to first item in Vector
		/// </summary>
		Iterator begin();

		/// <summary>
		/// Returns ConstIterator pointing to first item in Vector.
		/// </summary>
		ConstIterator begin() const;

		/// <summary>
		/// Returns Iterator pointing to first item in Vector
		/// </summary>
		ConstIterator cbegin() const;

		/// <summary>
		/// Returns Iterator pointing to empty space after last item.
		/// </summary>
		Iterator end();
		
		/// <summary>
		/// Returns ConstIterator pointing to empty space after last item.
		/// </summary>
		ConstIterator end() const;

		/// <summary>
		/// Returns ConstIterator pointing to empty space after last item.
		/// </summary>
		ConstIterator cend() const;

		/// <summary>
		/// Adds given data to the back of the Vector.
		/// </summary>
		/// <param name="data">Data to be added.</param>
		void PushBack(const T& data);

		/// <summary>
		/// Adds given data within Rvalue reference to the back of the Vector.
		/// </summary>
		/// <param name="data">Data to be added.</param>
		void PushBack(T&& data);

		/// <summary>
		/// Reserves given amount of memory space for Vector. 
		/// Given capacity cannot be lower than current capacity.
		/// </summary>
		/// <returns>Whether space was successfully reserved.</returns>
		void Reserve(std::size_t newCapacity);

		/// <summary>
		/// Returns Iterator pointing to first item found with matching value.
		/// If no such item is found, returns end().
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional substitue function if data type
		/// contains no equality operator.</typeparam>
		/// <param name="value">Value to search for.</param>
		template <typename EqualityFunctor = std::equal_to<T>>
		Iterator Find(const T& value);
		
		/// <summary>
		/// Returns ConstIterator pointing to first item found with matching value.
		/// If no such item is found, returns end().
		/// </summary>
		/// <typeparam name="EqualityFunctor">Optional substitue function if data type
		/// contains no equality operator.</typeparam>
		/// <param name="value">Value to search for.</param>
		template <typename EqualityFunctor = std::equal_to<T>>
		ConstIterator Find(const T& value) const;
		
		/// <summary>
		/// Returns whether this Vector contains given value.
		/// </summary>
		template <typename EqualityFunctor = std::equal_to<T>>
		bool Contains(const T& value) const;

		/// <summary>
		/// Removes given item from Vector. Returns whether item was actually removed.
		/// </summary>
		bool Remove(const T& value);

		/// <summary>
		/// Removes item at given index from Vector.
		/// </summary>
		/// <returns>Whether the item was removed.</returns>
		bool Remove(const Iterator& it);

		/// <summary>
		/// Removes items within given Iterators (start inclusive, end exclusive) from Vector.
		/// </summary>
		/// <returns>Whether the items were removed.</returns>
		bool Remove(const Iterator& startIt, const Iterator& endIt);

		/// <summary>
		/// Resize capacity to match size.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Changes size to match given, adjusting capacity as needed.
		/// New items are default constructed.
		/// </summary>
		/// <param name="size"></param>
		void Resize(std::size_t newSize);

		/// <summary>
		/// Removes all items from Vector. Calls destructor on each one.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns index within Vector at which value is found, if found.
		/// </summary>
		std::size_t IndexOf(const T& value) const;

	private:
		T* arrayPtr{ nullptr };
		std::size_t size{ 0 };
		std::size_t capacity{ 0 };
		std::function<std::size_t(std::size_t, std::size_t)> reserveFunctor;
	};
}

#include "Vector.inl"