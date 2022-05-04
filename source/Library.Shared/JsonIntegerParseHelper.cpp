#include "pch.h"
#include "JsonIntegerParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonIntegerParseHelper)
	RTTI_DEFINITIONS(JsonIntegerParseHelper::ParsedDataContainer)

	JsonIntegerParseHelper::ParsedDataContainer::~ParsedDataContainer()
	{
	}

	void JsonIntegerParseHelper::ParsedDataContainer::Initialize()
	{
		Data.Clear();
		Data.ShrinkToFit();
	}

	gsl::owner<JsonParseCoordinator::ParsedDataContainer*> JsonIntegerParseHelper::ParsedDataContainer::Create() const
	{
		return new JsonIntegerParseHelper::ParsedDataContainer();
	}

	JsonIntegerParseHelper::~JsonIntegerParseHelper()
	{
	}

	std::shared_ptr<IJsonParseHelper> JsonIntegerParseHelper::Create() const
	{
		return std::make_shared<JsonIntegerParseHelper>();
	}

	void JsonIntegerParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		_isParsingData = false;
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key, Json::Value& value)
	{
		JsonIntegerParseHelper::ParsedDataContainer* integerParsedData = data.As<JsonIntegerParseHelper::ParsedDataContainer>();
		if (integerParsedData == nullptr)
		{
			return false;
		}
		
		if (key != _integerKey)
		{
			return false;
		}

		if (_isParsingData)
		{
			throw std::runtime_error("Was already parsing other data.");
		}

		_isParsingData = true;		
		integerParsedData->Data.PushBack(value.asInt());
		
		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key)
	{
		JsonIntegerParseHelper::ParsedDataContainer* integerParsedData = data.As<JsonIntegerParseHelper::ParsedDataContainer>();
		if (integerParsedData == nullptr)
		{
			return false;
		}

		if (key != _integerKey)
		{
			return false;
		}

		if (_isParsingData == false)
		{
			return false;
		}

		_isParsingData = false;
		return true;
	}

}