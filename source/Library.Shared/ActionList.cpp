#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionList)

	ActionList::ActionList() :
		Action(ActionList::TypeIdClass())
	{
	}

	ActionList::ActionList(RTTI::IdType id) :
		Action(id)
	{
	}

	void ActionList::Update(const GameTime& time)
	{
		assert(Find("Actions") != nullptr);
		for (std::size_t i = 0; i < Find("Actions")->Size(); ++i)
		{
			GetAction(i).Update(time);
		}
	}

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		RTTI* rtti = Factory<RTTI>::Create(className);
		assert(rtti != nullptr);
		Action* actionPtr = rtti->As<Action>();
		assert(actionPtr != nullptr);

		actionPtr->SetName(instanceName);
		Adopt(*actionPtr, "Actions");
		return actionPtr;
	}

	Action& ActionList::GetAction(std::size_t index)
	{
		Datum& actions = Actions();
		if (index >= actions.Size())
		{
			throw std::runtime_error("Invalid index for children.");
		}
		Scope* scope = &actions.GetTable(index);
		Action* action = scope->As<Action>();
		assert(action != nullptr);

		return *action;
	}

	Datum& ActionList::Actions()
	{
		Datum* actions = Find("Actions");
		assert(actions != nullptr);
		return *actions;
	}

	gsl::owner<Attributed*> ActionList::Clone() const
	{
		return new ActionList();
	}


	const Vector<Signature> ActionList::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumType::String, 1, offsetof(ActionList, _name)},
			{"Actions", Datum::DatumType::Table, 0, 0}
		};
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		const auto rhsActionList = rhs->As<ActionList>();
		if (rhsActionList == nullptr)
		{
			return false;
		}

		return _name == rhsActionList->_name &&
			(*this)["Actions"s] == (*rhsActionList)["Actions"s];
	}

	std::string ActionList::ToString() const
	{
		return "ActionList"s;
	}
}