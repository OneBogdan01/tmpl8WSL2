#pragma once
#include <set>
#include <template.h>
#include "BulletWrapper.h"
#include <vector>

class Shader;

struct LineInfo
{
	btVector3 from;
	btVector3 color1;
	btVector3 to;
	btVector3 color2;
};


class DebugRenderer : public btIDebugDraw
{
public:
	void BindBuffers();
	void UnbindBuffers();
	void InitBuffers();
	DebugRenderer();
	~DebugRenderer() override;
	void SetShaderMatrices();
	void RenderDebug();
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
	std::vector<LineInfo> debugInfo;

};
