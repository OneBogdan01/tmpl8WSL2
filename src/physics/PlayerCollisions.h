#pragma once
#include "EventArgs.h"
#include "GameObject.h"

class PlayerCollisions final : public GameObject
{
public:
	explicit PlayerCollisions(ColliderType type,VoidEvent* ev);
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
	~PlayerCollisions() override;
	bool onGround = false;
	VoidEventVec3* onRope = nullptr;


private:
	VoidEvent* onHit=nullptr;
};
