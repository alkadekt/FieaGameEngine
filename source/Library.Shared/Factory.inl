#include "Factory.h"

namespace FieaGameEngine
{
	template <typename baseProduct>
	Factory<baseProduct>::~Factory()
	{
	}

	template <typename baseProduct>
	std::shared_ptr<const Factory<baseProduct>> Factory<baseProduct>::Find(const std::string& className)
	{
		auto it = _factories.Find(className);
		if (it != _factories.end())
		{
			return (*it).second;
		}
		else
		{
			return std::shared_ptr<const Factory<baseProduct>>(nullptr);
		}
	}

	template <typename baseProduct>
	gsl::owner<baseProduct*> Factory<baseProduct>::Create(const std::string& className)
	{
		std::shared_ptr<const Factory<baseProduct>> foundFactory = Find(className);
		if (foundFactory)
		{
			return foundFactory->Create();
		}
		else
		{
			return nullptr;
		}
	}

	template <typename baseProduct>
	void Factory<baseProduct>::Add(std::shared_ptr<Factory> factory)
	{
		const std::string& name = factory->ClassName();
		if (!_factories.ContainsKey(name))
		{
			_factories.Insert(std::make_pair(name, std::move(factory)));
		}
		else
		{
			throw std::runtime_error("Already have this class in list.");
		}
	}

	template <typename baseProduct>
	void Factory<baseProduct>::Remove(std::shared_ptr<Factory> factory)
	{
		_factories.Remove(factory->ClassName());
	}
}