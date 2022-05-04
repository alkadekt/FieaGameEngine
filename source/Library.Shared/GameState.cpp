#include "pch.h"
#include "GameState.h"

namespace FieaGameEngine
{
	void GameState::Update(GameTime time)
	{
		_sceneRoot->Update(time);
	}

	void GameState::EnqueueAction(Action* action)
	{
		_actionsQueue.PushBack(action);
	}
}