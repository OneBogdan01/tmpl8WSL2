#pragma once
#include <string>

#include "GameStateManager.h"
#include "imgui.h"

//most of this class is made by following Erik Cupark's guide
class Menu
{
public:
	void Init();
	void ChangeMenu(GameStateManager::GameState state);
	void Draw();
	ImFont* fontTitle;
	ImFont* fontBody;
	std::string menuTitle;
	std::string loadingText;
	//button functionality
	std::string startGame;
	std::string exitGame;
	//std::string paused;
	std::string backToMainMenu;
	std::string dif1;
	std::string dif2;
	std::string dif3;
	std::string endless;
	void ResetButtons();

private:
	float difficultySpeed1 = 13.0f;
	float difficultySpeed2 = 15.0f;
	float difficultySpeed3 = 20.0f;
};
