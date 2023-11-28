#include "game.h"

#include <iostream>

#include "Camera.h"
#include "imgui.h"
#include <glm/glm.hpp>

#include "stb_image.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtx/transform.hpp"

#include "model_loading/Model.h"


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

void Game::Init()
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");
	modelShader = new Shader(
		"shaders/ModelLoading.vert",
		"shaders/ModelLoading.frag");

	simpleShader->Bind();


	simpleShader->SetInt("wallTexture", 0); // or with shader class
	simpleShader->SetInt("faceTexture", 1); // or with shader class

	simpleShader->Unbind();


	triangle.Init();
	camera = new Camera();
	camera->Init();
	for (uint i = 1; i <= 10; i++)
	{
		btVector3 pos = btVector3(cubePositions[i].x, cubePositions[i].y, cubePositions[i].z);
		world.AddARigidbody(pos);
	}
	stbi_set_flip_vertically_on_load(true);
	model = new Model("assets/backpack/backpack.ob");
}

// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------


float mixing = .2f;

glm::vec3 position;
float fov = 45;
float yOffset = 0;

glm::vec2 rotateCam;
glm::vec2 moveCam;

float f = 0.3f;
char buf[] = "some windows";

void Game::Tick(float deltaTime)
{
	ImGui::Text("Hello, world %d", 123);

	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	if (ImGui::Button("Button"))
		f += deltaTime;

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


	//simpleShader->SetFloat3("offset", position.x, position.y, position.z);
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
		glm::mat4 model = glm::mat4(1.0f);
		btVector3 btVec = world.GetRigidBodyPosition(i);
		glm::vec3 pos = glm::vec3(btVec.x(), btVec.y(), btVec.z());
		model = glm::translate(model, pos);
		/*float angle = 20.0f * i;
		vec3 dir(1.0f, 0.3f, 0.5f);
		model = glm::rotate(model, radians(angle), dir);*/
		simpleShader->SetMat4x4("model", model);
		triangle.Draw();
	}

	simpleShader->Unbind();
	modelShader->Bind();
	glm::mat4 m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, glm::vec3(3.0f, 0.0f, -5.0f));
	m_model = glm::scale(m_model, glm::vec3(1.0f, 1.0f, 1.0f));
	modelShader->SetMat4x4("model", m_model);
	modelShader->SetMat4x4("projection", perspective);
	modelShader->SetMat4x4("view", view);


	model->Draw(*modelShader);
	modelShader->Unbind();
	world.Update(deltaTime);
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
