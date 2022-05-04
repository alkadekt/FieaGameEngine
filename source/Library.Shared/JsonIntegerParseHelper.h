#pragma once
#include "JsonParseHelper.h"

namespace FieaGameEngine
{
	class JsonIntegerParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, IJsonParseHelper)

	public:
		class ParsedDataContainer final : public JsonParseCoordinator::ParsedDataContainer
		{
			RTTI_DECLARATIONS(ParsedDataContainer, JsonParseCoordinator::ParsedDataContainer)

		public:
			virtual ~ParsedDataContainer() override;
			virtual void Initialize() override;
			virtual gsl::owner<JsonParseCoordinator::ParsedDataContainer*> Create() const override;
			Vector<std::int32_t> Data;
		};

		~JsonIntegerParseHelper() override;
		virtual std::shared_ptr<IJsonParseHelper> Create() const override;
		virtual void Initialize() override;
		virtual bool StartHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key, Json::Value& value) override;
		virtual bool EndHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key) override;

	private:
		inline static const std::string _integerKey{ "integer" };
		bool _isParsingData{ false };
	};
}