#pragma once
//from https://learnopengl.com/StaticModel-Loading/StaticModel
#include "SkinnedMesh.h"

#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "BoneInfo.h"

class Shader;

class SkinnedModel
{
public:
	SkinnedModel(const char* path)
	{
		loadModel(path);
	}

	SkinnedModel(const char* path, bool NoTextures)
	{
		noTextures = NoTextures;

		loadModel(path);
	}

	void Draw(Shader& shader);
	void SetUpMeshes();
	std::vector<SkinnedMesh>& GetMeshes();
	auto& GetBoneInfoMap() { return m_BoneInfoMap; }
	int& GetBoneCount() { return m_BoneCounter; }

private:
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;
	bool noTextures = false;

	std::vector<SkinnedMesh> meshes;
	std::string directory;
	std::vector<SkinnedMesh::MeshTexture> textures_loaded;
	void SetVertexBoneData(SkinnedMesh::Vertex& vertex, int boneID, float weight);
	void ExtractBoneWeightForVertices(std::vector<SkinnedMesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	void SetVertexBoneDataToDefault(SkinnedMesh::Vertex& vertex);
	SkinnedMesh processMesh(aiMesh* StaticMesh, const aiScene* scene);
	std::vector<SkinnedMesh::MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
	unsigned TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
};
