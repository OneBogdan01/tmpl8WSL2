#include "Button.h"

#include "game.h"
#include "template.h"

void Button::Draw()
{
	ImGui::PushStyleColor(ImGuiCol_Button, normalColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoverColor);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);

	//window setup
	ImGui::SetNextWindowSize({buttonSize.x, buttonSize.y});

	ImVec2 pos = {SCRWIDTH, SCRHEIGHT};
	pos.x -= buttonSize.x * 2;
	pos.y -= buttonSize.y * 2;
	ImGui::SetNextWindowPos(pos);

	ImGui::SetNextWindowBgAlpha(0.f); // Use this to apply transparency.
	// Use this to apply transparency.
	const ImVec4 windowColor = {0.1f, 0.1f, 0.5f, 0.0f}; // Don't use the alpha channel for transparency.
	ImGui::PushStyleColor(ImGuiCol_WindowBg, windowColor);
	ImGui::PushStyleColor(ImGuiCol_Border, windowColor);

	ImGui::Begin(" ", NULL,
	             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
	             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse |
	             ImGuiWindowFlags_NoSavedSettings);

	//ImGui::Dummy(gapSize);

	/*const float offset = (buttonSize.x) * 0.01f;*/
	ImGui::SetCursorPosX(0);
	ImGui::SetCursorPosY(0);

	if (ImGui::Button(name.c_str(), buttonSize))
	{
		// Code you want to do when the button is clicked.
		//gameState.SetState(GameStateManager::PLAYING);
		Game::gameState.SetState(GameStateManager::PAUSE);
	}

	ImGui::End();

	ImGui::PopStyleColor(5);
}

void Button::SetName(std::string& _name)
{
	name = _name;
}
