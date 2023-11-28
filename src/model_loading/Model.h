#pragma once
//from https://learnopengl.com/Model-Loading/Model
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;

class Model
{
public:
	Model(char* path)
	{
		loadModel(path);
	}

	void Draw(Shader& shader);
	std::vector<Mesh>& GetMeshes();

private:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh::MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	                                                    std::string typeName);
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
	std::vector<Mesh::MeshTexture> textures_loaded;
};
