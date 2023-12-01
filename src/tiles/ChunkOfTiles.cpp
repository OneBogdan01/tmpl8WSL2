#include "ChunkOfTiles.h"


#include "game.h"
#include "TileLoader.h"
#include "glm/fwd.hpp"
#include "glm/gtx/transform.hpp"

ChunkOfTiles::~ChunkOfTiles()
{
	delete modelShader;
}

ChunkOfTiles::ChunkOfTiles()
{
	modelShader = new Shader(
		"shaders/ModelLoading.vert",
		"shaders/ModelLoading.frag");
}

void ChunkOfTiles::LoadTile(size_t index, const char* path, glm::vec3 pos)
{
	tiles[index].LoadModel(path);
	tiles[index].SetPosition(pos);
}

void ChunkOfTiles::Draw()
{
	//m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));

	/*modelShader->Bind();
	modelShader->SetMat4x4("projection", Game::perspective);
	modelShader->SetMat4x4("view", Game::view);*/
	for (auto& tile : tiles)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 position = tile.GetPosition();
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(TILE_SIZE));
		modelShader->SetMat4x4("model", model);
		tile.Draw(*modelShader);
	}

	//modelShader->Unbind();
}
