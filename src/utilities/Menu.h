#pragma once
#include <string>

#include "GameStateManager.h"
#include "imgui.h"

class Menu
{
public:
	void Init();
	void ChangeMenu(GameStateManager::GameState state);
	void Draw();
	ImFont* fontTitle;
	ImFont* fontBody;
	std::string menuTitle;
	//button functionality
	std::string startGame;
	std::string exitGame;
	//std::string paused;
	std::string backToMainMenu;
	std::string dif1;
	std::string dif2;
	std::string dif3;
	void ResetButtons();

private:
};
