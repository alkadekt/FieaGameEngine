#include "pch.h"
#include "ActionListWhile.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionListWhile)

	ActionListWhile::ActionListWhile() :
		ActionList(ActionListWhile::TypeIdClass())
	{
	}

	void ActionListWhile::Update(const GameTime& time)
	{
		if (_condition == 1)
		{
			for (std::size_t i = 0; i < Find("Actions")->Size(); ++i)
			{
				GetAction(i).Update(time);
			}
		}
	}

	const Vector<Signature> ActionListWhile::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(ActionListWhile, _name) },
			{ "Condition", Datum::DatumType::Integer, 1, offsetof(ActionListWhile, _condition) },
			{ "Actions", Datum::DatumType::Table, 0, 0 }
		};
	}

	gsl::owner<Attributed*> ActionListWhile::Clone() const
	{
		return new ActionListWhile();
	}

	bool ActionListWhile::Equals(const RTTI* rhs) const
	{
		const auto rhsActionListWhile = rhs->As<ActionListWhile>();
		if (rhsActionListWhile == nullptr)
		{
			return false;
		}

		return _name == rhsActionListWhile->_name;
	}

	std::string ActionListWhile::ToString() const
	{
		return "ActionListWhile"s;
	}

	bool ActionListWhile::Condition() const
	{
		return _condition == 1;
	}

	void ActionListWhile::SetCondition(bool value)
	{
		if (value)
		{
			_condition = 1;
		}
		else
		{
			_condition = 0;
		}
	}
}