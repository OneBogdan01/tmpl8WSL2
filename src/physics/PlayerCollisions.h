#pragma once
#include "GameObject.h"

class PlayerCollisions final : public GameObject
{
public:
	explicit PlayerCollisions(ColliderType type);
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
	~PlayerCollisions() override;
	bool onGround = false;
};
