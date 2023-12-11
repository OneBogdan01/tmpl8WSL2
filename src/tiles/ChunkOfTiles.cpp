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
    tiles[index].Init(path, pos);
    activeTiles.push_back(index);
}

void ChunkOfTiles::Draw()
{
    //m_model = glm::scale(m_model, glm::vec3(scale, scale, scale));

    /*modelShader->Bind();
    modelShader->SetMat4x4("projection", Game::perspective);
    modelShader->SetMat4x4("view", Game::view);*/
    for (auto& index : activeTiles)
    {
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 position = tiles[index].GetPosition();
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(TILE_SIZE));
        modelShader->SetMat4x4("model", model);
        tiles[index].Draw(*modelShader);
    }

    //modelShader->Unbind();
}

void ChunkOfTiles::Update(float deltaTime)
{
    for (auto& index : activeTiles)
    {
        tiles[index].SetOffset(offset);
    }
    offset = glm::vec3(0.0f);
}

void ChunkOfTiles::SetPosition(glm::vec3 pos)
{
    offset = pos;
    position += offset;
}

glm::vec3 ChunkOfTiles::GetPosition()
{
    return position;
}
