#ifdef _WINDOWS
//fixes ambigous symbol error
#define _HAS_STD_BYTE 0
#endif

#include "game.h"

#include "tiles/ChunkManager.h"
#include "imgui.h"
#include "tiles/ModelTileFactory.h"
#include "utilities/RandomNumberGenerator.h"
#include <model_loading/SkinnedModel.h>
#include <animator/Animation.h>
#include <animator/Animator.h>

#include "tiles/Rope.h"
#include "utilities/RandomNames.h"
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
glm::mat4 Game::perspective;
glm::mat4 Game::view;
void Game::Init()
{
	menu.Init();
	lightShader = new Shader(
		"assets/shaders/BasicVertexShader.vert",
		"assets/shaders/SolidColor.frag");
	lightManager = new LightManager(lightShader);
	//this gets random every run
	RandomNumberGenerator::seed = RandomNumberGenerator::InitSeed(time(nullptr));
	world.Init();
	tileLoader = new ChunkManager();
	//from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/37494654#37494654
	tileLoader->Init();


	camera = new Camera();
	camera->Init();

	skybox.Init();
	player = new PlayerCharacter(world.GetDynamicWorld(), startingPlayerPosition);
	player->SetUpModel();

	////random names test
	//for (int j = 0; j < 100; j++) {
	//	for (int i = 0; i < 4; i++) {
	//		//cast it to all patterns avaiable
	//		cout << RandomNames::GetRandomName(static_cast<RandomNames::Pattern>(i)) << endl;;
	//	}
	//	cout << endl;
	//}
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------


float mixing = .2f;

glm::vec3 position = glm::vec3(0);
float fov = 45;
float yOffset = 0;

glm::vec2 rotateCam = glm::vec2(0);
glm::vec2 moveCam = glm::vec2(0);

float f = 0.3f;
char buf[] = "some windows";
float FPS = 0;
uint frameCount = 0;
Timer FPSTimer;
Timer FixedTimer;
const float FIXED_TIMESTEP = 1 / 60.0f;

#ifdef _WINDOWS
#include "windows.h"
#include "psapi.h"

#else
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

int parseLine(char* line)
{
	// This assumes that a digit will be found and the line ends in " Kb".
	int i = strlen(line);
	const char* p = line;
	while (*p < '0' || *p > '9') p++;
	line[i - 3] = '\0';
	i = atoi(p);
	return i;
}

namespace virtualMemory
{
	int getValue()
	{
		//Note: this value is in KB!
		FILE* file = fopen("/proc/self/status", "r");
		int result = -1;
		char line[128];

		while (fgets(line, 128, file) != NULL)
		{
			if (strncmp(line, "VmSize:", 7) == 0)
			{
				result = parseLine(line);
				break;
			}
		}
		fclose(file);
		return result;
	}
}

namespace physicalMemory
{
	int getValue()
	{
		//Note: this value is in KB!
		FILE* file = fopen("/proc/self/status", "r");
		int result = -1;
		char line[128];

		while (fgets(line, 128, file) != NULL)
		{
			if (strncmp(line, "VmRSS:", 6) == 0)
			{
				result = parseLine(line);
				break;
			}
		}
		fclose(file);
		return result;
	}
}
#endif

void Game::Update(float deltaTime)
{
	inputManager.Update();

	switch (gameState.GetState())
	{
	case GameStateManager::PLAYING:
		//update physics


		world.Update(deltaTime);

		//TODO encapsulate camera init
		if (f > 1)
			f = 0;
		camera->RotateMouse(rotateCam);
		camera->MoveX(moveCam.x);
		camera->MoveZ(moveCam.y);
		fov -= yOffset;
		if (fov < 1.0f)
			fov = 1.0f;
		if (fov > 45.0f)
			fov = 45.0f;
		yOffset = 0;


		camera->Update(deltaTime);

		perspective = glm::perspective(glm::radians(fov),
			static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
			0.1f, 100.0f);

		view = camera->GetViewMat();


		//TODO make update loop for entities
		tileLoader->Update(deltaTime);

		player->Update(deltaTime);
		break;
	case GameStateManager::LOSE:
		break;
	case GameStateManager::PAUSE:
		break;
	case GameStateManager::START_MENU:
		break;
	default: break;
	}


}
void Game::Render()
{
	switch (gameState.GetState())
	{

	case GameStateManager::LOSE:
		break;
	case GameStateManager::PLAYING:
		lightManager->Draw();
		tileLoader->DrawChunks();
		player->Draw();
		skybox.Draw();
		ImGui::PushFont(menu.fontTitle);
		pauseButton.Draw();

		ImGui::PopFont();

		break;
	case GameStateManager::PAUSE:
		lightManager->Draw();
		tileLoader->DrawChunks();
		player->Draw();
		skybox.Draw();
		menu.menuTitle = "Paused";
		menu.startGame = "Resume";
		menu.backToMainMenu = "Main menu";
		menu.exitGame = "Exit";
		CreateMenu();
		break;
		//Erik Cupak made a guide about how to setup an imgui menu
	case GameStateManager::START_MENU:
		menu.menuTitle = "Endless Pink";
		//menu.startGame = "Start Game";
		if (menu.dif1.length() == 0) {
			menu.dif1 = "[Easy] " + RandomNames::GenerateRandomName();
			menu.dif2 = "[Medium] " + RandomNames::GenerateRandomName();
			menu.dif3 = "[Hard] " + RandomNames::GenerateRandomName();
			menu.endless = "[Endless] " + RandomNames::GenerateRandomName();
		}
		
		menu.exitGame = "Exit";
		CreateMenu();
		break;

	}



}
void Game::CreateMenu()
{
	menu.Draw();
}
void Game::DisplayDebugInfo()
{
	frameCount++;
	if (FPSTimer.elapsed() >= 1.0f)
	{
		FPS = static_cast<float>(frameCount) / FPSTimer.elapsed();
		frameCount = 0;
		FPSTimer.reset();
	}


	ImGui::Text("FPS:%f", FPS);
	ImGui::Checkbox("Free Camera", &freeCam);
	ImGui::Text("Camera position: %f, %f, %f", camera->GetPosition().x, camera->GetPosition().y,
		camera->GetPosition().z);

	/*ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Button"))
		f += deltaTime;*/

		//from this post on memory usage https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process

#ifdef _WINDOWS


	PROCESS_MEMORY_COUNTERS_EX pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsed = pmc.PrivateUsage;

	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	SIZE_T physMemUsed = pmc.WorkingSetSize;


#else

	//convert to byes
	long long virtualMemUsed = virtualMemory::getValue() * 1000;

	long long physMemUsed = physicalMemory::getValue() * 1000;


#endif
	// Convert from bytes to megabytes
	const double virtualMemUsedMB = virtualMemUsed / 1024.0 / 1024.0;
	const double physMemUsedMB = physMemUsed / 1024.0 / 1024.0;

	ImGui::Text("Virt mem: %.2f MB", virtualMemUsedMB);
	ImGui::Text("Phys mem: %.2f MB", physMemUsedMB);
	world.RenderDebug();
}

void Game::Tick(float deltaTime)
{

	Update(deltaTime);
	//displaying stuff
#ifdef __DEBUG__
	DisplayDebugInfo();

#endif

	Render();
}

void Game::Shutdown()
{
	delete simpleShader;
	delete tileLoader;
	delete camera;
}
#ifdef _WINDOWS
void Game::KeyDown(int key)
{
	inputManager.KeyPressed(key);
}

void Game::KeyUp(int key)
{
	inputManager.KeyReleased(key);
}
#else

void Game::KeyDown(const KeySym keycode)
{
	inputManager.KeyPressed(keycode);
}


void Game::KeyUp(const KeySym keycode)
{
	inputManager.KeyReleased(keycode);
}
#endif


void Game::MouseScroll(float x)
{
}

void Game::MouseDown(unsigned button)
{
}

void Game::MouseUp(unsigned button)
{
}

void Game::MouseMove(int x, int y)
{
}
