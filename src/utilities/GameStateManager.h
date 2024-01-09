#pragma once

class GameStateManager
{
public:
	enum GameState
	{
		PLAYING,
		LOSE,
		PAUSE,
		START_MENU,
		CHOOSE_DIFFICULTY
	};


	GameState GetState() const;

	void SetState(GameState _state);

private:
	GameState state = START_MENU;
};
