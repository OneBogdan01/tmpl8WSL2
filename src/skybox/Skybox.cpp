#include "Skybox.h"

#include <iostream>

#include "game.h"
#include "stb_image.h"

Skybox::~Skybox()
{
	delete skyboxShader;
}

unsigned int Skybox::LoadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void Skybox::Draw() const
{
#ifdef _WINDOWS
	glDepthFunc(GL_LEQUAL);
	skyboxShader->Bind();
	skyboxShader->SetMat4x4("projection", Game::perspective);
	glm::mat4 view = glm::mat4(glm::mat3(Game::view));

	skyboxShader->SetMat4x4("view", view);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glDepthFunc(GL_LESS); // set depth function back to default

#else

	glDepthFunc(GL_LEQUAL);
	skyboxShader->Bind();
	skyboxShader->SetMat4x4("projection", Game::perspective);
	glm::mat4 view = glm::mat4(glm::mat3(Game::view));

	skyboxShader->SetMat4x4("view", view);
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
#endif
}

void Skybox::Init()
{
	skyboxShader = new Shader("assets/shaders/Skybox.vert", "assets/shaders/Skybox.frag");
	skyboxShader->Bind();
	skyboxShader->SetInt("skybox", 0);
	skyboxShader->Unbind();


	vector<std::string> faces
	{
		"assets/skybox/px.jpg",
		"assets/skybox/nx.jpg",
		"assets/skybox/py.jpg",
		"assets/skybox/ny.jpg",
		"assets/skybox/pz.jpg",
		"assets/skybox/nz.jpg"
	};
	cubemapTexture = LoadCubemap(faces);


	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Vertex buffer object creation
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
