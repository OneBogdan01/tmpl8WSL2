#include "Triangle.h"


void Triangle::Draw()
{
	glActiveTexture(GL_TEXTURE0);

	wall.Bind();

	glActiveTexture(GL_TEXTURE1);

	face.Bind();

	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	face.Unbind();
	wall.Unbind();
}

void Triangle::Init()
{
	//texture loading

	wall.Init("assets/wall.jpg");
	face.Init("assets/awesomeface.png", true);
	//Vertex array object creation
	//first triangle
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Vertex buffer object creation
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	////Element buffer object
	//glGenBuffers(1, &EBO);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//saves to EBO or the current buffer binding
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//after VAO is unbind
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
