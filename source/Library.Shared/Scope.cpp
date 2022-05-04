#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(std::size_t capacity) :
		_unorderedMap(capacity), _orderedVector(capacity)
	{
	}

	Scope::Scope(const Scope& other) :
		_unorderedMap(other._unorderedMap.BucketCount()), _orderedVector(other._orderedVector.Capacity())
	{
		for (auto it : other._orderedVector)
		{
			Datum& currentDatum = (*it).second;
			Datum newDatum;
			if (currentDatum.Type() == Datum::DatumType::Table)
			{
				newDatum.SetType(Datum::DatumType::Table);
				for (std::size_t i = 0; i < currentDatum.Size(); ++i)
				{
					Scope* childCopy = currentDatum.GetTable(i).Clone();
					childCopy->_parent = this;
					newDatum.PushBackTable(*childCopy);
				}
			}
			else
			{
				newDatum = currentDatum;
			}
			std::pair entry = std::make_pair((*it).first, newDatum);
			std::pair result = _unorderedMap.Insert(entry);
			_orderedVector.PushBack(&(*result.first));
		}
	}

	Scope::Scope(Scope&& other) noexcept :
		_unorderedMap(std::move(other._unorderedMap)), _orderedVector(std::move(other._orderedVector))
	{
		for (auto it : _orderedVector)
		{
			Datum& currentDatum = (*it).second;
			if (currentDatum.Type() == Datum::DatumType::Table)
			{
				for (std::size_t i = 0; i < currentDatum.Size(); ++i)
				{
					currentDatum.GetTable(i)._parent = this;
				}
			}
		}
		if (other._parent != nullptr)
		{
			other._parent->Orphan(other);
			other._parent = nullptr;
		}
		other.Clear();
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (&other != this && !other.IsAncestorOf(*this) && !other.IsDescendantOf(*this))
		{
			Clear();
			_unorderedMap = Hashmap<std::string, Datum>(other._unorderedMap.BucketCount());
			_orderedVector = Vector<typename Hashmap<std::string, Datum>::pair_type*>(other._orderedVector.Capacity());
			for (auto it : other._orderedVector)
			{
				Datum& currentDatum = (*it).second;
				Datum newDatum;
				if (currentDatum.Type() == Datum::DatumType::Table)
				{
					newDatum.SetType(Datum::DatumType::Table);
					for (std::size_t i = 0; i < currentDatum.Size(); ++i)
					{
						Scope* childCopy = currentDatum.GetTable(i).Clone();
						childCopy->_parent = this;
						newDatum.PushBackTable(*childCopy);
					}
				}
				else
				{
					newDatum = currentDatum;
				}
				std::pair entry = std::make_pair((*it).first, newDatum);
				std::pair result = _unorderedMap.Insert(entry);
				_orderedVector.PushBack(&(*result.first));
			}
		}		
		return *this;
	}

	Scope& Scope::operator=(Scope&& other) noexcept
	{
		if (_parent != nullptr)
		{
			_parent->Orphan(*this);
			_parent = nullptr;
		}
		Clear();
		_unorderedMap = std::move(other._unorderedMap);
		_orderedVector = std::move(other._orderedVector);
		if (other._parent != nullptr)
		{
			other._parent->Orphan(other);
			other._parent = nullptr;
		}
		other.Clear();
		return *this;
	}

	Scope::~Scope()
	{
		if (_parent != nullptr)
		{
			_parent->Orphan(*this);
			_parent = nullptr;
		}
		Clear();
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		bool areSame = true;
		if (this != &rhs)
		{
			if (_orderedVector.Size() == rhs._orderedVector.Size())
			{
				for (std::size_t i = 0; i < _orderedVector.Size(); ++i)
				{
					auto& lhsPair = *_orderedVector[i];
					auto& rhsPair = *rhs._orderedVector[i];

					if (lhsPair.first == "this"s)
					{
						continue;
					}

					if (lhsPair != rhsPair)
					{
						areSame = false;
						break;
					}
				}
			}
			else
			{
				areSame = false;
			}			
		}
		return areSame;
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !operator==(rhs);
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (this == rhs)
		{
			return true;
		}

		if (rhs == nullptr)
		{
			return false;
		}

		const Scope* rhsScope = rhs->As<Scope>();
		if (rhsScope != nullptr)
		{
			return *this == *rhsScope;
		}
		else
		{
			return false;
		}
	}

	std::size_t Scope::Size() const
	{
		return _orderedVector.Size();
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	Datum* Scope::Find(const std::string& name)
	{
		if (_unorderedMap.ContainsKey(name))
		{
			return &(_unorderedMap.At(name));
		}
		else
		{
			return nullptr;
		}
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		if (_unorderedMap.ContainsKey(name))
		{
			return &(_unorderedMap.At(name));
		}
		else
		{
			return nullptr;
		}
	}

	Datum* Scope::Search(const std::string& name)
	{
		if (_unorderedMap.ContainsKey(name))
		{
			return &(_unorderedMap.At(name));
		}
		else
		{
			if (_parent != nullptr)
			{
				return _parent->Search(name);
			}
			else
			{
				return nullptr;
			}
		}
	}

	const Datum* Scope::Search(const std::string& name) const
	{
		if (_unorderedMap.ContainsKey(name))
		{
			return &(_unorderedMap.At(name));
		}
		else
		{
			if (_parent != nullptr)
			{
				return _parent->Search(name);
			}
			else
			{
				return nullptr;
			}
		}
	}

	Datum* Scope::Search(const std::string& name, Scope*& foundScope)
	{
		if (_unorderedMap.ContainsKey(name))
		{
			foundScope = this;
			return &(_unorderedMap.At(name));
		}
		else
		{
			if (_parent != nullptr)
			{
				return _parent->Search(name, foundScope);
			}
			else
			{
				return nullptr;
			}
		}
	}

	const Datum* Scope::Search(const std::string& name, const Scope*& foundScope) const
	{
		if (_unorderedMap.ContainsKey(name))
		{
			foundScope = this;
			return &(_unorderedMap.At(name));
		}
		else
		{
			if (_parent != nullptr)
			{
				return _parent->Search(name, foundScope);
			}
			else
			{
				return nullptr;
			}
		}
	}

	std::pair<Scope*, std::size_t> Scope::DownSearch(const std::string& name)
	{
		for (auto pair : _orderedVector)
		{
			Datum& datum = pair->second;
			if (pair->first == name)
			{
				return std::make_pair(this, _orderedVector.IndexOf(pair));
			}
			else if (datum.Type() == Datum::DatumType::Table)
			{

				for (std::size_t i = 0; i < datum.Size(); ++i)
				{
					auto foundPair = datum.GetTable(i).DownSearch(name);
					if (foundPair.first != nullptr)
					{
						return foundPair;
					}
				}
			}
		}
		return std::make_pair(nullptr, 0);
	}

	Datum* Scope::SearchForTargetDatum(const std::string& name)
	{
		auto [scope, index] = SearchForTargetContainedDatum(name);
		if (scope != nullptr)
		{
			return &(*scope)[index];
		}
		else
		{
			return nullptr;
		}
	}

	std::pair<Scope*, std::size_t> Scope::SearchForTargetContainedDatum(const std::string& name)
	{
		if (_parent == nullptr)
		{
			return DownSearch(name);
		}
		else
		{
			return _parent->SearchForTargetContainedDatum(name);
		}
	}

	Datum& Scope::Append(const std::string& name)
	{
		bool wasInserted;
		return Append(name, wasInserted);
	}

	Datum& Scope::Append(const std::string& name, bool& wasInserted)
	{
		if (name.empty())
		{
			throw std::invalid_argument("Name cannot be empty");
		}

		std::pair result = _unorderedMap.Insert(std::make_pair(name, Datum()));
		auto& position = result.first;
		wasInserted = result.second;
		if (wasInserted)
		{
			_orderedVector.PushBack(&(*position));
		}

		return (*position).second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		bool wasInserted;
		Datum& datum = Append(name, wasInserted);

		Scope* newScope = new Scope();
		newScope->_parent = this;
		datum.PushBackTable(*newScope);
		return datum.GetTable();
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (&child == this)
		{
			throw std::runtime_error("Self-adoption.");
		}

		if (child.IsAncestorOf(*this))
		{
			throw std::runtime_error("Ancestor adoption.");
		}

		bool wasInserted;
		Datum& datum = Append(name, wasInserted);
		if (wasInserted)
		{
			// Validate that the found key is associated with a Datum of type Table (or Unknown)
			if (datum.Type() != Datum::DatumType::Table && datum.Type() != Datum::DatumType::Unknown)
			{
				throw std::runtime_error("An existing key found with an incompatible Datum.");
			}
		}

		if (child._parent != nullptr)
		{
			child._parent->Orphan(child);
		}
		child._parent = this;
		datum.PushBackTable(child);
	}

	Scope* Scope::GetParent() const
	{
		return _parent;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	const Datum& Scope::operator[](const std::string& name) const
	{
		if (_unorderedMap.ContainsKey(name))
		{
			return (*_unorderedMap.Find(name)).second;
		}
		else
		{
			throw std::runtime_error("Name not found in Scope.");
		}
	}

	Datum& Scope::operator[](std::size_t index)
	{
		return (*_orderedVector[index]).second;
	}

	const Datum& Scope::operator[](std::size_t index) const
	{
		return (*_orderedVector[index]).second;
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(const Scope& scope) const
	{
		std::pair<Datum*, std::size_t> result = std::make_pair(nullptr, std::numeric_limits<std::size_t>::max());
		for (auto it : _orderedVector)
		{
			Datum& datum = (*it).second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				for (std::size_t i = 0; i < datum.Size(); ++i)
				{
					if (datum.GetTable(i) == scope)
					{
						result.first = &datum;
						result.second = i;
						break;
					}
				}
			}
		}
		return result;
	}

	void Scope::Clear()
	{
		for (auto it : _orderedVector)
		{
			Datum& datum = it->second;
			if (datum.Type() == Datum::DatumType::Table)
			{
				assert(!datum.IsExternal());
				for (std::size_t i = 0; i < datum.Size(); ++i)
				{
					datum.GetTable(i)._parent = nullptr;
					delete &datum.GetTable(i);
				}
				datum.Clear();
			}
		}
		_unorderedMap.Clear();
		_orderedVector.Clear();
	}

	void Scope::Orphan(Scope& child)
	{
		auto [datum, index] = FindContainedScope(child);
		if (datum != nullptr)
		{
			datum->RemoveAt(index);
			child._parent = nullptr;
		}
	}

	std::size_t Scope::IndexOf(const std::string& name) const
	{
		auto it = _unorderedMap.Find(name);
		if (it == _unorderedMap.cend())
		{
			return _orderedVector.Size();
		}
		return _orderedVector.IndexOf(const_cast<std::pair<const std::string, Datum>*>(&(*it)));
	}

	bool Scope::IsAncestorOf(const Scope& scope) const
	{
		bool result = false;
		Scope* parent = scope._parent;
		while (parent != nullptr)
		{
			if (parent == this)
			{
				result = true;
				break;
			}
			parent = parent->_parent;
		}
		return result;
	}

	bool Scope::IsDescendantOf(const Scope& scope) const
	{
		return scope.IsAncestorOf(*this);
	}
}