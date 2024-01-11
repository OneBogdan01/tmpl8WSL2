#pragma once


#include "Camera.h"
#include "KeyboardManager.h"
#include "PlayerCharacter.h"
#include "Triangle.h"
#include "animator/Animation.h"
#include "lighting/LightManager.h"
#include "physics/World.h"
#include "lighting/Sun.h"
#include "model_loading/SkinnedModel.h"
#include "skybox/Skybox.h"
#include <animator/Animator.h>

#include "imgui.h"
#include "tiles/ExplodingBarrelsFactory.h"
#include "utilities/Button.h"
#include "utilities/GameStateManager.h"
#include "utilities/Menu.h"
class StaticModel;
class Camera;
class ChunkManager;
// Function to convert btVector3 to glm::vec3
inline glm::vec3 BtVector3ToGlm(const btVector3& vector)
{
	return glm::vec3(vector.x(), vector.y(), vector.z());
}

inline btVector3 GlmToBtVector3(const glm::vec3& vector)
{
	return btVector3(vector.x, vector.y, vector.z);
}

namespace Tmpl8
{
	class Game
	{
	public:
		// game flow methods
		void Init();
		void Update(float deltaTime);
		void Render();
		void CreateMenu();
		void DisplayDebugInfo();
		void Tick(float deltaTime);

		void Shutdown();

#ifdef _WINDOWS
		void KeyDown(int key);
		void KeyUp(int key);
#else
		void KeyDown(XID key);
		void KeyUp(XID key);

#endif


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
		inline static LightManager* lightManager = nullptr;

		inline static ChunkManager* tileLoader = nullptr;
		Sun sun;
		Skybox skybox;
		PlayerCharacter* player = nullptr;
		inline static GameStateManager gameState;
		btVector3 startingPlayerPosition = { 0, 5, 0 };
		inline static KeyboardManager inputManager;
		inline static bool freeCam = false;
		ExplodingBarrelsFactory* explodingBarrelsFactory = nullptr;
		Menu menu;
		Button pauseButton;
	};
} // namespace Tmpl8
