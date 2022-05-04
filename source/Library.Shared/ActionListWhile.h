#pragma once
#include "ActionList.h"

namespace FieaGameEngine
{
	class ActionListWhile final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListWhile, ActionList)

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionListWhile();

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		ActionListWhile(const ActionListWhile& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		ActionListWhile& operator=(const ActionListWhile& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		ActionListWhile(ActionListWhile&& other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		ActionListWhile& operator=(ActionListWhile&& other) = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionListWhile() = default;

		/// <summary>
		/// Executes the contained actions if condition is true.
		/// </summary>
		virtual void Update(const GameTime& time);

		/// <summary>
		/// Returns the signatures of each data member for this class.
		/// </summary>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Creates a heap allocated copy of this class.
		/// </summary>
		gsl::owner<Attributed*> Clone() const override;

		/// <summary>
		/// Returns whether this and GameObject passed through RTTI pointer are equal.
		/// </summary>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns stringified version of this class.
		/// </summary>
		/// <returns></returns>
		std::string ToString() const override;

		/// <summary>
		/// Returns current condition status.
		/// </summary>
		bool Condition() const;

		/// <summary>
		/// Sets the current condition status.
		/// </summary>
		void SetCondition(bool value);

	private:
		std::uint32_t _condition{ 0 };
	};

	ConcreteFactory(ActionListWhile, RTTI)
}