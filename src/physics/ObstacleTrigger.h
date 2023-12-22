#pragma once
#include "GameObject.h"
#include "EventArgs.h"

class ObstacleTrigger : public GameObject
{
public:
	explicit ObstacleTrigger(ColliderType type, size_t index)
		: GameObject(type)
	{
	}

	VoidEventUInt& GetEvent();
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;

private:
	size_t index = 0;
	VoidEventUInt onHit;
};
