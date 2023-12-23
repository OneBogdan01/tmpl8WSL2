#include "StaticMesh.h"

#include "Camera.h"
#include "Camera.h"
#include "game.h"
#include "template.h"
#include "physics/DebugRenderer.h"


StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned> indices,
                       std::vector<StaticMesh::MeshTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	//setupMesh();
}

void StaticMesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
		// retrieve texture number (the N in diffuse_textureN)
		string number;
		string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader.SetInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	// draw StaticMesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

//we only bake directional lighting
void StaticMesh::BakeDirectionalLighting()
{
	//get data from texture
	if (textures.size() > 0)
	{
		string path = "assets/tiled/castle/" + textures[0].path;
		Surface surface(path.c_str());

		for (auto& vertex : vertices)
		{
			// ambient
			glm::vec3 ambient = glm::vec3(0.1f);

			// diffuse
			glm::vec3 norm = glm::normalize(vertex.Normal);
			glm::vec3 lightDir = glm::normalize(-Game::lightManager->GetSunDir());
			float diff = max(glm::dot(norm, lightDir), 0.0f);
			//a pink color
			glm::vec3 diffuse = glm::vec3(1.0f, 0.71f, 0.75f) * diff;

			vertex.Color = ambient + diffuse;
		}
	}
}

void StaticMesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
	             &indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// vertex color 
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

	glBindVertexArray(0);
}
