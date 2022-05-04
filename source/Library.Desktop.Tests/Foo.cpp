#include "pch.h"
#include "Foo.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Foo)

	Foo::Foo(std::int32_t data) :
		data(new int32_t(data))
	{
	}

	Foo::Foo(const Foo& other) :
		data(new int32_t(other.Data()))
	{
	}

	Foo::Foo(Foo&& other) noexcept :
		data(other.data)
	{
		other.data = nullptr;
	}

	Foo& Foo::operator=(Foo&& other) noexcept
	{
		delete data;
		data = other.data;
		other.data = nullptr;
		return *this;
	}

	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			*data = *other.data;
		}
		return *this;
	}

	Foo::~Foo()
	{
		delete data;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (data == nullptr && rhs.data == nullptr)
		{
			return true;
		}
		else if (data == nullptr || rhs.data == nullptr)
		{
			throw std::runtime_error("Data is null.");
		}
		return *data == rhs.Data();
	}

	bool Foo::operator!=(const Foo& rhs) const
	{
		return !operator==(rhs);
	}

	const std::int32_t& Foo::Data() const
	{
		return *data;
	}

	std::int32_t& Foo::Data()
	{
		return *data;
	}
}