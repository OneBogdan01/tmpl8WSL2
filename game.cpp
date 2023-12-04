#include "game.h"
#include <iostream>
#include "Camera.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"
#include "model_loading/Model.h"
#include <filesystem>
#include "Timer.h"
#include "tiles/TileLoader.h"

// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
glm::mat4 Game::perspective;
glm::mat4 Game::view;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 5.5f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};
glm::vec3 modelPos = glm::vec3(-5.0f, 0.0f, 0.0f);
float scale = 5.0f;


void Game::Init()
{
	tileLoader = new TileLoader();
	//from https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c/37494654#37494654
	tileLoader->Init("assets/tiled/map.tmx");

	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");
	lightShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/Sun.frag");

	modelShader = new Shader(
		"shaders/ModelLoading.vert",
		"shaders/ModelLoading.frag");
	modelShader->Bind();
	modelShader->SetFloat3("objectColor", 1.0f, 0.5f, 0.31f);
	modelShader->SetFloat3("lightColor", 1.0f, 1.0f, 1.0f);
	modelShader->Unbind();
	simpleShader->Bind();


	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
	sun.Init();
	camera = new Camera();
	camera->Init();

	for (int i = 0; i <= 9; i++)
	{
		btVector3 pos = btVector3(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
		world.AddARigidbody(pos);
	}


	//model = new Model("assets/backpack/backpack.ob");
	model = new Model("assets/tiled/castle/tower.ob");

	world.AddAModelRigidbody(btVector3(modelPos.x, modelPos.y, modelPos.z), model->GetMeshes(), scale);
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
const int startFrame = 0;
const int endFrame = 197;
int renderFrame = startFrame;
// Rendering loop
float interpolation = 0.0f;
int bufferIndex = 0;

void Game::Tick(float deltaTime)
{
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

	simpleShader->Bind();


	simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);


	camera->Update(deltaTime);

	perspective = glm::perspective(glm::radians(fov),
	                               static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
	                               0.1f, 100.0f);
	simpleShader->SetMat4x4("projection", perspective);
	view = camera->LookAt();

	simpleShader->SetMat4x4("view", view);


	for (uint i = 1; i <= 10; i++)
	{
		glm::mat4 matModel = glm::mat4(1.0f);
		btVector3 btVec = world.GetRigidBodyPosition(i);
		glm::vec3 pos = glm::vec3(btVec.x(), btVec.y(), btVec.z());
		matModel = glm::translate(matModel, pos);
		/*float angle = 20.0f * i;
		vec3 dir(1.0f, 0.3f, 0.5f);
		matModel = glm::rotate(matModel, radians(angle), dir);*/
		simpleShader->SetMat4x4("model", matModel);
		triangle.Draw();
	}

	simpleShader->Unbind();
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

	modelShader->Bind();
	modelShader->SetMat4x4("projection", perspective);
	modelShader->SetMat4x4("view", view);
	glm::mat4 matModel = glm::mat4(1.0f);
	btVector3 btVec = world.GetRigidBodyPosition(11);
	glm::vec3 pos = glm::vec3(btVec.x(), btVec.y(), btVec.z());
	matModel = glm::translate(matModel, pos);
	modelShader->SetMat4x4("model", matModel);
	glm::vec3 camPos = camera->GetPosition();
	modelShader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
	modelShader->SetFloat3("lightPos", GetLightPos().x, GetLightPos().y, GetLightPos().z);
	modelShader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	modelShader->SetFloat("material.shininess", 32.0f);
	modelShader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	modelShader->SetFloat3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	modelShader->SetFloat3("light.specular", 1.0f, 1.0f, 1.0f);

	modelShader->SetInt("material.diffuse", 0);
	model->Draw(*modelShader);
	//draw the tile map
	tileLoader->DrawChunk(0);

	modelShader->Unbind();
	//render player animating

	player.Draw(renderFrame, 0, interpolation, view, perspective);
	if (interpolation >= 1.0f)
	{
		interpolation = 0.0f;
		if (renderFrame == endFrame)
		{
			renderFrame = startFrame;
			bufferIndex = 0;
		}
		else
		{
			renderFrame++;
			bufferIndex++;
		}
	}
	interpolation += 10.0f * deltaTime;


	//skybox
	skybox.Draw();
}

void Game::Shutdown()
{
	delete simpleShader;
	delete tileLoader;
	delete camera;
}

void Game::KeyDown(XID key)
{
	switch (key)
	{
	case XK_w:
		yOffset -= 1;
		break;
	case XK_s:
		yOffset += 1;
		break;
	case XK_d:
		rotateCam.x += -1;
		break;
	case XK_a:
		rotateCam.x -= -1;
		break;
	case XK_z:
		rotateCam.y += 1;
		break;
	case XK_c:
		rotateCam.y -= 1;
		break;
	case XK_Left:
		moveCam.x = -1;


		break;
	case XK_Right:
		moveCam.x = 1;


		break;
	case XK_Down:
		moveCam.y = -1;


		break;
	case XK_Up:
		moveCam.y = 1;


		break;
	default:
		break;
	}
}

void Game::KeyUp(XID key)
{
	switch (key)
	{
	case XK_w:
		yOffset -= 1;

		break;
	case XK_s:
		yOffset += 1;
		break;
	case XK_d:
		rotateCam.x += 1;
		break;
	case XK_a:
		rotateCam.x -= 1;
		break;
	case XK_z:
		rotateCam.y += -1;
		break;
	case XK_c:
		rotateCam.y -= -1;
		break;
	case XK_Left:
		moveCam.x -= -1;


		break;
	case XK_Right:
		moveCam.x -= 1;


		break;
	case XK_Down:
		moveCam.y -= -1;


		break;
	case XK_Up:
		moveCam.y -= 1;


		break;
	default:
		break;
	}
	rotateCam.x = glm::clamp(rotateCam.x, -1.0f, 1.0f);
	rotateCam.y = glm::clamp(rotateCam.y, -1.0f, 1.0f);
	moveCam.x = glm::clamp(moveCam.x, -1.0f, 1.0f);
	moveCam.y = glm::clamp(moveCam.y, -1.0f, 1.0f);
}

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
