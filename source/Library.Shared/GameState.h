#pragma once
#include "GameTime.h"
#include "Action.h"
#include "Vector.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	class GameState final
	{
	public:
		void Update(GameTime time);
		void EnqueueAction(Action* action);

	private:
		GameObject* _sceneRoot;
		Vector<Action*> _actionsQueue;
	};
}