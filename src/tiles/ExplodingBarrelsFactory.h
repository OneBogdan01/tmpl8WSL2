#pragma once
#include <vector>

#include "ExplodingBarrel.h"
#include "model_loading/StaticMesh.h"


class ExplodingBarrelsFactory
{
public:
	ExplodingBarrelsFactory();
	void SetMaterialProperties();
	~ExplodingBarrelsFactory() = default;
	static void CreateExplosion(glm::vec3& position);
	void Render();
	void Update(float deltaTime);

private:
	static inline std::array<ExplodingBarrel*, 7> barrels;
	Shader* modelShader=nullptr;
};
