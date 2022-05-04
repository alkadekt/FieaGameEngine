#pragma once
#include "Action.h"
#include "Vector.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action to create an action based on a class name and instance.
	/// Creates action underneath this action's parent.
	/// </summary>
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		ActionCreateAction(const ActionCreateAction& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		ActionCreateAction(ActionCreateAction&& other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		ActionCreateAction& operator=(ActionCreateAction&& other) = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Calls update on each Action contained within the list.
		/// </summary>
		void Update(const GameTime& time) override;

		/// <summary>
		/// Creates a heap allocated copy of this class.
		/// </summary>
		virtual gsl::owner<Attributed*> Clone() const override;

		/// <summary>
		/// Returns list of prescribed attributes for this class.
		/// </summary>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Returns whether this instance and given ActionList instance are equivalent.
		/// </summary>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns name of this class.
		/// </summary>
		std::string ToString() const override;

		/// <summary>
		/// Return ref to name of class to create.
		/// </summary>
		const std::string& ClassName() const;

		/// <summary>
		/// Set the name of the class to create.
		/// </summary>
		void SetClassName(const std::string& name);

		/// <summary>
		/// Get name for instance of class created.
		/// </summary>
		const std::string& InstanceName() const;

		/// <summary>
		/// Set name to give created class instance.
		/// </summary>
		void SetInstanceName(const std::string& name);

	private:
		std::string _className;
		std::string _instanceName;
	};

	ConcreteFactory(ActionCreateAction, RTTI)
}