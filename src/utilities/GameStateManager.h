#pragma once
#include "EventArgs.h"

class GameStateManager
{
public:
	enum GameState
	{
		PLAYING,
		LOSE,
		PAUSE,
		START_MENU,
		LOADING
	};


	GameState GetState() const;

	void SetState(GameState _state);
	VoidEvent resetGame;
	VoidEvent loadingGame;

private:
	GameState state = START_MENU;
};
