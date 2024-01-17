#include "TerrainChunk.h"
#include <vector>

using namespace std;
TerrainChunk::TerrainChunk() : heightMap{}, VBO(0), VAO(0), EBO(0)
{

}

void TerrainChunk::Init()
{
	float maxHeight = 0;

	if (firstRow[0] < 0)
	{
		DiamondSquare::generateHeightMap(settings, heightMap, maxHeight);
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			firstRow[i] = heightMap[MAX_TERRAIN_SIZE - 1][i];
			lastRow[i] = heightMap[0][i];
		}
	}
	else
	{
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			heightMap[0][i] = firstRow[i];
		}
		DiamondSquare::generateHeightMapNoFirstRow(settings, heightMap, maxHeight);
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			firstRow[i] = heightMap[MAX_TERRAIN_SIZE - 1][i];
		}
	}

	cout << maxHeight << endl;
	PrintHeightMap(heightMap);

	// Calculate normals and colors
	vector<glm::vec3> normals;
	vector<glm::vec3> colors;

	for (int i = 0; i < MAX_TERRAIN_SIZE; ++i)
	{
		for (int j = 0; j < MAX_TERRAIN_SIZE; ++j)
		{


			// Calculate color based on height
			glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); // Default color (e.g., blue)
			// You can adjust color based on height, e.g., interpolate between different colors
			color.y = heightMap[i][j] / (maxHeight); // Adjust based on your actual max height
			colors.push_back(color);
		}
	}
	//from Lynn, I had the wrong direction
	for (int i = 0; i < MAX_TERRAIN_SIZE - 1; ++i)
	{
		for (int j = 0; j < MAX_TERRAIN_SIZE - 1; ++j)
		{
			// First triangle of the square
			indices.push_back(j + i * MAX_TERRAIN_SIZE); // Top left vertej
			indices.push_back(j + 1 + i * MAX_TERRAIN_SIZE); // Top right vertej
			indices.push_back(j + (i + 1) * MAX_TERRAIN_SIZE); // Bottom left vertej

			// Second triangle
			indices.push_back(j + 1 + i * MAX_TERRAIN_SIZE); // Top right vertej
			indices.push_back(j + 1 + (i + 1) * MAX_TERRAIN_SIZE); // Bottom right vertej
			indices.push_back(j + (i + 1) * MAX_TERRAIN_SIZE); // Bottom left vertej



		}
	}

	normals.assign(MAX_TERRAIN_SIZE * MAX_TERRAIN_SIZE, glm::vec3(0.0f, 1.0f, 0.0f));




	vector<float> modifiedVertices;
	cout << endl;
	for (int i = 0; i < MAX_TERRAIN_SIZE; ++i)
	{

		for (int j = 0; j < MAX_TERRAIN_SIZE; ++j)
		{

			modifiedVertices.push_back(static_cast<float>(i));
			modifiedVertices.push_back(heightMap[i][j]);
			modifiedVertices.push_back(static_cast<float>(j));

			// Add normals
			modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].x );
			modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].y );
			modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].z );

			// Add colors
			modifiedVertices.push_back(colors[i * MAX_TERRAIN_SIZE + j].r);
			modifiedVertices.push_back(colors[i * MAX_TERRAIN_SIZE + j].g);
			modifiedVertices.push_back(colors[i * MAX_TERRAIN_SIZE + j].b);
		}
	}
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind the Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copy the modified vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, modifiedVertices.size() * sizeof(float), &modifiedVertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
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

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TerrainChunk::Draw()
{

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


}

void TerrainChunk::PrintHeightMap(const float heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE]) const
{
	for (int i = 0; i < MAX_TERRAIN_SIZE; ++i)
	{
		for (int j = 0; j < MAX_TERRAIN_SIZE; ++j)
		{
			cout << heightMap[i][j] << " ";
		}
		cout << endl;
	}
}

void TerrainChunk::SetLastRow()
{
	for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
	{
		heightMap[MAX_TERRAIN_SIZE - 1][i] = lastRow[i];
	}
}
