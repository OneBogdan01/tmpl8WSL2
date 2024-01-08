#pragma once
#include "template.h"
#include "physics/DebugRenderer.h"

class Line
{
public:
	Line();
	~Line();

	void BindBuffers() const;

	void UnbindBuffers();
	void DrawLine();
	void StoreLine(const btVector3& from, const btVector3& to, const btVector3& _color);

private:
	Shader* simpleShader = nullptr;
	GLuint VAO, VBO;
	std::vector<LineInfo> lines;
};
