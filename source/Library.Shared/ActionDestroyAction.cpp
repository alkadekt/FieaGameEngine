#include "pch.h"
#include "ActionDestroyAction.h"
#include "GameObject.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionDestroyAction)

	ActionDestroyAction::ActionDestroyAction() :
		Action(ActionDestroyAction::TypeIdClass())
	{
	}

	void ActionDestroyAction::Update(const GameTime& /*time*/)
	{
		Scope* parent = GetParent();
		assert(parent != nullptr);
		assert(parent->As<GameObject>() != nullptr || parent->As<ActionList>() != nullptr);

		assert(_className.empty() == false);
		assert(_instanceName.empty() == false);

		Datum* actions = parent->Find("Actions");
		assert(actions != nullptr);

		for (std::size_t i = 0; i < actions->Size(); ++i)
		{
			Scope* scope = &actions->GetTable(i);
			Action* action = scope->As<Action>();
			assert(action != nullptr);
			if (action->Name() == _instanceName)
			{
				assert(action->Is(action->TypeName()));

				action->GetParent()->Orphan(*scope);
				delete scope;
				return;
			}
		}
	}

	gsl::owner<Attributed*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction();
	}

	const Vector<Signature> ActionDestroyAction::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(ActionDestroyAction, _name) },
			{ "ClassName", Datum::DatumType::String, 1, offsetof(ActionDestroyAction, _className) },
			{ "InstanceName", Datum::DatumType::String, 1, offsetof(ActionDestroyAction, _instanceName) },
			{ "Actions", Datum::DatumType::Table, 1, 0 }
		};
	}

	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const auto rhsActionDestroyAction = rhs->As<ActionDestroyAction>();
		if (rhsActionDestroyAction == nullptr)
		{
			return false;
		}

		return _name == rhsActionDestroyAction->_name;
	}

	std::string ActionDestroyAction::ToString() const
	{
		return "ActionDestroyAction"s;
	}

	const std::string& ActionDestroyAction::ClassName() const
	{
		return _className;
	}

	void ActionDestroyAction::SetClassName(const std::string& name)
	{
		_className = name;
	}

	const std::string& ActionDestroyAction::InstanceName() const
	{
		return _instanceName;
	}

	void ActionDestroyAction::SetInstanceName(const std::string& name)
	{
		_instanceName = name;
	}
}