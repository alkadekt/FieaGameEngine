#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(RTTI::IdType id) :
		_id(id)
	{
		(*this)["this"s] = this;
		Populate();
	}

	Attributed::Attributed(const Attributed& other) :
		Scope(other), _id(other._id)
	{
		UpdateExternalStorage();
	}

	Attributed& Attributed::operator=(const Attributed& other)
	{
		if (&other != this)
		{
			Scope::operator=(other);
			_id = other._id;
			UpdateExternalStorage();
		}
		return *this;
	}

	Attributed::Attributed(Attributed&& other) noexcept :
		Scope(std::move(other)), _id(other._id)
	{
		UpdateExternalStorage();
	}

	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		if (&other != this)
		{
			Scope::operator=(std::move(other));
			_id = other._id;
			UpdateExternalStorage();
		}
		return *this;
	}

	Attributed::~Attributed()
	{
	}

	bool Attributed::IsAttribute(const std::string& name) const
	{
		return Find(name) != nullptr;
	}

	bool Attributed::IsPrescribedAttribute(const std::string& name) const
	{
		TypeRegistry& registry = TypeRegistry::GetInstance();
		const Vector<Signature>& signatures = registry.GetSignaturesOfType(_id);
		return IndexOf(name) <= signatures.Size();
	}

	bool Attributed::IsAuxiliaryAttribute(const std::string& name) const
	{
		return IsAttribute(name) && !IsPrescribedAttribute(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const std::string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::runtime_error("This attribute already exists as a prescribed attribute");
		}
		return Append(name);
	}

	void Attributed::Clear()
	{
		Scope::Clear();
	}

	void Attributed::Populate()
	{
		TypeRegistry& registry = TypeRegistry::GetInstance();
		const Vector<Signature>& signatures = registry.GetSignaturesOfType(_id);
		for (const auto& sig : signatures)
		{
			Datum& newDatum = Append(sig._name);
			newDatum.SetType(sig._type);
			if (sig._type != Datum::DatumType::Table)
			{
				newDatum.SetStorage(sig._type, reinterpret_cast<std::uint8_t*>(this) + sig._offset, sig._count);
			}
			else
			{
				for (std::size_t i = 0; i < sig._count; ++i)
				{
					AppendScope(sig._name);
				}
			}
		}
	}

	void Attributed::UpdateExternalStorage()
	{
		(*this)["this"s] = this;
		TypeRegistry& registry = TypeRegistry::GetInstance();
		const Vector<Signature>& signatures = registry.GetSignaturesOfType(_id);
		for (std::size_t i = 0; i < Size(); ++i)
		{
			Datum& datum = (*this)[i];
			if (datum.IsExternal())
			{
				Datum::DatumType prevType = datum.Type();
				std::size_t prevSize = datum.Size();
				datum.SetStorage(prevType, reinterpret_cast<std::uint8_t*>(this) + signatures[i-1]._offset, prevSize);
			}
		}
	}
}