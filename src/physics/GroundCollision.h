#pragma once
#include "physics/GameObject.h"

class GroundCollision : public GameObject
{
public:
	explicit GroundCollision(ColliderType type)
		: GameObject(type)
	{
	}

	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
};
