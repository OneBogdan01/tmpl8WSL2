#pragma once
#include "GameObject.h"

#include "EventArgs.h"

class RopeTrigger : public GameObject
{
public:
	explicit RopeTrigger(ColliderType type, size_t index);
	VoidEventUInt& GetEvent();
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;

private:
	size_t index = 0;
	VoidEventUInt onCollect;
};
