#include "TileFactory.h"
#include "template.h"
#include <iostream>

#include "animator/Animation.h"
#include "animator/Animation.h"
#include "animator/Animation.h"
#include "animator/Animation.h"

TileFactory::TileFactory()
{
	glGenBuffers(1, &instancedBuffer);
}

void TileFactory::CreateTileModel(const char* path, const glm::vec3 initialPosition)
{
	if (models.find(path) == models.end())
	{
		models[path] = LoadModel(path, initialPosition);
	}
}

StaticModel* TileFactory::GetModel(const char* path)
{
	auto model = models.find(path);
	if (model == models.end())
	{
		std::cout << "Model not found: " << path << std::endl;
	}

	return models[path];
}

void TileFactory::Draw(const char* path, Shader& shader) const
{
	models.at(path)->Draw(shader);
}
void TileFactory::DrawInstanced(const char* path, Shader& shader,std::vector<glm::mat4>& modelMatrices) const
{
	StaticModel* model = models.at(path);
	
	glBindBuffer(GL_ARRAY_BUFFER, instancedBuffer);
	const size_t count = modelMatrices.size();

	glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (auto& mesh : model->GetMeshes())
	{
		mesh.SetMaterials(shader);
		unsigned int VAO = mesh.GetVAO();
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0, static_cast<GLsizei>(count));
		glBindVertexArray(0);
	}
}


StaticModel* TileFactory::LoadModel(const char* path, glm::vec3 /*initialPosition*/)
{
	//initialPosition;
	const auto model = new StaticModel(path);

	for (auto& mesh : model->GetMeshes())
	{
		mesh.BakeDirectionalLighting();
	}

	model->SetUpMeshes();
	return model;
}
