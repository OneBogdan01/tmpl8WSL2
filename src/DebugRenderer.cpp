#include "DebugRenderer.h"

#include <iostream>

#include "game.h"
#include "template.h"
#include "glm/gtx/transform.hpp"


void DebugRenderer::BindBuffers()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void DebugRenderer::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DebugRenderer::InitBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

DebugRenderer::DebugRenderer() : mode(DBG_DrawWireframe)
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/SolidColor.frag");
	InitBuffers();
}

DebugRenderer::~DebugRenderer()
{
	delete simpleShader;
	glDeleteVertexArrays(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void DebugRenderer::SetShaderMatrices()
{
	simpleShader->SetMat4x4("projection", Game::perspective);
	simpleShader->SetMat4x4("view", Game::view);
	glm::mat4 model = glm::mat4(1.0f);
	simpleShader->SetMat4x4("model", model);
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//assign vertices
	for (int i = 0; i < 3; i++)
	{
		vertices[i] = from[i];
	}
	for (int i = 3; i < 6; i++)
	{
		vertices[i] = to[i % 3];
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	simpleShader->Bind();
	SetShaderMatrices();

	simpleShader->SetFloat3("color", color.x(), color.y(), color.z());

	glDrawArrays(GL_LINES, 0, 2);


	simpleShader->Unbind();
}

void DebugRenderer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance,
                                     int lifeTime, const btVector3& color)
{
}

void DebugRenderer::reportErrorWarning(const char* warningString)
{
}

void DebugRenderer::draw3dText(const btVector3& location, const char* textString)
{
}

void DebugRenderer::setDebugMode(int debugMode)
{
	mode = static_cast<DebugDrawModes>(debugMode);
}

int DebugRenderer::getDebugMode() const
{
	return mode;
}
