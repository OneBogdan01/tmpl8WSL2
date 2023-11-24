#pragma once
#include <gl2.h>
#include <LinearMath/btIDebugDraw.h>


class Shader;

class DebugRenderer : public btIDebugDraw
{
public:
	void BindBuffers();
	void UnbindBuffers();
	void InitBuffers();
	DebugRenderer();
	~DebugRenderer() override;
	void SetShaderMatrices();
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
	                      const btVector3& color) override;
	void reportErrorWarning(const char* warningString) override;
	void draw3dText(const btVector3& location, const char* textString) override;
	void setDebugMode(int debugMode) override;
	int getDebugMode() const override;
	bool firstLines = true;

private:
	Shader* simpleShader = nullptr;

	DebugDrawModes mode;

	GLuint VAO;
	GLuint VBO;
	GLfloat vertices[6] = {0};
};
