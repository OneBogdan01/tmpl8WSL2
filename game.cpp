#include "game.h"

#include <iostream>

#include "Camera.h"
#include "imgui.h"
#include <glm/glm.hpp>
using namespace glm;
// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------
void Game::Init()
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");

	simpleShader->Bind();


	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
	camera = new Camera();
	camera->Init();
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------


float mixing = .2f;
vec3 cubePositions[] = {
	vec3(0.0f, 0.0f, 0.0f),
	vec3(2.0f, 5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f, 3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f, 2.0f, -2.5f),
	vec3(1.5f, 0.2f, -1.5f),
	vec3(-1.3f, 1.0f, -1.5f)
};
vec3 position;
float fov = 45;
float yOffset = 0;

vec2 rotateCam;
vec2 moveCam;


void Game::Tick(float deltaTime)
{
	world.Update(deltaTime);
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


	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
	simpleShader->SetFloat("mixing", mixing);


	camera->Update(deltaTime);

	mat4 projection = glm::perspective(glm::radians(fov),
	                                   static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
	                                   0.1f, 100.0f);
	simpleShader->SetMat4x4("projection", projection);
	mat4 view = camera->LookAt();
	simpleShader->SetMat4x4("view", view);

	//camera->SetViewMatrix(simpleShader);
	for (unsigned int i = 0; i < 10; i++)
	{
		mat4 model = mat4(1.0f);
		model = translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		vec3 dir(1.0f, 0.3f, 0.5f);
		model = glm::rotate(model, radians(angle), dir);
		simpleShader->SetMat4x4("model", model);
		triangle.Draw();
	}

	simpleShader->Unbind();
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
	rotateCam.x = clamp(rotateCam.x, -1.0f, 1.0f);
	rotateCam.y = clamp(rotateCam.y, -1.0f, 1.0f);
	moveCam.x = clamp(moveCam.x, -1.0f, 1.0f);
	moveCam.y = clamp(moveCam.y, -1.0f, 1.0f);
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
