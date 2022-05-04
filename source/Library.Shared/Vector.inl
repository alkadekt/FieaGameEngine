#include "Vector.h"

namespace FieaGameEngine
{
	template<typename T>
	Vector<T>::Iterator::Iterator(Vector& owner, std::size_t index) :
		owner(&owner), index(index)
	{
	}

	template<typename T>
	bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return owner != rhs.owner || index != rhs.index;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		if (index < owner->size)
		{
			++index;
		}
		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename T>
	typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}
		if (index > 0)
		{
			--index;
		}
		return *this;
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator temp = *this;
		--(*this);
		return temp;
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a Vector.");
		}
		if (index >= owner->size)
		{
			throw std::out_of_range("Iterator is not within valid range.");
		}
		return owner->arrayPtr[index];
	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Vector& owner, std::size_t index) :
		owner(&owner), index(index)
	{
	}

	template<typename T>
	Vector<T>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner), index(other.index)
	{
	}

	template<typename T>
	bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return owner != rhs.owner || index != rhs.index;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		if (index < owner->size)
		{
			++index;
		}
		return *this;
	}

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename T>
	typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}
		if (index > 0)
		{
			--index;
		}
		return *this;
	}

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator temp = *this;
		--(*this);
		return temp;
	}

	template<typename T>
	inline const T& Vector<T>::ConstIterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a Vector.");
		}
		if (index >= owner->size)
		{
			throw std::out_of_range("Iterator is not within valid range.");
		}
		return owner->arrayPtr[index];
	}

	template<typename T>
	Vector<T>::Vector(std::size_t capacity, 
		std::function<std::size_t(std::size_t size, std::size_t capacity)> reserveFunctor) :
		reserveFunctor(reserveFunctor)
	{
		if (capacity > 0)
		{
			Reserve(capacity);
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector& other) :
		reserveFunctor(other.reserveFunctor)
	{
		Reserve(other.capacity);
		for (auto it = other.cbegin(); it != other.cend(); ++it)
		{
			PushBack(*it);
		}
	}

	template<typename T>
	Vector<T>::Vector(std::initializer_list<T> list, 
		std::function<std::size_t(std::size_t size, std::size_t capacity)> reserveFunctor)
	{
		Reserve(list.size());
		for (auto& data : list)
		{
			PushBack(data);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& other) noexcept :
		arrayPtr(other.arrayPtr), size(other.size), capacity(other.capacity)
	{
		other.arrayPtr = nullptr;
		other.size = 0;
		other.capacity = 0;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		free(arrayPtr);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();
			free(arrayPtr);
			Reserve(other.size);
			for (auto it = other.cbegin(); it != other.cend(); ++it)
			{
				PushBack(*it);
			}
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const std::initializer_list<T> list)
	{
		Clear();
		free(arrayPtr);
		Reserve(list.size());
		for (auto data : list)
		{
			PushBack(data);
		}
		return *this;
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
	{
		if (this != &other)
		{
			Clear();
			free(arrayPtr);
			arrayPtr = other.arrayPtr;
			size = other.size;
			capacity = other.capacity;
			other.arrayPtr = nullptr;
			other.size = 0;
			other.capacity = 0;
		}
		return *this;
	}

	template<typename T>
	bool Vector<T>::operator==(const Vector& rhs) const
	{
		bool result = false;
		if (size == rhs.size)
		{
			for (std::size_t i = 0; i < size; ++i)
			{
				if (arrayPtr[i] != rhs.arrayPtr[i])
				{
					return result;
				}
			}
			result = true;
		}
		return result;
	}

	template<typename T>
	bool Vector<T>::operator!=(const Vector& rhs) const
	{
		return !operator==(rhs);
	}

	template<typename T>
	T& Vector<T>::operator[](std::size_t index)
	{
		if (index >= size)
		{
			throw std::out_of_range("Given index not within valid range.");
		}
		return arrayPtr[index];
	}

	template<typename T>
	const T& Vector<T>::operator[](std::size_t index) const
	{
		if (index >= size)
		{
			throw std::out_of_range("Given index not within valid range.");
		}
		return arrayPtr[index];
	}

	template<typename T>
	typename Vector<T>::Iterator Vector<T>::At(std::size_t index)
	{
		if (index >= size)
		{
			throw std::out_of_range("Given index not within valid range.");
		}
		return Iterator(*this, index);
	}

	template<typename T>
	typename Vector<T>::ConstIterator Vector<T>::At(std::size_t index) const
	{
		if (index >= size)
		{
			throw std::out_of_range("Given index not within valid range.");
		}
		return ConstIterator(*this, index);
	}

	template<typename T>
	void Vector<T>::PopBack()
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot PopBack of an empty Vector.");
		}
		
		arrayPtr[size - 1].~T();
		--size;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return size == 0;
	}

	template<typename T>
	T& Vector<T>::Front()
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot look at front of empty Vector.");
		}
		return arrayPtr[0];
	}

	template<typename T>
	const T& Vector<T>::Front() const
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot look at front of empty Vector.");
		}
		return arrayPtr[0];
	}

	template<typename T>
	T& Vector<T>::Back()
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot look at back of empty Vector.");
		}
		return arrayPtr[size - 1];
	}

	template<typename T>
	const T& Vector<T>::Back() const
	{
		if (size == 0)
		{
			throw std::runtime_error("Cannot look at back of empty Vector.");
		}
		return arrayPtr[size - 1];
	}

	template<typename T>
	inline size_t Vector<T>::Size() const
	{
		return size;
	}

	template<typename T>
	inline size_t Vector<T>::Capacity() const
	{
		return capacity;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return cbegin();
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator(*this, size);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator(*this, size);
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return cend();
	}

	template<typename T>
	void Vector<T>::PushBack(const T& data)
	{
		if (size == capacity)
		{
			Reserve(reserveFunctor(size, capacity));
		}

		new (arrayPtr + size) T(data);
		++size;
	}

	template<typename T>
	void Vector<T>::PushBack(T&& data)
	{
		if (size == capacity)
		{
			Reserve(reserveFunctor(size, capacity));
		}

		new (arrayPtr + size) T(data);
		++size;
	}

	template<typename T>
	void Vector<T>::Reserve(std::size_t newCapacity)
	{
		if (newCapacity > capacity)
		{
			T* temp = static_cast<T*>(realloc(arrayPtr, sizeof(T) * newCapacity));
			assert(temp != nullptr);
			arrayPtr = temp;
			capacity = newCapacity;
		}		
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		EqualityFunctor equalityFunc;
		Iterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunc(*it, value))
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	template<typename EqualityFunctor>
	typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		EqualityFunctor equalityFunc;
		ConstIterator it;
		for (it = begin(); it != end(); ++it)
		{
			if (equalityFunc(*it, value))
			{
				break;
			}
		}
		return it;
	}

	template <typename T>
	template <typename EqualityFunctor>
	bool Vector<T>::Contains(const T& value) const
	{
		return Find(value) != cend();
	}

	template<typename T>
	bool Vector<T>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template<typename T>
	bool Vector<T>::Remove(const Iterator& it)
	{
		if (it.owner != this)
		{
			throw std::runtime_error("Given Iterator is not associated with this Vector.");
		}

		bool successful = false;

		if (it != end())
		{
			arrayPtr[it.index].~T();
			memmove_s(arrayPtr + it.index, (capacity - it.index) * sizeof(T), 
				arrayPtr + (it.index + 1), (capacity - it.index - 1) * sizeof(T));
			--size;
			successful = true;
		}
		return successful;
	}

	template<typename T>
	bool Vector<T>::Remove(const Iterator& startIt, const Iterator& endIt)
	{
		if (startIt.owner != this)
		{
			throw std::runtime_error("Given start Iterator is not associated with this Vector.");
		}
		if (endIt.owner != this)
		{
			throw std::runtime_error("Given end Iterator is not associated with this Vector.");
		}
		if (startIt.index > endIt.index)
		{
			throw std::runtime_error("Given Iterators are in incorrect order.");
		}

		if (startIt.index != endIt.index)
		{
			for (std::size_t i = startIt.index; i < endIt.index; ++i)
			{
				arrayPtr[i].~T();
				--size;
			}
			memmove_s(arrayPtr + startIt.index, (capacity - startIt.index) * sizeof(T),
				arrayPtr + endIt.index, (capacity - endIt.index) * sizeof(T));

			return true;
		}
		return false;
	}

	template<typename T>
	void Vector<T>::ShrinkToFit()
	{
		if (size == 0)
		{
			free(arrayPtr);
			arrayPtr = nullptr;
			capacity = 0;
		}
		else if (size != capacity)
		{
			T* data = reinterpret_cast<T*>(realloc(arrayPtr, size * sizeof(T)));
			assert(data != nullptr);
			arrayPtr = data;
			capacity = size;
		}
	}

	template<typename T>
	void Vector<T>::Resize(std::size_t newSize)
	{
		if (newSize > size)
		{
			Reserve(newSize);

			for (std::size_t i = size; i < capacity; ++i)
			{
				new (arrayPtr + i) T();
			}
			size = newSize;
		}
		else
		{
			for (std::size_t i = newSize; i < size; ++i)
			{
				arrayPtr[i].~T();
			}
			if (newSize == 0)
			{
				free(arrayPtr);
				arrayPtr = nullptr;
			}
			else
			{
				T* temp = static_cast<T*>(realloc(arrayPtr, sizeof(T) * newSize));
				assert(temp != nullptr);
				arrayPtr = temp;
			}			
			capacity = newSize;
			size = newSize;
		}
	}

	template<typename T>
	void Vector<T>::Clear()
	{
		for (std::size_t i = 0; i < size; ++i)
		{
			arrayPtr[i].~T();
		}
		size = 0;
	}

	template<typename T>
	std::size_t Vector<T>::IndexOf(const T& value) const
	{
		ConstIterator it = Find(value);
		return it.index;
	}
}