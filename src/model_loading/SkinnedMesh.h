#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <string>
#include <vector>

#include "glm/fwd.hpp"


//from https://learnopengl.com/Model-Loading/StaticMesh


class Shader;

class SkinnedMesh
{
public:
#define MAX_BONE_INFLUENCE 10

	struct Vertex {
		// position
		glm::vec3 Position;
		// normal
		glm::vec3 Normal;
		// texCoords
		glm::vec2 TexCoords;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
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
	SkinnedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
	void Draw(Shader& shader);
	void setupMesh();

private:
	//  render data
	unsigned int VAO, VBO, EBO;
};
