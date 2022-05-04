#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action)

	Action::Action(RTTI::IdType id) :
		Attributed(id)
	{
	}

	const Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{"Name", Datum::DatumType::String, 1, offsetof(Action, _name)}
		};
	}

	bool Action::Equals(const RTTI* rhs) const 
	{
		const auto rhsAction = rhs->As<Action>();
		if (rhsAction == nullptr)
		{
			return false;
		}

		return _name == rhsAction->_name;	
	}

	std::string Action::ToString() const
	{
		return "Action"s;
	}

	const std::string& Action::Name()
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}
}