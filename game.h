#pragma once


#include "Camera.h"
#include "KeyboardManager.h"
#include "PlayerCharacter.h"
#include "Triangle.h"
#include "World.h"
#include "lighting/Sun.h"
#include "md2_loader/Md2.h"
#include "skybox/Skybox.h"

class StaticModel;
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

		static glm::vec3 GetCameraPosition()
		{
			return camera->GetPosition();
		}

		static Camera& GetCamera()
		{
			return *camera;
		}

		static glm::vec3 GetLightPos()
		{
			return glm::vec3(-5.f, 1.0f, 2.0f);
		}

		static KeyboardManager& GetInputManager()
		{
			return inputManager;
		}

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
		//Triangle triangle;
		inline static Camera* camera = nullptr;
		inline static World world;
		TileLoader* tileLoader = nullptr;
		Sun sun;
		Shader* modelShader = nullptr;
		Skybox skybox;
		PlayerCharacter* player = nullptr;
		btVector3 startingPlayerPosition = {0, 0, 0};
		inline static KeyboardManager inputManager;
	};
} // namespace Tmpl8
