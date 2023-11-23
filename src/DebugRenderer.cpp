#include "DebugRenderer.h"
#include "template.h"


DebugRenderer::DebugRenderer()
{
	simpleShader = new Shader(
		"shaders/BasicVertexShader.vert",
		"shaders/BasicFragmentShader.frag");
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	simpleShader->Bind();
	// Use modern OpenGL ES 3.1 functions
	GLuint vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

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

	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
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
}

int DebugRenderer::getDebugMode() const
{
}
