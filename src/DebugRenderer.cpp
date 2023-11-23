#include "DebugRenderer.h"

#include "game.h"
#include "template.h"
#include "glm/gtx/transform.hpp"


DebugRenderer::DebugRenderer(): mode(DBG_DrawWireframe)
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/SolidColor.frag");
}

DebugRenderer::~DebugRenderer()
{
	delete simpleShader;
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	simpleShader->Bind();

	simpleShader->SetMat4x4("projection", Game::perspective);
	simpleShader->SetMat4x4("view", Game::view);


	glm::mat4 model = glm::mat4(1.0f);

	simpleShader->SetMat4x4("model", model);


	GLuint VAO;
	glGenBuffers(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VAO);

	GLuint VBO;
	glGenVertexArrays(1, &VBO);
	glBindVertexArray(VBO);

	GLfloat vertices[] = {
		from.x(), from.y(), from.z(),
		to.x(), to.y(), to.z()
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Assuming you have a shader program with a "color" attribute
	simpleShader->SetFloat3("color", color.x(), color.y(), color.z());


	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glDrawArrays(GL_LINES, 0, 2);

	glDeleteVertexArrays(1, &VBO);
	glDeleteBuffers(1, &VAO);

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
