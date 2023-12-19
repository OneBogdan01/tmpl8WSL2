#pragma once
#include <set>
#include <template.h>
#include "BulletWrapper.h"
#include <vector>

class Shader;

struct DebugInfo
{
	btVector3 from;
	btVector3 color1;
	btVector3 to;
	btVector3 color2;
};
struct Color
{
	float r, g, b;
	bool operator<(const Color& other) const
	{
		if (r != other.r)
			return r < other.r;
		if (g != other.g)
			return g < other.g;
		return b < other.b;
	}
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
	std::vector<DebugInfo> debugInfo;
	set<Color> allColors;
	btVector3 color;
	btVector3 secondColor = btVector3(1.0f, 0.0f, 1.0f);
};
