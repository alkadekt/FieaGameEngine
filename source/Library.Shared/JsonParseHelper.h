#pragma once
#include <gsl/gsl>
#include <json/json.h>
#include "RTTI.h"
#include "JsonParseCoordinator.h"

namespace FieaGameEngine
{
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI)

	public:
		virtual ~IJsonParseHelper() = 0;
		virtual void Initialize();
		virtual void Cleanup();
		virtual std::shared_ptr<IJsonParseHelper> Create() const = 0;
		virtual bool StartHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key, Json::Value& value) = 0;
		virtual bool EndHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key) = 0;
	};
}