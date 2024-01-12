#include "Menu.h"

#include <string>

#include "game.h"
#include "imgui.h"
#include "template.h"
#include "tiles/ChunkManager.h"

void Menu::Init()
{
	const ImGuiIO& io = ImGui::GetIO();
	fontTitle = io.Fonts->AddFontFromFileTTF("assets/PixelifySans-Regular.ttf", 32.0f, NULL,
	                                         io.Fonts->GetGlyphRangesDefault());
	fontBody = io.Fonts->AddFontFromFileTTF("assets/PixelifySans-Regular.ttf", 18.0f, NULL,
	                                        io.Fonts->GetGlyphRangesDefault());
	ResetButtons();
}

void Menu::ChangeMenu(GameStateManager::GameState state)
{
	Game::gameState.SetState(state);
	ResetButtons();
}

void Menu::Draw()
{
	// Button setup.
	const ImVec2 buttonSize = {200.0f, 40.0f};
	{
		const ImVec4 normalColor = {0.9f, 0.1f, 0.2f, 1.0f};
		const ImVec4 hoverColor = {0.9f, 0.1f, 0.2f, 0.5f};
		const ImVec4 activeColor = {1.f, 0.2f, 0.2f, 1.0f};

		ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
	}

	// Window setup.


	ImGui::SetNextWindowSize({CENTER_X, CENTER_Y * 1.5f});

	ImVec2 position = {CENTER_X, CENTER_Y};
	position.x -= CENTER_X / 2;
	position.y -= CENTER_Y / 2;
	ImGui::SetNextWindowPos(position);

	ImGui::SetNextWindowBgAlpha(0.75f); // Use this to apply transparency.
	const ImVec4 normalColor = {0.1f, 0.1f, 0.5f, 1.0f}; // Don't use this alpha channel to set transparency.
	ImGui::PushStyleColor(ImGuiCol_WindowBg, normalColor);


	// Calculate how much extra space there is in the window.
	// The title and the title font are pulled up to here, so we can get the height of the text.
	ImGui::PushFont(fontTitle);
	const std::string text = menuTitle;
	const float textWidth = ImGui::CalcTextSize(text.c_str()).x;
	const float titleHeight = ImGui::CalcTextSize(text.c_str()).y;

	const float totalButtonHeight = buttonSize.y * 6;
	float remainingHeight = CENTER_Y - titleHeight - totalButtonHeight;

	// Now we take the remaining height and split it up so we can place equal amounts of space between the ImGui elements.
	// The x value is not important in this situation.
	ImVec2 gapSize = {10.0f, remainingHeight * 0.25f};

	// Window contents.
	ImGui::Begin("Menu", NULL,
	             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
	             ImGuiWindowFlags_NoSavedSettings);
	{
		const float windowWidth = ImGui::GetWindowSize().x;

		//title
		ImGui::Dummy({gapSize.x, gapSize.y * 0.5f});
		{
			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			ImGui::Text(text.c_str());
			ImGui::PopFont();
		}
		ImGui::Dummy({gapSize.x, gapSize.y * 0.5f});

		// Display buttons.
		{
			const float offset = (windowWidth - buttonSize.x) * 0.5f;
			ImGui::PushFont(fontBody);

			ImGui::BeginGroup();
			//start drawing 
			{
				ImGui::SetCursorPosX(offset);

				if (startGame.length() > 0)
					// The button will return True when it is clicked.
					if (ImGui::Button(startGame.c_str(), buttonSize))
					{
						// Code you want to do when the button is clicked.
						ChangeMenu(GameStateManager::PLAYING);
					}
				ImGui::Dummy(gapSize);
				ImGui::SetCursorPosX(offset);
				if (loadingText.length() > 0)
				{
					ImGui::SetWindowFontScale(3.f); //
					ImGui::Text(loadingText.c_str());
					ImGui::SetWindowFontScale(1); //
				}
				ImGui::Dummy(gapSize);
				ImGui::SetCursorPosX(offset);

				if (dif1.length() > 0)
					if (ImGui::Button(dif1.c_str(), buttonSize))
					{
						// Code you want to do when the button is clicked.
						ChangeMenu(GameStateManager::LOADING);
						Game::tileLoader->SetDirectionZ(difficultySpeed1);
					}
				ImGui::Dummy(gapSize);
				ImGui::SetCursorPosX(offset);
				if (dif2.length() > 0)
					if (ImGui::Button(dif2.c_str(), buttonSize))
					{
						// Code you want to do when the button is clicked.
						ChangeMenu(GameStateManager::LOADING);
						Game::tileLoader->SetDirectionZ(difficultySpeed2);
					}
				ImGui::Dummy(gapSize);
				ImGui::SetCursorPosX(offset);
				if (dif3.length() > 0)
					if (ImGui::Button(dif3.c_str(), buttonSize))
					{
						ChangeMenu(GameStateManager::LOADING);
						Game::tileLoader->SetDirectionZ(difficultySpeed3);
					}
				ImGui::Dummy(gapSize);
				ImGui::SetCursorPosX(offset);
				if (endless.length() > 0)
					if (ImGui::Button(endless.c_str(), buttonSize))
					{
						ChangeMenu(GameStateManager::LOADING);
						Game::tileLoader->SetEndless(difficultySpeed3);
					}
				ImGui::Dummy(gapSize);

				ImGui::SetCursorPosX(offset);

				if (backToMainMenu.length() > 0)
					if (ImGui::Button(backToMainMenu.c_str(), buttonSize))
					{
						ChangeMenu(GameStateManager::START_MENU);
					}
				ImGui::Dummy(gapSize);

				ImGui::SetCursorPosX(offset);
				if (exitGame.length() > 0)
					if (ImGui::Button(exitGame.c_str(), buttonSize))
					{
						// Code you want to do when the button is clicked.
						ExitGame();
					}
				ImGui::Dummy(gapSize);
			}
			ImGui::EndGroup();
			ImGui::PopFont();
		}
	}
	ImGui::End();
	ImGui::PopStyleColor(4);
}

void Menu::ResetButtons()
{
	loadingText = "";
	menuTitle = "";
	startGame = "";
	exitGame = "";
	//paused = "";
	backToMainMenu = "";
	endless = "";
	dif1 = "";
	dif2 = "";
	dif3 = "";
}
