#pragma once


#include "Camera.h"
#include "KeyboardManager.h"
#include "PlayerCharacter.h"
#include "Triangle.h"
#include "lighting/LightManager.h"
#include "physics/World.h"
#include "lighting/Sun.h"
#include "md2_loader/Md2.h"
#include "skybox/Skybox.h"
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
		Shader* animationShader = nullptr;

		//Triangle triangle;
		inline static Camera* camera = nullptr;
		inline static World world;
		inline static LightManager* lightManager = nullptr;

		ChunkManager* tileLoader = nullptr;
		Sun sun;
		Shader* modelShader = nullptr;
		Skybox skybox;
		PlayerCharacter* player = nullptr;
		btVector3 startingPlayerPosition = {0, 5, 0};
		inline static KeyboardManager inputManager;
		inline static bool freeCam = false;
	};
} // namespace Tmpl8
