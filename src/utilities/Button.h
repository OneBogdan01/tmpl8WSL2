#pragma once
#include <string>

#include "imgui.h"

class Button
{
public:
	const ImVec2 buttonSize = {40.0f, 40.0f};
	void Draw();
	ImVec2 buttonOffset = {10, 10};
	ImVec2 gapSize = {0, 0};
	void SetName(std::string& _name);

private:
	// Button setup.
	std::string name = "||";

	const ImVec4 normalColor = {0.9f, 0.1f, 0.2f, 1.0f};
	const ImVec4 hoverColor = {0.9f, 0.1f, 0.2f, 0.5f};
	const ImVec4 activeColor = {1.f, 0.2f, 0.2f, 1.0f};
};
