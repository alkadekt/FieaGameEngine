#include "pch.h"
#include "JsonParseCoordinator.h"
#include "JsonParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator::ParsedDataContainer);

	JsonParseCoordinator::ParsedDataContainer::~ParsedDataContainer()
	{
		Cleanup();
	}

	void JsonParseCoordinator::ParsedDataContainer::Initialize()
	{
		_depth = 0;
	}

	void JsonParseCoordinator::ParsedDataContainer::Cleanup()
	{
	}

	JsonParseCoordinator* JsonParseCoordinator::ParsedDataContainer::GetJsonParseCoordinator() const
	{
		return _coordinator;
	}

	void JsonParseCoordinator::ParsedDataContainer::IncrementDepth()
	{
		++_depth;
	}

	void JsonParseCoordinator::ParsedDataContainer::DecrementDepth()
	{
		--_depth;
	}

	std::size_t JsonParseCoordinator::ParsedDataContainer::Depth() const
	{
		return _depth;
	}

	void JsonParseCoordinator::ParsedDataContainer::SetJsonParseCoordinator(JsonParseCoordinator* coordinator)
	{
		_coordinator = coordinator;
	}

	JsonParseCoordinator::JsonParseCoordinator(ParsedDataContainer* container) :
		_dataContainer(container)
	{
		_dataContainer->SetJsonParseCoordinator(this);
		Initialize();
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		_isClone(other._isClone), _dataContainer(other._dataContainer), _helpers(std::move(other._helpers))
	{
		_dataContainer->SetJsonParseCoordinator(this);
		other._dataContainer = nullptr;
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		_isClone = other._isClone;
		_dataContainer = other._dataContainer;
		_dataContainer->SetJsonParseCoordinator(this);
		_helpers = std::move(other._helpers);
		other._dataContainer = nullptr;	
		if (other._isClone)
		{
			delete& other;
		}
		return *this;
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (_isClone)
		{
			delete _dataContainer;
			_dataContainer = nullptr;
		}
		Cleanup();
	}

	void JsonParseCoordinator::Initialize()
	{
		if (_dataContainer != nullptr)
		{
			_dataContainer->Initialize();
		}
		for (auto helperPtr : _helpers)
		{
			helperPtr->Initialize();
		}
	}

	void JsonParseCoordinator::Cleanup()
	{
		if (_dataContainer != nullptr)
		{
			_dataContainer->Cleanup();
		}
		for (auto helperPtr : _helpers)
		{
			helperPtr->Cleanup();
		}
	}

	gsl::owner<JsonParseCoordinator*> JsonParseCoordinator::Clone() const
	{
		ParsedDataContainer* container = _dataContainer->Create();
		JsonParseCoordinator* newCoord = new JsonParseCoordinator(container);
		for (auto helperPtr : _helpers)
		{
			std::shared_ptr<IJsonParseHelper> helper = std::move(helperPtr->Create());
			assert(helper != nullptr);
			newCoord->AddHelper(helper);
		}
		newCoord->_isClone = true;
		return newCoord;
	}

	bool JsonParseCoordinator::IsClone() const
	{
		return _isClone;
	}

	void JsonParseCoordinator::AddHelper(std::shared_ptr<IJsonParseHelper> helper)
	{
		std::shared_ptr<IJsonParseHelper> newHelper;
		if (_isClone)
		{
			newHelper = helper->Create();
		}
		else
		{
			newHelper = std::move(helper);
		}
		_helpers.PushBack(newHelper);
	}
	
	void JsonParseCoordinator::RemoveHelper(const std::shared_ptr<IJsonParseHelper>& helper)
	{
		_helpers.Remove(helper);
	}
	
	const Vector<std::shared_ptr<IJsonParseHelper>>& JsonParseCoordinator::Helpers()
	{
		return _helpers;
	}

	void JsonParseCoordinator::Parse(const std::string& data)
	{
		std::stringstream input;
		input.str(data);
		Parse(input);
	}

	void JsonParseCoordinator::Parse(std::istream& data)
	{
		if (_helpers.Size() == 0)
		{
			return;
		}

		assert(_dataContainer != nullptr);
		Json::Value root;
		data >> root;

		ParseMembers(root);
	}

	void JsonParseCoordinator::ParseFromFile(const std::string& filename)
	{
		std::ifstream file;
		file.open(filename);
		Parse(file);
	}

	JsonParseCoordinator::ParsedDataContainer* JsonParseCoordinator::GetParsedDataContainer()
	{
		return _dataContainer;
	}
	
	void JsonParseCoordinator::SetParsedDataContainer(JsonParseCoordinator::ParsedDataContainer* address)
	{
		if (address == nullptr)
		{
			throw std::runtime_error("Cannot set container to nullptr.");
		}
		_dataContainer = address;
	}

	void JsonParseCoordinator::ParseMembers(Json::Value& value)
	{
		_dataContainer->IncrementDepth();


		auto memberNames = value.getMemberNames();
		for (const auto& memberName : memberNames)
		{
			Parse(memberName, value[memberName], value[memberName].isArray());
		}

		_dataContainer->DecrementDepth();
	}

	void JsonParseCoordinator::Parse(const std::string& key, Json::Value& value, bool isArray)
	{
		for (auto helper : _helpers)
		{
			helper->Initialize();
			bool result = helper->StartHandler(*_dataContainer, key, value);
			if (result)
			{
				if (isArray)
				{
					for (auto& arrayValue : value)
					{
						Parse(key, arrayValue, arrayValue.isArray());
					}
				}
				else if (value.isObject())
				{
					ParseMembers(value);
				}
				helper->EndHandler(*_dataContainer, key);
				helper->Cleanup();
				break;
			}
		}		
	}
}