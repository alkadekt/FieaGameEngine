#pragma once
#include <cassert>
#include "JsonParseHelper.h"
#include "Scope.h"
#include "Stack.h"

namespace FieaGameEngine
{
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)
	
	public:
		class ParsedDataContainer final : public JsonParseCoordinator::ParsedDataContainer
		{
			RTTI_DECLARATIONS(ParsedDataContainer, JsonParseCoordinator::ParsedDataContainer)
	
		public:
			/// <summary>
			/// Constructor that takes smart pointer to Scope or Scope derived object.
			/// </summary>
			ParsedDataContainer(Scope* scope);

			ParsedDataContainer(const JsonTableParseHelper::ParsedDataContainer& other) = delete;
			ParsedDataContainer& operator=(const JsonTableParseHelper::ParsedDataContainer& other) = delete;

			/// <summary>
			/// Move constructor. Creates shallow copy from argument, then clears out argument container.
			/// </summary>
			ParsedDataContainer(JsonTableParseHelper::ParsedDataContainer&& other) noexcept = default;
			
			/// <summary>
			/// Move assignment operator. Creates shallow copy from argument, then clears out argument container.
			/// </summary>
			ParsedDataContainer& operator=(JsonTableParseHelper::ParsedDataContainer&& other) noexcept = default;

			/// <summary>
			/// Overriden destructor from base.
			/// </summary>
			virtual ~ParsedDataContainer() override;

			/// <summary>
			/// Returns pointer to new table data container holding empty scope.
			/// </summary>
			virtual gsl::owner<JsonParseCoordinator::ParsedDataContainer*> Create() const override;

			Scope* _rootScope;
		};

		/// <summary>
		/// Default contstructor.
		/// </summary>
		JsonTableParseHelper() = default;

		JsonTableParseHelper(const JsonTableParseHelper& other) = delete;
		JsonTableParseHelper& operator=(const JsonTableParseHelper& other) = delete;

		/// <summary>
		/// Default move constructor. Shallow copy and clears given.
		/// </summary>
		JsonTableParseHelper(JsonTableParseHelper&& other) = default;

		/// <summary>
		/// Default move assignment operator. Shallow copy and clears given.
		/// </summary>
		JsonTableParseHelper& operator=(JsonTableParseHelper&& other) = default;

		/// <summary>
		/// Override for derived constructor. 
		/// </summary>
		~JsonTableParseHelper() override;

		/// <summary>
		/// Creates a new instance of this class type within smart pointer.
		/// </summary>
		virtual std::shared_ptr<IJsonParseHelper> Create() const override;

		/// <summary>
		/// Attempts to handle given key and Json::Value and store data within container's root scope.
		/// Returns whether data was handled.
		/// </summary>
		virtual bool StartHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key, Json::Value& value) override;
		
		/// <summary>
		/// Finishes the handling process. Should be called once for every StartHandler call.
		/// </summary>
		virtual bool EndHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key) override;

	private:
		bool DeserializeContext(const std::string& key, Json::Value& value,
			const std::string& contextKey, Datum& contextDatum, Datum::DatumType contextType);

		struct StackFrame final
		{
			const std::string& _key;
			Json::Value& _value;
			Datum::DatumType _type;
			std::string _className;
			Scope& _currentScope;
			bool _needType;
		};

		Stack<StackFrame> _contextStack;
		inline static const std::string _typeKey{ "type" };
		inline static const std::string _valueKey{ "value" };
		inline static const std::string _classKey{ "class" };
		inline static const std::string _childrenKey{ "Children" };
		inline static const std::string _actionsKey{ "Action" };
	};
}