#include "pch.h"
#include "ActionCreateAction.h"
#include "Factory.h"
#include "GameObject.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionCreateAction)

	ActionCreateAction::ActionCreateAction() :
		Action(ActionCreateAction::TypeIdClass())
	{
	}

	void ActionCreateAction::Update(const GameTime& /*time*/)
	{
		Scope* parent = GetParent();
		assert(parent != nullptr);
		assert(parent->As<GameObject>() != nullptr || parent->As<ActionList>() != nullptr);

		assert(_className.empty() == false);
		assert(_instanceName.empty() == false);

		RTTI* rtti = Factory<RTTI>::Create(_className);
		Action* actionPtr = rtti->As<Action>();
		assert(actionPtr != nullptr);
		actionPtr->SetName(_instanceName);

		parent->Adopt(*actionPtr, "Actions");
	}

	gsl::owner<Attributed*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction();
	}

	const Vector<Signature> ActionCreateAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(ActionCreateAction, _name) },
			{ "ClassName", Datum::DatumType::String, 1, offsetof(ActionCreateAction, _className) },
			{ "InstanceName", Datum::DatumType::String, 1, offsetof(ActionCreateAction, _instanceName) },
			{ "Actions", Datum::DatumType::Table, 1, 0 }
		};
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const auto rhsActionCreateAction = rhs->As<ActionCreateAction>();
		if (rhsActionCreateAction == nullptr)
		{
			return false;
		}

		return _name == rhsActionCreateAction->_name;
	}

	std::string ActionCreateAction::ToString() const
	{
		return "ActionCreateAction"s;
	}

	const std::string& ActionCreateAction::ClassName() const
	{
		return _className;
	}

	void ActionCreateAction::SetClassName(const std::string& name)
	{
		_className = name;
	}

	const std::string& ActionCreateAction::InstanceName() const
	{
		return _instanceName;
	}

	void ActionCreateAction::SetInstanceName(const std::string& name)
	{
		_instanceName = name;
	}
}