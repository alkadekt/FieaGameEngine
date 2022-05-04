#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class Action;

	/// <summary>
	/// Concrete class with Name, Transform, and Children prescribed attributes.
	/// Used for describing a scene hierarchy.
	/// </summary>
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed)

	public:

		/// <summary>
		/// Contstructor that calls Attributed's constructor to populate signatures.
		/// </summary>
		GameObject();

		/// <summary>
		/// Defaulted copy constructor.
		/// </summary>
		GameObject(const GameObject& other) = default;

		/// <summary>
		/// Defaulted copy assignment operator.
		/// </summary>
		GameObject& operator=(const GameObject& other) = default;

		/// <summary>
		/// Defaulted move constructor.
		/// </summary>
		GameObject(GameObject&& other) noexcept = default;

		/// <summary>
		/// Defaulted move assignment operator.
		/// </summary>
		GameObject& operator=(GameObject&& other) noexcept = default;

		/// <summary>
		/// Destructor. Does no work.
		/// </summary>
		virtual ~GameObject() override;

		std::string Name;
		glm::vec4 Position;
		glm::vec4 Rotation;
		glm::vec4 Scale;

		/// <summary>
		/// Returns datum holding the children of this GameObject.
		/// </summary>
		Datum& Children();

		/// <summary>
		/// Returns reference to child at given index.
		/// </summary>
		GameObject& GetChild(std::size_t index = 0);

		/// <summary>
		/// Creates a new action instance within this gameobject under Actions attribute.
		/// </summary>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns datum holding the actions of this GameObject.
		/// </summary>
		Datum& Actions();

		/// <summary>
		/// Returns reference to action at given index.
		/// </summary>
		Action& GetAction(std::size_t index = 0);

		/// <summary>
		/// Calls update on all children.
		/// </summary>
		virtual void Update(const GameTime& time);
		
		/// <summary>
		/// Returns the signatures of each data member for this class.
		/// </summary>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Creates a heap allocated copy of this class.
		/// </summary>
		gsl::owner<GameObject*> Clone() const override;

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
		/// Adds the action to the action queue.
		/// </summary>
		void PushOnActionQueue(Action* action);

	protected:
		Vector<Action*> _actionQueue;
	};

	ConcreteFactory(GameObject, RTTI)
}