#pragma once
#include "Attributed.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// A component attachable to a GameObject that enables a specific behavior.
	/// Action is abstract, so must use a concrete derived Action class.
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	protected:
		/// <summary>
		/// Constructor that passes id from derived classes up to Attributed.
		/// </summary>
		Action(RTTI::IdType id);

	public:
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		Action(const Action& other) = default;

		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		Action& operator=(const Action& other) = default;

		/// <summary>
		/// Default move constructor.
		/// </summary>
		Action(Action&& other) = default;

		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		Action& operator=(Action&& other) = default;

		/// <summary>
		/// Default virtual destructor.
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Updates this action object based on time.
		/// </summary>
		/// <param name="time"></param>
		virtual void Update(const GameTime& time) = 0;

		/// <summary>
		/// Returns list of prescribed attributes for this class.
		/// </summary>
		/// <returns></returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Returns whether this class and given Action are equivalent.
		/// </summary>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns name of this class.
		/// </summary>
		std::string ToString() const override;

		/// <summary>
		/// Returns Name attribute of this class.
		/// </summary>
		const std::string& Name();

		/// <summary>
		/// Sets the Name attribute of this class.
		/// </summary>
		/// <param name="name"></param>
		void SetName(const std::string& name);

	protected:
		std::string _name;
	};
}