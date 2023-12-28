#include "StaticModel.h"


#include "template.h"


#include "stb_image.h"

#include <iostream>
using namespace std;

void StaticModel::Draw(Shader& shader)
{
	for (auto& StaticMesh : meshes)
		StaticMesh.Draw(shader);
}

void StaticModel::SetUpMeshes()
{
	for (auto& StaticMesh : meshes)
	{
		StaticMesh.setupMesh();
	}
}

std::vector<StaticMesh>& StaticModel::GetMeshes()
{
	return meshes;
}

void StaticModel::loadModel(std::string path)
{
#ifdef _WINDOWS
	std::replace(path.begin(), path.end(), '/', '\\'); // Replace \ with 
#endif

	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
#ifdef _WINDOWS
	directory = path.substr(0, path.find_last_of('\\'));

#else


	directory = path.substr(0, path.find_last_of('/'));
#endif

	processNode(scene->mRootNode, scene);
}

//can be used to get a parent child relationship
void StaticModel::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* StaticMesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(StaticMesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

StaticMesh StaticModel::processMesh(aiMesh* StaticMesh, const aiScene* scene)
{
	// data to fill
	vector<StaticMesh::Vertex> vertices;
	vector<unsigned int> indices;
	vector<StaticMesh::MeshTexture> textures;

	// walk through each of the StaticMesh's vertices
	for (unsigned int i = 0; i < StaticMesh->mNumVertices; i++)
	{
		StaticMesh::Vertex vertex;
		glm::vec3 vector;
		// we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = StaticMesh->mVertices[i].x;
		vector.y = StaticMesh->mVertices[i].y;
		vector.z = StaticMesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (StaticMesh->HasNormals())
		{
			vector.x = StaticMesh->mNormals[i].x;
			vector.y = StaticMesh->mNormals[i].y;
			vector.z = StaticMesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// MeshTexture coordinates
		if (StaticMesh->mTextureCoords[0]) // does the StaticMesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = StaticMesh->mTextureCoords[0][i].x;
			vec.y = StaticMesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// now wak through each of the StaticMesh's faces (a face is a StaticMesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < StaticMesh->mNumFaces; i++)
	{
		aiFace face = StaticMesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	if (noTextures)
		return {vertices, indices, textures};
	// process materials
	aiMaterial* material = scene->mMaterials[StaticMesh->mMaterialIndex];


	if (StaticMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[StaticMesh->mMaterialIndex];
		vector<StaticMesh::MeshTexture> diffuseMaps = loadMaterialTextures(material,
		                                                                   aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<StaticMesh::MeshTexture> specularMaps = loadMaterialTextures(material,
		                                                                    aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	// return a StaticMesh object created from the extracted StaticMesh data
	return {vertices, indices, textures};
}

std::vector<StaticMesh::MeshTexture> StaticModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                                                       std::string typeName)
{
	{
		vector<StaticMesh::MeshTexture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			cout << "Texture path: " << str.C_Str() << endl;
		}
	}
	vector<StaticMesh::MeshTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;

		mat->GetTexture(type, i, &str);
#ifdef _WINDOWS
		//for some reason the string has 4 null characters before the path
		str = str.C_Str() + 4;
#endif

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
			StaticMesh::MeshTexture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}

unsigned int StaticModel::TextureFromFile(const char* path, const std::string& directory, bool gamma)
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

		stbi_image_free(data);
	}
	else
	{
		std::cout << "MeshTexture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
