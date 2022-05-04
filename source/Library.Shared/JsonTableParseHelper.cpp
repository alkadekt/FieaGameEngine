#include "pch.h"
#include "JsonTableParseHelper.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper)
	RTTI_DEFINITIONS(JsonTableParseHelper::ParsedDataContainer)

	JsonTableParseHelper::ParsedDataContainer::ParsedDataContainer(Scope* scope) :
		_rootScope(scope)
	{
	}

	JsonTableParseHelper::ParsedDataContainer::~ParsedDataContainer()
	{
	}

	gsl::owner<JsonParseCoordinator::ParsedDataContainer*> JsonTableParseHelper::ParsedDataContainer::Create() const
	{
		return new JsonTableParseHelper::ParsedDataContainer(new Scope());
	}

	JsonTableParseHelper::~JsonTableParseHelper()
	{

	}

	std::shared_ptr<IJsonParseHelper> JsonTableParseHelper::Create() const
	{
		return std::make_shared<JsonTableParseHelper>();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key, Json::Value& value)
	{		
		JsonTableParseHelper::ParsedDataContainer* tableParsedData = data.As<JsonTableParseHelper::ParsedDataContainer>();
		if (tableParsedData == nullptr)
		{
			return false;
		}

		if (key == _typeKey)
		{
			StackFrame& topFrame = _contextStack.Top();

			Datum::DatumType contextType;
			try
			{
				contextType = Datum::_stringToDatumTypeMap.At(value.asString());
			}
			catch (std::runtime_error)
			{
				return false;
			}

			topFrame._type = contextType;
			const std::string& contextKey = topFrame._key;
			Datum& contextDatum = topFrame._currentScope.Append(contextKey);
			contextDatum.SetType(contextType);

			if (topFrame._needType)
			{
				topFrame._needType = false;
				if (topFrame._className.empty() == false)
				{
					auto nestedScope = Factory<Scope>::Create(topFrame._className);
					topFrame._currentScope.Adopt(*nestedScope, contextKey);
					
					StackFrame newStack{ key, value, Datum::DatumType::Unknown, std::string(), *nestedScope, false };
					_contextStack.Push(newStack);
				}
				else
				{
					if (!value.isArray())
					{
						return DeserializeContext(key, value, contextKey, contextDatum, contextType);
					}
				}
			}
		}
		else if (key == _classKey)
		{
			StackFrame& topFrame = _contextStack.Top();
			
			topFrame._className = value.asString();
		}
		else if (key == _valueKey)
		{
			StackFrame& topFrame = _contextStack.Top();

			Datum::DatumType contextType = topFrame._type;
			const std::string& contextKey = topFrame._key;
			Datum& contextDatum = topFrame._currentScope.Append(contextKey);

			if (contextType == Datum::DatumType::Unknown)
			{
				topFrame._needType = true;
			}
			else
			{
				if (!value.isArray())
				{
					if (topFrame._className.empty() == false)
					{
						RTTI* rtti = Factory<RTTI>::Create(topFrame._className);

						Scope* nestedScope = rtti->As<Scope>();
						assert(nestedScope != nullptr);

						topFrame._currentScope.Adopt(*nestedScope, contextKey);

						StackFrame newStack{ key, value, Datum::DatumType::Unknown, std::string(), *nestedScope, false };
						_contextStack.Push(newStack);						
					}
					else
					{
						return DeserializeContext(key, value, contextKey, contextDatum, contextType);
					}
				}
			}
		}
		else
		{
			if (_contextStack.Size() == 0)
			{
				StackFrame startStack{ key, value, Datum::DatumType::Unknown, std::string(), *tableParsedData->_rootScope, false};
				_contextStack.Push(startStack);
			}
			else
			{
				StackFrame newStack{ key, value, Datum::DatumType::Unknown, std::string(), _contextStack.Top()._currentScope, false};
				_contextStack.Push(newStack);
			}			
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::ParsedDataContainer& data, const std::string& key)
	{
		JsonTableParseHelper::ParsedDataContainer* tableParsedData = data.As<JsonTableParseHelper::ParsedDataContainer>();
		if (tableParsedData == nullptr)
		{
			return false;
		}

		if (_contextStack.Top()._key == key)
		{
			_contextStack.Pop();
		}

		return true;
	}

	bool JsonTableParseHelper::DeserializeContext(const std::string& key, Json::Value& value, 
		const std::string& contextKey, Datum& contextDatum, Datum::DatumType contextType)
	{
		switch (contextType)
		{
		case FieaGameEngine::Datum::DatumType::Integer:
			if (contextDatum.IsExternal())
			{
				contextDatum.SetInteger(value.asInt());
			}
			else
			{
				contextDatum.PushBackInteger(value.asInt());
			}
			break;
		case FieaGameEngine::Datum::DatumType::Float:
			if (contextDatum.IsExternal())
			{
				contextDatum.SetFloat(value.asFloat());
			}
			else
			{
				contextDatum.PushBackFloat(value.asFloat());
			}
			break;
		case FieaGameEngine::Datum::DatumType::String:
			if (contextDatum.IsExternal())
			{
				contextDatum.SetString(value.asString());
			}
			else
			{
				contextDatum.PushBackString(value.asString());
			}
			break;
		case FieaGameEngine::Datum::DatumType::Vector:
			if (contextDatum.IsExternal())
			{
				contextDatum.SetFromString(value.asString());
			}
			else
			{
				contextDatum.PushBackFromString(value.asString());
			}
			break;
		case FieaGameEngine::Datum::DatumType::Matrix:
			if (contextDatum.IsExternal())
			{
				contextDatum.SetFromString(value.asString());
			}
			else
			{
				contextDatum.PushBackFromString(value.asString());
			}
			break;
		case FieaGameEngine::Datum::DatumType::Table:
		{
			Scope& newScopeRef = _contextStack.Top()._currentScope.AppendScope(contextKey);

			StackFrame newStack{ key, value, Datum::DatumType::Unknown, std::string(), newScopeRef, false};
			_contextStack.Push(newStack);
			break;
		}
		default:
			return false;
			break;
		}
		return true;
	}
}