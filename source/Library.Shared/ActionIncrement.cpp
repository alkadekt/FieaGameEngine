#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement)

	ActionIncrement::ActionIncrement() :
		Action(ActionIncrement::TypeIdClass())
	{
	}

	void ActionIncrement::Update(const GameTime& /*time*/)
	{
		if (_target.empty() == false)
		{
			_targetDatum = SearchForTargetDatum(_target);
			assert(_targetDatum != nullptr);

			_targetDatum->SetFloat(_targetDatum->GetFloat() + _step);
		}		
	}

	gsl::owner<Attributed*> ActionIncrement::Clone() const
	{
		return new ActionIncrement();
	}

	const Vector<Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(ActionIncrement, _name) },
			{ "Target", Datum::DatumType::String, 1, offsetof(ActionIncrement, _target) },
			{ "Step", Datum::DatumType::Float, 1, offsetof(ActionIncrement, _step)}
		};
	}

	bool ActionIncrement::Equals(const RTTI* rhs) const
	{
		const auto rhsActionIncrement = rhs->As<ActionIncrement>();
		if (rhsActionIncrement == nullptr)
		{
			return false;
		}

		return _name == rhsActionIncrement->_name;
	}

	std::string ActionIncrement::ToString() const
	{
		return "ActionIncrement"s;
	}

	const std::string& ActionIncrement::Target() const
	{
		return _target;
	}

	void ActionIncrement::SetTarget(const std::string& target)
	{
		_target = target;
	}

	float ActionIncrement::Step() const
	{
		return _step;
	}
}