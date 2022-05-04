#pragma once
#include "Action.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// An ActionList is a list of Actions that can update as one unit.
	/// </summary>
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		ActionList();

	protected:
		/// <summary>
		/// Constructor for dervied classes to pass id up to Attributed.
		/// </summary>
		ActionList(RTTI::IdType id);

	public:
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		ActionList(const ActionList& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		ActionList& operator=(const ActionList& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		ActionList(ActionList&& other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		ActionList& operator=(ActionList&& other) = default;

		/// <summary>
		/// Default destructor.
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// Calls update on each Action contained within the list.
		/// </summary>
		void Update(const GameTime& time) override;

		/// <summary>
		/// Creates a new action instance within this list.
		/// </summary>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns Action at given index.
		/// </summary>
		Action& GetAction(std::size_t index);

		/// <summary>
		/// Returns datum holding the actions of this list.
		Datum& Actions();

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
	};

	ConcreteFactory(ActionList, RTTI)
}