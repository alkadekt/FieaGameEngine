#pragma once
#include <cstdint>
#include "RTTI.h"
#include "Factory.h"

namespace FieaGameEngine
{
	class Foo : public RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)

	public:
		Foo(std::int32_t data = 0);
		Foo(const Foo& other);
		Foo& operator=(const Foo& other);
		Foo(Foo&& other) noexcept;
		Foo& operator=(Foo&& other) noexcept;
		virtual ~Foo();
		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;
		const std::int32_t& Data() const;
		std::int32_t& Data();

	private:
		std::int32_t* data{ nullptr };
	};

	ConcreteFactory(Foo, RTTI);

	//Expansion of ConcreteFactory
	//class FooFactory : public Factory<RTTI>
	//{
	//public:
	//	FooFactory() :
	//		_className("Foo"s)
	//	{
	//	}
	//
	//	~FooFactory()
	//	{
	//	}
	//
	//	const std::string& ClassName() const override
	//	{
	//		return _className;
	//	}
	//
	//	std::unique_ptr<RTTI> Create() const override
	//	{
	//		return std::make_unique<Foo>();
	//	}
	//
	//private:
	//	std::string _className;
	//};
}
