#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <string>
#include <vector>


//from https://learnopengl.com/Model-Loading/Mesh


class Shader;

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
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
	std::vector<MeshTexture> textures_loaded;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
	void Draw(Shader& shader);

private:
	//  render data
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};
