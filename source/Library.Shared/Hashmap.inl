#include "Hashmap.h"

namespace FieaGameEngine
{
	template<typename TKey, typename TData>
	Hashmap<TKey, TData>::Iterator::Iterator(Hashmap& owner, 
		typename map_type::Iterator bucketIt, typename chain_type::Iterator chainIt) :
		owner(&owner), bucketIt(bucketIt), chainIt(chainIt)
	{
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const
	{
		bool result = true;
		if (owner == rhs.owner && bucketIt == rhs.bucketIt &&
			(chainIt == rhs.chainIt || bucketIt == owner->_map.end()))
		{
			result = false;
		}
		return result;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator& Hashmap<TKey, TData>::Iterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		++chainIt;
		if (chainIt == (*bucketIt).end())
		{
			++bucketIt;
			while (bucketIt != owner->_map.end())
			{
				if ((*bucketIt).Size() > 0)
				{
					break;
				}
				++bucketIt;
			}
			if (bucketIt != owner->_map.end())
			{
				chainIt = (*bucketIt).begin();
			}
		}
		return *this;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::pair_type& Hashmap<TKey, TData>::Iterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a Hashmap.");
		}

		return *chainIt;
	}

	template<typename TKey, typename TData>
	Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Hashmap& owner, 
		typename map_type::ConstIterator bucketIt, typename chain_type::ConstIterator chainIt) :
		owner(&owner), bucketIt(bucketIt), chainIt(chainIt)
	{
	}

	template<typename TKey, typename TData>
	Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& other) :
		owner(other.owner), bucketIt(other.bucketIt), chainIt(other.chainIt)
	{
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		bool result = true;
		if (owner == rhs.owner && bucketIt == rhs.bucketIt && 
			(chainIt == rhs.chainIt || bucketIt == owner->_map.cend()))
		{
			result = false;
		}
		return result;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator& Hashmap<TKey, TData>::ConstIterator::operator++()
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Cannot increment Iterator with no owning container.");
		}

		++chainIt;
		if (chainIt == (*bucketIt).cend())
		{
			++bucketIt;
			while (bucketIt != owner->_map.cend())
			{
				if ((*bucketIt).Size() > 0)
				{
					break;
				}
				++bucketIt;
			}
			if (bucketIt != owner->_map.cend())
			{
				chainIt = (*bucketIt).cbegin();
			}
		}
		return *this;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		++(*this);
		return temp;
	}

	template<typename TKey, typename TData>
	inline const typename Hashmap<TKey, TData>::pair_type& Hashmap<TKey, TData>::ConstIterator::operator*() const
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a Hashmap.");
		}

		return *chainIt;
	}

	template<typename TKey, typename TData>
	Hashmap<TKey,TData>::Hashmap(std::size_t bucketCount, 
		std::function<std::size_t(const TKey&)> hashFunction,
		std::function<bool(const TKey&, const TKey&)> equalityFunction) :
		_map(bucketCount), _hashFunction(hashFunction), _equalityFunction(equalityFunction)
	{
		if (bucketCount < 2)
		{
			throw std::runtime_error("bucketCount cannot be less than 2.");
		}

		_map.Resize(bucketCount);
	}

	template<typename TKey, typename TData>
	Hashmap<TKey, TData>::Hashmap(std::initializer_list<pair_type> list,
		std::function<std::size_t(const TKey&)> hashFunction,
		std::function<bool(const TKey&, const TKey&)> equalityFunction) :
		_map(list.size()), _hashFunction(hashFunction), _equalityFunction(equalityFunction)
	{
		_map.Resize(list.size());

		for (auto& pair : list)
		{
			Insert(pair);
		}
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Find(const TKey& key)
	{
		if (_size > 0)
		{
			std::size_t index = _hashFunction(key) % _map.Size();
			chain_type& chain = _map[index];

			typename chain_type::Iterator chainIt;
			for (chainIt = chain.begin(); chainIt != chain.end(); ++chainIt)
			{
				if (_equalityFunction((*chainIt).first, key))
				{
					return Iterator(*this, _map.At(index), chainIt);
				}
			}
		}		
		return end();
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::Find(const TKey& key) const
	{
		if (_size > 0)
		{
			const std::size_t index = _hashFunction(key) % _map.Size();
			const chain_type& chain = _map[index];

			typename chain_type::ConstIterator chainIt;
			for (chainIt = chain.cbegin(); chainIt != chain.cend(); ++chainIt)
			{
				if (_equalityFunction((*chainIt).first, key))
				{
					return ConstIterator(*this, _map.At(index), chainIt);
				}
			}
		}
		return cend();
	}

	template<typename TKey, typename TData>
	std::pair<typename Hashmap<TKey, TData>::Iterator, bool> Hashmap<TKey, TData>::Insert(const pair_type& entry)
	{
		bool wasInserted = false;

		std::size_t index = _hashFunction(entry.first) % _map.Size();
		chain_type& chain = _map[index];

		typename chain_type::Iterator chainIt;
		for (chainIt = chain.begin(); chainIt != chain.end(); ++chainIt)
		{
			if (_equalityFunction((*chainIt).first, entry.first))
			{
				return std::make_pair(Iterator(*this, _map.At(index), chainIt), wasInserted);
			}
		}
		chainIt = chain.PushBack(std::pair(entry.first, entry.second));
		++_size;
		wasInserted = true;
		return std::make_pair(Iterator(*this, _map.At(index), chainIt), wasInserted);
	}

	template<typename TKey, typename TData>
	TData& Hashmap<TKey, TData>::operator[](const TKey& key)
	{
		Iterator it = Find(key);
		if (it != end())
		{
			return (*it).second;
		}
		else
		{
			return (*Insert(std::make_pair(key, TData())).first).second;
		}
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Remove(const TKey& key)
	{
		Iterator it = Find(key);
		if (it != end())
		{
			--_size;
			return (*(it.bucketIt)).Remove(it.chainIt);
		}
		else
		{
			return false;
		}
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::Remove(const Iterator& it)
	{
		it;
		return false;
	}

	template<typename TKey, typename TData>
	void Hashmap<TKey, TData>::Clear()
	{
		for (auto& bucket : _map)
		{
			bucket.Clear();
		}

		_size = 0;
	}

	template<typename TKey, typename TData>
	inline std::size_t Hashmap<TKey, TData>::Size() const
	{
		return _size;
	}

	template<typename TKey, typename TData>
	inline std::size_t Hashmap<TKey, TData>::BucketCount() const
	{
		return _map.Capacity();
	}

	template<typename TKey, typename TData>
	inline float Hashmap<TKey, TData>::LoadFactor() const
	{
		std::size_t filledBuckets = 0;
		for (auto it = _map.begin(); it != _map.end(); ++it)
		{
			if ((*it).Size() > 0)
			{
				++filledBuckets;
			}
		}
		return float(filledBuckets) / BucketCount();
	}

	template<typename TKey, typename TData>
	bool Hashmap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		return Find(key) != cend();
	}

	template<typename TKey, typename TData>
	TData& Hashmap<TKey, TData>::At(const TKey& key)
	{
		return (*Find(key)).second;
	}

	template<typename TKey, typename TData>
	const TData& Hashmap<TKey, TData>::At(const TKey& key) const
	{
		return (*Find(key)).second;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::begin()
	{
		if (_size == 0)
		{
			return Iterator(*this, _map.begin(), _map[0].begin());
		}
		typename map_type::Iterator bucketIt;
		for (bucketIt = _map.begin(); bucketIt != _map.end(); ++bucketIt)
		{
			if ((*bucketIt).Size() > 0)
			{
				break;
			}
		}
		return Iterator(*this, bucketIt, (*bucketIt).begin());
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cbegin() const
	{
		if (_size == 0)
		{
			return ConstIterator(*this, _map.cbegin(), _map[0].cbegin());
		}
		typename map_type::ConstIterator bucketIt;
		for (bucketIt = _map.begin(); bucketIt != _map.end(); ++bucketIt)
		{
			if ((*bucketIt).Size() > 0)
			{
				break;
			}
		}
		return ConstIterator(*this, bucketIt, (*bucketIt).cbegin());
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::end()
	{
		return Iterator(*this, _map.end(), chain_type::Iterator());
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, _map.cend(), chain_type::ConstIterator());
	}
}