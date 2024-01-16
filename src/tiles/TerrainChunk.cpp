#include "TerrainChunk.h"
#include <vector>

using namespace std;
TerrainChunk::TerrainChunk() : heightMap{}, VBO(0), VAO(0)
{
	
}

void TerrainChunk::Init()
{

	DiamondSquare::generateHeightMap(settings, heightMap);
	PrintHeightMap(heightMap);

	// Calculate normals and colors
	vector<glm::vec3> normals;
	vector<glm::vec3> colors;

	for (int i = 0; i < MAX_TERRAIN_SIZE; ++i)
	{
		for (int j = 0; j < MAX_TERRAIN_SIZE; ++j)
		{
			// Calculate normals (simple example, you might want to improve this)
			glm::vec3 normal = glm::normalize(glm::vec3(
				heightMap[i < MAX_TERRAIN_SIZE - 1 ? i + 1 : i][j] - heightMap[i > 0 ? i - 1 : i][j],
				2.0f,
				heightMap[i][j < MAX_TERRAIN_SIZE - 1 ? j + 1 : j] - heightMap[i][j > 0 ? j - 1 : j]
			));

			normals.push_back(normal);

			// Calculate color based on height
			glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); // Default color (e.g., blue)
			// You can adjust color based on height, e.g., interpolate between different colors
			color.y = heightMap[i][j] / maxH; // Adjust based on your actual max height
			colors.push_back(color);
		}
	}

	vector<float> modifiedVertices;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			modifiedVertices.push_back(static_cast<float>(i));
			modifiedVertices.push_back(heightMap[i][j]);
			modifiedVertices.push_back(static_cast<float>(j));

			// Add normals
			modifiedVertices.push_back(normals[i * size + j].x);
			modifiedVertices.push_back(normals[i * size + j].y);
			modifiedVertices.push_back(normals[i * size + j].z);

			// Add colors
			modifiedVertices.push_back(colors[i * size + j].r);
			modifiedVertices.push_back(colors[i * size + j].g);
			modifiedVertices.push_back(colors[i * size + j].b);
		}
	}
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind the Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the modified vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, modifiedVertices.size() * sizeof(float), modifiedVertices.data(), GL_STATIC_DRAW);

	// Set up vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up normal attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set up color attributes
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TerrainChunk::Draw()
{
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, size*size); 
	glBindVertexArray(0);
	

}

void TerrainChunk::PrintHeightMap(const float heightMap[129][129]) const
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			cout << heightMap[i][j] << " ";
		}
		cout << endl;
	}
}
