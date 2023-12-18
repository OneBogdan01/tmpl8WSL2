#include "game.h"

#include "tiles/TileLoader.h"
#include "imgui.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
glm::mat4 Game::perspective;
glm::mat4 Game::view;

//glm::vec3 cubePositions[] = {
//	glm::vec3(0.0f, 5.5f, 0.0f),
//	glm::vec3(2.0f, 5.0f, -15.0f),
//	glm::vec3(-1.5f, -2.2f, -2.5f),
//	glm::vec3(-3.8f, -2.0f, -12.3f),
//	glm::vec3(2.4f, -0.4f, -3.5f),
//	glm::vec3(-1.7f, 3.0f, -7.5f),
//	glm::vec3(1.3f, -2.0f, -2.5f),
//	glm::vec3(1.5f, 2.0f, -2.5f),
//	glm::vec3(1.5f, 0.2f, -1.5f),
//	glm::vec3(-1.3f, 1.0f, -1.5f)
//};
glm::vec3 modelPos = glm::vec3(-5.0f, 0.0f, 0.0f);
float scale = 5.0f;


void Game::Init()
{
	world.Init();
	tileLoader = new TileLoader();
	//from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/37494654#37494654
	tileLoader->Init();


	/*simpleShader = new Shader(
	    "assets/shaders/BasicVertexShader.vert",
	    "assets/shaders/BasicFragmentShader.frag");*/
	lightShader = new Shader(
		"assets/shaders/BasicVertexShader.vert",
		"assets/shaders/Sun.frag");

	modelShader = new Shader(
		"assets/shaders/ModelLoading.vert",
		"assets/shaders/ModelLoading.frag");
	modelShader->Bind();
	modelShader->SetFloat3("objectColor", 1.0f, 0.5f, 0.31f);
	modelShader->SetFloat3("lightColor", 1.0f, 1.0f, 1.0f);
	modelShader->Unbind();

	sun.Init();
	camera = new Camera();
	camera->Init();
	player = new PlayerCharacter(world.GetDynamicWorld(), startingPlayerPosition);

	skybox.Init();
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
Timer timer;

void Game::Tick(float deltaTime)
{
	inputManager.Update();
	//input is done first in the template
	//update physics

	world.Update(deltaTime);

	//displaying stuff
#ifdef __DEBUG__
	frameCount++;
	if (timer.elapsed() >= 1.0f)
	{
		FPS = static_cast<float>(frameCount) / timer.elapsed();
		frameCount = 0;
		timer.reset();
	}


	ImGui::Text("FPS:%f", FPS);
	ImGui::Checkbox("Free Camera", &freeCam);
	ImGui::Text("Camera position: %f, %f, %f", camera->GetPosition().x, camera->GetPosition().y,
	            camera->GetPosition().z);

	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Button"))
		f += deltaTime;
#endif

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

	//simpleShader->Bind();


	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	//simpleShader->SetFloat("mixing", mixing);


	camera->Update(deltaTime);

	perspective = glm::perspective(glm::radians(fov),
	                               static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
	                               0.1f, 300.0f);
	//simpleShader->SetMat4x4("projection", perspective);
	view = camera->GetViewMat();

	//simpleShader->SetMat4x4("view", view);


	//for (uint i = 1; i <= 10; i++)
	//{
	//	glm::mat4 matModel = glm::mat4(1.0f);
	//	btVector3 btVec = world.GetRigidBodyPosition(i);
	//	glm::vec3 pos = glm::vec3(btVec.x(), btVec.y(), btVec.z());
	//	matModel = glm::translate(matModel, pos);
	//	/*float angle = 20.0f * i;
	//	vec3 dir(1.0f, 0.3f, 0.5f);
	//	matModel = glm::rotate(matModel, radians(angle), dir);*/
	//	simpleShader->SetMat4x4("model", matModel);
	//	triangle.Draw();
	//}


	tileLoader->Update(deltaTime);

	player->Update(deltaTime);


	tileLoader->DrawChunks();
	player->Draw();

	//skybox
	//simpleShader->Unbind();
	//draw lighting
	lightShader->Bind();
	lightShader->SetMat4x4("projection", perspective);
	lightShader->SetMat4x4("view", view);

	glm::mat4 sunModel = glm::mat4(1.0f);
	sunModel = glm::translate(sunModel, GetLightPos());
	sunModel = glm::scale(sunModel, glm::vec3(0.2f));
	lightShader->SetMat4x4("model", sunModel);
	sun.Draw();
	lightShader->Unbind();
	skybox.Draw();
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
