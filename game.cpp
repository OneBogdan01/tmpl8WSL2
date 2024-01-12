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
#include "utilities/MathLibrary.h"
#include "utilities/RandomNames.h"
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
glm::mat4 Game::perspective;
glm::mat4 Game::view;

void Game::Init()
{
	world.Init();

	menu.Init();
	lightShader = new Shader(
		"assets/shaders/BasicVertexShader.vert",
		"assets/shaders/SolidColor.frag");
	lightManager = new LightManager(lightShader);
	explodingBarrelsFactory = new ExplodingBarrelsFactory();

	//this gets random every run
	RandomNumberGenerator::seed = RandomNumberGenerator::InitSeed(time(nullptr));
	tileLoader = new ChunkManager();
	//from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/37494654#37494654
	tileLoader->Init();


	camera = new Camera();
	camera->Init();

	skybox.Init();
	player = new PlayerCharacter(world.GetDynamicWorld(), startingPlayerPosition);
	player->SetUpModel();
	gameState.resetGame.connect(&Game::ResetState, this);
	gameState.loadingGame.connect(&Game::LoadingGame, this);


	//loading stuff
	loadingShader = new Shader(
		"assets/shaders/ModelLoadingNoInstance.vert",
		"assets/shaders/ModelLoading.frag");

	lightManager->SetLightProperties(*loadingShader);
	//we are using the red channel
	loadingShader->Bind();

	loadingShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	loadingShader->SetFloat("material.shininess", 32.0f);
	loadingShader->Unbind();

	size_t count = tileLoader->modelPaths.size() - 1;
	const size_t index = static_cast<size_t>(RandomNumberGenerator::RandomFloat() * count);
	string path = tileLoader->modelPaths[index];

	loadingModel = ModelTileFactory::LoadModel(path.c_str(), glm::vec3(0));
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------

void Game::GoToMainMenu()
{
	gameState.SetState(GameStateManager::START_MENU);
}

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
const float seconds = 5.0f;

void Game::UpdateCam(float deltaTime)
{
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
}

void Game::Update(float deltaTime)
{
	inputManager.Update();

	switch (gameState.GetState())
	{
	case GameStateManager::PLAYING:
		//update physics


		world.Update(deltaTime);

		UpdateCam(deltaTime);


	//TODO make update loop for entities
		tileLoader->Update(deltaTime);

		player->Update(deltaTime);
		explodingBarrelsFactory->Update(deltaTime);
		break;
	case GameStateManager::LOSE:
		break;
	case GameStateManager::PAUSE:
		break;
	case GameStateManager::START_MENU:
		break;
	case GameStateManager::LOADING:
		UpdateCam(deltaTime);
		menu.menuTitle = "Loading...";


		currentProgress = MathLibrary::InvLerp(0, 100, currentTasks);
		char buffer[256]; // Adjust the buffer size accordingly
		std::sprintf(buffer, "%.2f", currentProgress * 100.0f);

	// Store the formatted string in a std::string
		std::string formattedString(buffer);
		menu.loadingText = formattedString;

		CreateMenu();

		if (currentProgress >= 1.0f)
		{
			currentProgress = 0;
			currentProgress = 0;
			menu.loadingText = "";
			gameState.SetState(GameStateManager::PLAYING);
		}
		else
		{
			currentTasks++;
			if (currentProgress > 0)
			{
#ifdef _WINDOWS
				//Sleep(seconds * 1000.0f);
#else

				//sleep(seconds);
#endif
			}
		}

		break;
	}
}

void Game::ResetState()
{
	std::cout << "Reseting '\n";
	player->ResetPosition();
	tileLoader->Reset();
	explodingBarrelsFactory->ResetState();
	currentTasks = 0;
	currentProgress = 0;
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
		explodingBarrelsFactory->Render();

		ImGui::PushFont(menu.fontTitle);
		pauseButton.Draw();

		ImGui::PopFont();
		skybox.Draw();

		break;
	case GameStateManager::PAUSE:
		lightManager->Draw();
		tileLoader->DrawChunks();
		player->Draw();
		explodingBarrelsFactory->Render();

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
		if (menu.dif1.length() == 0)
		{
			menu.dif1 = "[Easy] " + RandomNames::GenerateRandomName();
			menu.dif2 = "[Medium] " + RandomNames::GenerateRandomName();
			menu.dif3 = "[Hard] " + RandomNames::GenerateRandomName();
			menu.endless = "[Endless] " + RandomNames::GenerateRandomName();
		}

		menu.exitGame = "Exit";
		CreateMenu();
		break;
	case GameStateManager::LOADING:
		loadingShader->Bind();
		loadingShader->SetMat4x4("projection", Game::perspective);
		loadingShader->SetMat4x4("view", Game::view);
		const glm::vec3 camPos = Game::GetCameraPosition();

		loadingShader->SetFloat3("viewPos", 0, 0, 5);

		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 tilePosition = glm::vec3(-8, 5.0f, 0);
		model = glm::translate(model, tilePosition);
		model = glm::rotate(model, glm::radians(10.0f * currentTasks * -2), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(2));
		loadingShader->SetMat4x4("model", model);
		loadingModel->Draw(*loadingShader);

		model = glm::mat4(1.0f);
		tilePosition = glm::vec3(8, 5.0f, 0);
		model = glm::translate(model, tilePosition);
		model = glm::rotate(model, glm::radians(10.0f * currentTasks * 2), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(2));
		loadingShader->SetMat4x4("model", model);

		loadingModel->Draw(*loadingShader);
		loadingShader->Unbind();

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

	ImGui::Begin("info");
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
	ImGui::End();

	world.RenderDebug();
}

void Game::Tick(float deltaTime)
{
	Update(deltaTime);
	//displaying stuff
#ifdef __DEBUG__
	if (gameState.GetState() != GameStateManager::LOADING)
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

void Game::LoadingGame()
{
	std::cout << "Loading game\n";
	currentTasks = 0;
}
