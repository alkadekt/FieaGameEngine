#include "pch.h"
#include "GameObject.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject)

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{
	}

	GameObject::~GameObject()
	{
	}

	Datum& GameObject::Children()
	{
		Datum* children = Find("Children");
		assert(children != nullptr);
		return *children;
	}

	GameObject& GameObject::GetChild(std::size_t index)
	{
		Datum* children = Find("Children");
		assert(children != nullptr);
		if (index >= children->Size())
		{
			throw std::runtime_error("Invalid index for children.");
		}
		GameObject& child = static_cast<GameObject&>(children->GetTable(index));
		return child;
	}

	Action* GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		RTTI* rtti = Factory<RTTI>::Create(className);
		assert(rtti != nullptr);
		Action* actionPtr = rtti->As<Action>();
		assert(actionPtr != nullptr);

		actionPtr->SetName(instanceName);
		Adopt(*actionPtr, "Actions");
		return actionPtr;
	}

	Datum& GameObject::Actions()
	{
		Datum* actions = Find("Actions");
		assert(actions != nullptr);
		return *actions;
	}

	Action& GameObject::GetAction(std::size_t index)
	{
		Datum* actions = Find("Actions");
		assert(actions != nullptr);
		if (index >= actions->Size())
		{
			throw std::runtime_error("Invalid index for actions.");
		}
		Action& child = static_cast<Action&>(actions->GetTable(index));
		return child;
	}

	void GameObject::Update(const GameTime& time)
	{
		assert(Find("Children") != nullptr);
		assert(Find("Actions") != nullptr);

		for (std::size_t i = 0; i < Find("Children")->Size(); ++i)
		{
			GetChild(i).Update(time);
		}

		for (std::size_t i = 0; i < Find("Actions")->Size(); ++i)
		{
			GetAction(i).Update(time);
		}
	}

	const Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumType::String, 1, offsetof(GameObject, Name) },
			{ "Position", Datum::DatumType::Vector, 1, offsetof(GameObject, Position) },
			{ "Rotation", Datum::DatumType::Vector, 1, offsetof(GameObject, Rotation) },
			{ "Scale", Datum::DatumType::Vector, 1, offsetof(GameObject, Scale) },
			{ "Children", Datum::DatumType::Table, 0, 0 },
			{ "Actions", Datum::DatumType::Table, 0, 0 }
		};
	}

	gsl::owner<GameObject*> GameObject::Clone() const
	{
		return new GameObject(*this);
	}

	bool GameObject::Equals(const RTTI* rhs) const
	{
		const auto rhsGameObject = rhs->As<GameObject>();
		if (rhsGameObject == nullptr)
		{
			return false;
		}

		return Name == rhsGameObject->Name &&
			Position == rhsGameObject->Position &&
			Rotation == rhsGameObject->Rotation &&
			Scale == rhsGameObject->Scale &&
			(*this)["Children"s] == (*rhsGameObject)["Children"s];
	}

	std::string GameObject::ToString() const
	{
		return "GameObject"s;
	}

	void GameObject::PushOnActionQueue(Action* action)
	{
		if (action == nullptr)
		{
			throw std::runtime_error("Cannot add nullptr action to queue.");
		}
		_actionQueue.PushBack(action);
	}
}