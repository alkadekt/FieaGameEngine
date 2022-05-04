#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Action to destroy an action based on a class name and instance.
	/// </summary>
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Default copy constructor.
		/// </summary>
		ActionDestroyAction(const ActionDestroyAction& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		ActionDestroyAction& operator=(const ActionDestroyAction & other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		ActionDestroyAction(ActionDestroyAction && other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		ActionDestroyAction& operator=(ActionDestroyAction && other) = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		~ActionDestroyAction() = default;

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
		/// Return ref to name of class to destroy.
		/// </summary>
		const std::string& ClassName() const;

		/// <summary>
		/// Set the name of the class to destroy.
		/// </summary>
		void SetClassName(const std::string& name);

		/// <summary>
		/// Get name for instance of class to destroy.
		/// </summary>
		const std::string& InstanceName() const;

		/// <summary>
		/// Set name to use to find instance to destroy.
		/// </summary>
		void SetInstanceName(const std::string& name);

	private:
		std::string _className;
		std::string _instanceName;
		Scope* _targetScope;
	};

	ConcreteFactory(ActionDestroyAction, RTTI)
}