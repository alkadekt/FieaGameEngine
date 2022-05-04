#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Increments a target float value on update by the given step value.
	/// </summary>
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		ActionIncrement(const ActionIncrement& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		ActionIncrement& operator=(const ActionIncrement& other) = default;
		
		/// <summary>
		/// Default move constructor.
		/// </summary>
		ActionIncrement(ActionIncrement&& other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		ActionIncrement& operator=(ActionIncrement&& other) = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Updates this action object based on time.
		/// </summary>
		/// <param name="time"></param>
		virtual void Update(const GameTime& time) override;

		/// <summary>
		/// Creates a heap allocated copy of this class.
		/// </summary>
		/// <returns></returns>
		gsl::owner<Attributed*> Clone() const override;

		/// <summary>
		/// Returns list of prescribed attributes for this class.
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Returns whether this class and given ActionIncrement are equivalent.
		/// </summary>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns name of this class.
		/// </summary>
		std::string ToString() const override;

		/// <summary>
		/// Returns the current target of increment.
		/// </summary>
		const std::string& Target() const;

		/// <summary>
		/// Sets the current target of increment.
		/// </summary>
		void SetTarget(const std::string& target);

		/// <summary>
		/// Returns the value added at each update.
		/// </summary>
		float Step() const;

	private:
		std::string _target;
		float _step{ 1.0f };
		Datum* _targetDatum{ nullptr };
	};

	ConcreteFactory(ActionIncrement, RTTI)
}