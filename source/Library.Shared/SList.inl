#include "SList.h"

namespace FieaGameEngine
{
	template<typename T>
	SList<T>::Node::Node(const T& data, Node* next) :
		data(data), next(next)
	{
	}

	template<typename T>
	SList<T>::Node::Node(Node&& other) noexcept :
		data(other.data), next(other.next)
	{
		other.next = nullptr;
	}

	template<typename T>
	SList<T>::Iterator::Iterator(const SList& owner, Node* node) :
		owner(&owner), node(node)
	{
	}

	template<typename T>
	bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		bool result = true;
		if (owner == rhs.owner)
		{
			if (node == rhs.node)
			{
				result = false;
			}
		}
		return result;
	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		if (node != nullptr)
		{
			node = node->next;
		}

		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename T>
	inline T& SList<T>::Iterator::operator*() const
	{
		if (node == nullptr)
		{
			throw std::runtime_error("Cannot dereference an empty Iterator.");
		}
		return node->data;
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const SList& owner, Node* node) :
		owner(&owner), node(node)
	{
	}

	template<typename T>
	SList<T>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner), node(other.node)
	{
	}

	template<typename T>
	bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		bool result = true;
		if (owner == rhs.owner)
		{
			if (node == rhs.node)
			{
				result = false;
			}
		}
		return result;
	}

	template<typename T>
	typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		if (node != nullptr)
		{
			node = node->next;
		}

		return *this;
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (node == nullptr)
		{
			throw std::runtime_error("Cannot dereference an empty Iterator.");
		}
		return node->data;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator(*this, front);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return Iterator(*this, front);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return cbegin();
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator(*this);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return Iterator(*this);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return cend();
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& data)
	{
		if (it.owner != this)
		{
			throw std::runtime_error("Given Iterator is not associated with this list instance.");
		}

		if (it == end() || it.node == back)
		{
			PushBack(data);
			return Iterator(*this, back);
		}
		else
		{
			Node* temp = it.node->next;
			it.node->next = new Node(data, temp);
			size++;
			return Iterator(*this, it.node->next);
		}
	}

	template<typename T>
	template <typename EqualityFunctor>
	typename SList<T>::Iterator SList<T>::Find(const T& value)
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
	template <typename EqualityFunctor>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
	{
		EqualityFunctor equalityFunc;
		ConstIterator it;
		for (it = cbegin(); it != cend(); ++it)
		{
			if (equalityFunc(*it, value))
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	template <typename EqualityFunctor>
	bool SList<T>::Remove(const Iterator& it)
	{
		if (size > 0 && it != end())
		{
			EqualityFunctor equalityFunc;

			if (equalityFunc(Back(), it.node->data))
			{
				PopBack();
			}
			else
			{
				Node* nextNode = it.node->next;
				it.node->~Node();
				new (it.node) Node(std::move(*nextNode));
				delete nextNode;
				--size;
			}
			return true;
		}
		return false;
	}

	template<typename T>
	template <typename EqualityFunctor>
	bool SList<T>::Remove(const T& data)
	{
		if (size > 0)
		{
			EqualityFunctor equalityFunc;
			Iterator it;
			for (it = begin(); it.node != nullptr; ++it)
			{
				if (equalityFunc(it.node->data, data))
				{
					if (equalityFunc(Back(), data))
					{
						PopBack();
					}
					else
					{
						Node* nextNode = it.node->next;
						it.node->~Node();
						new (it.node) Node(std::move(*nextNode));
						delete nextNode;
						--size;
					}
					return true;
				}				
			}
		}
		return false;
	}

	template<typename T>
	SList<T>::SList()
	{
	}

	template<typename T>
	SList<T>::SList(const SList& other)
	{
		Node* otherPtr = other.front;
		while (otherPtr != nullptr)
		{
			PushBack(otherPtr->data);
			otherPtr = otherPtr->next;
		}
	}

	template<typename T>
	SList<T>::SList(const std::initializer_list<T> list)
	{
		for (auto data : list)
		{
			PushBack(data);
		}
	}

	template<typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();

			Node* otherPtr = other.front;
			while (otherPtr != nullptr)
			{
				PushBack(otherPtr->data);
				otherPtr = otherPtr->next;
			}
		}
		return *this;
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const std::initializer_list<T> list)
	{
		Clear();
		for (auto data : list)
		{
			PushBack(data);
		}
		return *this;
	}

	template<typename T>
	T& SList<T>::Front()
	{
		if (front == nullptr)
		{
			throw std::runtime_error("Cannot access front of empty list.");
		}
		return front->data;
	}

	template<typename T>
	const T& SList<T>::Front() const
	{
		if (front == nullptr)
		{
			throw std::runtime_error("Cannot access front of empty list.");
		}
		return front->data;
	}

	template<typename T>
	T& SList<T>::Back()
	{
		if (back == nullptr)
		{
			throw std::runtime_error("Cannot access back of empty list.");
		}
		return back->data;
	}

	template<typename T>
	const T& SList<T>::Back() const
	{
		if (back == nullptr)
		{
			throw std::runtime_error("Cannot access back of empty list.");
		}
		return back->data;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& entry)
	{
		front = new Node(entry, front);
		if (size == 0)
		{
			back = front;
		}
		++size;
		return Iterator(*this, front);
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (front == nullptr)
		{
			throw std::runtime_error("Cannot pop front of empty list.");
		}
		if (size == 1)
		{
			Clear();
		}
		else
		{
			Node* temp = front;
			front = front->next;
			delete temp;
			--size;
		}
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& entry)
	{
		Node* node = new Node(entry);
		if (size == 0)
		{
			front = node;
		}
		else
		{
			back->next = node;
		}
		back = node;
		++size;
		return Iterator(*this, back);
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		if (back == nullptr)
		{
			throw std::runtime_error("Cannot pop back of empty list.");
		}
		if (size == 1)
		{
			PopFront();
		}
		else
		{
			Node* currentNode = front;
			while (currentNode->next != back)
			{
				currentNode = currentNode->next;
			}
			Node* temp = back;
			back = currentNode;
			back->next = nullptr;
			delete temp;
			--size;
		}
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return size == 0;
	}

	template <typename T>
	inline std::size_t SList<T>::Size() const
	{
		return size;
	}

	template<typename T>
	void SList<T>::Clear()
	{
		Node* currentNode = front;
		while (currentNode != nullptr)
		{
			Node* nextNode = currentNode->next;
			delete currentNode;
			currentNode = nextNode;
		}
		front = back = nullptr;
		size = 0;
	}
}