#include "Line.h"

#include "game.h"

Line::Line()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	simpleShader = new Shader("assets/shaders/DebugVert.vert", "assets/shaders/DebugFrag.frag");
}

Line::~Line()
{
	delete simpleShader;
	glDeleteVertexArrays(1, &VBO);
	glDeleteBuffers(1, &VAO);
}

void Line::BindBuffers() const
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void Line::UnbindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::DrawLine()
{
	BindBuffers();
	simpleShader->Bind();
	simpleShader->SetMat4x4("projection", Game::perspective);
	simpleShader->SetMat4x4("view", Game::view);
	glm::mat4 model = glm::mat4(1.0f);
	simpleShader->SetMat4x4("model", model);

	glBufferData(GL_ARRAY_BUFFER, 2 * lines.size() * sizeof(lines), &lines[0],
		GL_STATIC_DRAW);
	//vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(btVector3), (void*)sizeof(btVector3));
	glEnableVertexAttribArray(1);


	glDrawArrays(GL_LINES, 0, static_cast<unsigned int>(lines.size() * 2));


	simpleShader->Unbind();
	lines.clear();
	UnbindBuffers();
}

void Line::StoreLine(const btVector3& from, const btVector3& to, const btVector3& _color)
{
	//store for later render in one go
	LineInfo info;
	info.from = from;
	info.to = to;
	info.color1 = _color;
	info.color2 = _color;


	lines.emplace_back(info);
}
