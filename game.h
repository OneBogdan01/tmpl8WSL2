#pragma once


#include "Triangle.h"
#include "World.h"
#include "lighting/Sun.h"
#include "md2_loader/Md2.h"
#include "skybox/Skybox.h"

class Model;
class Camera;
class TileLoader;

namespace Tmpl8
{
	class Game
	{
	public:
		// game flow methods
		void Init();
		void Tick(float deltaTime);

		void Shutdown();


		void KeyDown(XID key);
		void KeyUp(XID key);
		void MouseScroll(float x);
		void MouseDown(unsigned button);
		void MouseUp(unsigned button);
		void MouseMove(int x, int y);

		// data members
		//Surface* screen;
		//TODO less big array just to store input
		//int keystate[66666];

		float vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		static glm::mat4 perspective;
		static glm::mat4 view;
		Shader* simpleShader = nullptr;
		Shader* lightShader = nullptr;
		Triangle triangle;
		Camera* camera = nullptr;
		World world;
		TileLoader* tileLoader = nullptr;
		md2model::Md2 player = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
		Model* model = nullptr;
		Sun sun;
		Shader* modelShader = nullptr;
		Skybox skybox;
	};
} // namespace Tmpl8
