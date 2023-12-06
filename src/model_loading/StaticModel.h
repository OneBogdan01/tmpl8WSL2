#pragma once
//from https://learnopengl.com/Model-Loading/Model
#include "StaticMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;

class StaticModel
{
public:
	StaticModel(const char* path)
	{
		loadModel(path);
	}

	void Draw(Shader& shader);
	std::vector<StaticMesh>& GetMeshes();

private:
	// model data
	std::vector<StaticMesh> meshes;
	std::string directory;
	std::vector<StaticMesh::MeshTexture> textures_loaded;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	StaticMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<StaticMesh::MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	                                                          std::string typeName);
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};
