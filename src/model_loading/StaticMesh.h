#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <string>
#include <vector>

#include "glm/fwd.hpp"


//from https://learnopengl.com/Model-Loading/StaticMesh


class Shader;

class StaticMesh
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Color;
	};

	struct MeshTexture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;
	StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
	void SetMaterials(Shader& shader);
	void DrawMesh();
	void Draw(Shader& shader);
	void BakeDirectionalLighting();
	void setupMesh();
	unsigned int GetVAO();
private:
	//  render data
	unsigned int VAO, VBO, EBO;
};
