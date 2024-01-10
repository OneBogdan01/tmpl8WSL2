#pragma once
#include "GameObject.h"

#include "EventArgs.h"
#include "glm/vec3.hpp"

class RopeTrigger : public GameObject
{
public:
	explicit RopeTrigger(ColliderType type, size_t index);
	VoidEvent& GetEvent();
	VoidEventUInt& GetEventDisable();
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
	glm::vec3* ropeEnd=nullptr;

private:
	size_t index = 0;
	VoidEvent onCollect;
	VoidEventUInt onDisable;
};
