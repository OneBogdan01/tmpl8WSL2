#pragma once
#include "Triangle.h"
#include "World.h"

class Camera;

namespace Tmpl8
{
	class Game
	{
	public:
		// game flow methods
		void Init();
		void Tick(float deltaTime);

		void Shutdown()
		{
			/* implement if you want to do something on exit */
		}

		void KeyDown(XID key);
		void KeyUp(XID key);
		void MouseScroll(float x);
		void MouseDown(unsigned button);
		void MouseUp(unsigned button);
		void MouseMove(int x, int y);

		// data members
		//Surface* screen;
		//TODO less big array just to store input
		int keystate[66666];

		float vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		static glm::mat4 perspective;
		static glm::mat4 view;
		Shader* simpleShader = nullptr;
		Triangle triangle;
		Camera* camera = nullptr;
		World world;
	};
} // namespace Tmpl8
