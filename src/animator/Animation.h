

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <assimp/scene.h>



#include <model_loading/SkinnedModel.h>
#include "Bone.h"



class Animation
{
public:
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};
	Animation() = default;

	Animation(const char* animationPath, SkinnedModel* model);

	~Animation()
	= default;

	Bone* FindBone(const std::string& name);


	float GetTicksPerSecond() const { return m_TicksPerSecond; }
	float GetDuration() const { return m_Duration; }
	const AssimpNodeData& GetRootNode() { return m_RootNode; }

	const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap()
	{
		return m_BoneInfoMap;
	}

private:
	void ReadMissingBones(const aiAnimation* animation, SkinnedModel& model);

	void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
	float m_Duration;
	int m_TicksPerSecond;
	std::vector<Bone> m_Bones;
	AssimpNodeData m_RootNode;
	std::unordered_map<std::string, BoneInfo> m_BoneInfoMap;
};

