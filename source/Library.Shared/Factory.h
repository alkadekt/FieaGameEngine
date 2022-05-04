#pragma once
#include <gsl/gsl>
#include "RTTI.h"
#include "Hashmap.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Abstract class template to create a factory capable of creating instances of type baseProduct.
	/// </summary>
	template <typename baseProduct>
	class Factory
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Factory() = default;

		Factory(const Factory& other) = delete;
		Factory& operator=(const Factory& other) = delete;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		Factory(Factory&& other) noexcept = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		Factory& operator=(Factory&& other) noexcept = default;

		/// <summary>
		/// Destructor. Performs no work.
		/// </summary>
		virtual ~Factory();
		
		/// <summary>
		/// Pure virtual method to return name of class produced by derived factory.
		/// </summary>
		virtual const std::string& ClassName() const = 0;

		/// <summary>
		/// Pure virtual method to create instance of product class from derived factory.
		/// </summary>
		virtual gsl::owner<baseProduct*> Create() const = 0;
		
		/// <summary>
		/// Finds a contained Factory that produces classes of className.
		/// Returns nullptr if not found.
		/// </summary>
		static std::shared_ptr<const Factory> Find(const std::string& className);

		/// <summary>
		/// Creates a class instance of className if contained factory exists for it.
		/// Returns nullptr if that class does not exist as factory in list.
		/// </summary>
		static gsl::owner<baseProduct*> Create(const std::string& className);

		/// <summary>
		/// Adds a factory into the list.
		/// </summary>
		static void Add(std::shared_ptr<Factory> factory);

		/// <summary>
		/// Removes a factory from the list.
		/// </summary>
		/// <param name="factory"></param>
		static void Remove(std::shared_ptr<Factory> factory);

	private:
		inline static Hashmap<std::string, std::shared_ptr<const Factory<baseProduct>>> _factories;
	};

#define ConcreteFactory(DerivedProductType, BaseProductType)	\
	class DerivedProductType ## Factory : public Factory<BaseProductType> \
	{ \
	public: \
		DerivedProductType ## Factory() : \
			_className(#DerivedProductType) \
		{ \
		} \
 \
		virtual ~ ## DerivedProductType ## Factory() \
		{ \
		} \
\
		const std::string& ClassName() const override \
		{ \
			return _className; \
		} \
\
		gsl::owner<BaseProductType*> Create() const override \
		{ \
			return new DerivedProductType(); \
		} \
\
	private: \
		std::string _className; \
	};
}

#include "Factory.inl"