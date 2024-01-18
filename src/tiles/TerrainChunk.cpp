#include "TerrainChunk.h"
#include <vector>

#include "animator/Animation.h"


using namespace std;
TerrainChunk::TerrainChunk() : heightMap{}, VBO(0), VAO(0), EBO(0)
{

}

glm::vec3 TerrainChunk::GetColor(float height)
{


	glm::vec3 waterColor = glm::vec3(0.0f, 0.6f, 1.0f);   // Light blue for water
	glm::vec3 shallowWaterColor = glm::vec3(0.0f, 0.8f, 1.0f); // Lighter blue for shallow water
	glm::vec3 sandColor = glm::vec3(0.9f, 0.8f, 0.5f); // Sand color
	glm::vec3 grassColor = glm::vec3(0.0f, 0.4f, 0.3f);   // Green for grass
	glm::vec3 mountainColor = glm::vec3(0.7f, 0.7f, 0.7f); // Grey for mountains
	glm::vec3 snowColor = glm::vec3(1.0f, 1.0f, 1.0f); // White for snow
	if (height > 0.9f)
	{
		return snowColor;
	}
	else if (height > 0.6f)
	{
		return mountainColor;
	}
	else if (height > 0.3f)
	{
		return grassColor;
	}
	else if (height > 0.2f)
	{
		return sandColor;
	}
	else if (height > 0.1f)
	{
		return shallowWaterColor;
	}
	else
	{
		return waterColor;
	}
}




void TerrainChunk::Init()
{
	float maxHeight = 0;
	for (int j = MAX_TERRAIN_SIZE / 2 -3; j < MAX_TERRAIN_SIZE / 2 + 5; j++)
	{
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			heightMap[i][j] = 0.3f;
		}
	}
	if (firstRow[0] < 0)
	{
		DiamondSquare::generateHeightMap(settings, heightMap);
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			firstRow[i] = heightMap[MAX_TERRAIN_SIZE - 1][i];
			//heightMap[0][i] = firstRow[i];

			lastRow[i] = heightMap[0][i];
		}
	}
	else
	{
		for (int i = 0; i < MAX_TERRAIN_SIZE; i++)
		{
			heightMap[0][i] = firstRow[i];
		}
		DiamondSquare::generateHeightMapNoFirstRow(settings, heightMap);
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



			float h = heightMap[i][j]/maxH;
			colors.push_back(GetColor(h));
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

			//// Add normals
			//modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].x );
			//modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].y );
			//modifiedVertices.push_back(normals[i * MAX_TERRAIN_SIZE + j].z );

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// Set up color attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TerrainChunk::Draw()
{
	glEnable(GL_CULL_FACE);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);


}

void TerrainChunk::PrintHeightMap(const float _heightMap[MAX_TERRAIN_SIZE][MAX_TERRAIN_SIZE]) const
{
	for (int i = 0; i < MAX_TERRAIN_SIZE; ++i)
	{
		for (int j = 0; j < MAX_TERRAIN_SIZE; ++j)
		{
			cout << _heightMap[i][j] << " ";
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
