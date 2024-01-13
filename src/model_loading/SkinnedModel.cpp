#include "SkinnedModel.h"


#include "template.h"


#include "stb_image.h"

#include <iostream>
#include "UtilityAssimp.h"
using namespace std;

void SkinnedModel::Draw(Shader& shader)
{
	for (auto& SkinnedMesh : meshes)
		SkinnedMesh.Draw(shader);
}

void SkinnedModel::SetUpMeshes()
{
	for (auto& SkinnedMesh : meshes)
	{
		SkinnedMesh.setupMesh();
	}
}
std::vector<SkinnedMesh>& SkinnedModel::GetMeshes()
{
	return meshes;
}

void SkinnedModel::SetVertexBoneData(SkinnedMesh::Vertex& vertex, int boneID, float weight)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}

}

void SkinnedModel::ExtractBoneWeightForVertices(std::vector<SkinnedMesh::Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
			cout << boneName << endl;
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_BoneCounter;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			m_BoneInfoMap[boneName] = newBoneInfo;
			boneID = m_BoneCounter;
			m_BoneCounter++;
		}
		else {
			boneID = m_BoneInfoMap[boneName].id;
		}
		assert(boneID != -1);

		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}

}

void SkinnedModel::loadModel(string path)
{

	//from Sven
	Assimp::Importer import;
	unsigned int assimp_read_flag = aiProcess_Triangulate | aiProcess_FlipUVs;
import.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const aiScene * scene = import.ReadFile(path, assimp_read_flag);
	//Assimp::Importer import;
	////this line craches horribly
	//const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}



	directory = path.substr(0, path.find_last_of('/'));


	processNode(scene->mRootNode, scene);
}

//can be used to get a parent child relationship
void SkinnedModel::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* SkinnedMesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(SkinnedMesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
void SkinnedModel::SetVertexBoneDataToDefault(SkinnedMesh::Vertex& vertex)
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
	{
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

SkinnedMesh SkinnedModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// data to fill
	vector<SkinnedMesh::Vertex> vertices;
	vector<GLuint> indices;
	vector<SkinnedMesh::MeshTexture> textures;

	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		SkinnedMesh::Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (noTextures)
		return { vertices, indices, textures };
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


	if (mesh->mMaterialIndex >= 0)
	{
		material = scene->mMaterials[mesh->mMaterialIndex];
		vector<SkinnedMesh::MeshTexture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<SkinnedMesh::MeshTexture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	// return a mesh object created from the extracted mesh data

	ExtractBoneWeightForVertices(vertices, mesh, scene);
	return { vertices, indices, textures };
}

std::vector<SkinnedMesh::MeshTexture> SkinnedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
	std::string typeName)
{
	{
		vector<SkinnedMesh::MeshTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			cout << "Texture path: " << str.C_Str() << endl;
		}
	}
	vector<SkinnedMesh::MeshTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);


		bool skip = false;

		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			SkinnedMesh::MeshTexture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int SkinnedModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


#ifdef _WINDOWS 
#else// exclude the first 4 mip maps

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 4);

#endif

		stbi_image_free(data);
	}
	else
	{
		std::cout << "MeshTexture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
