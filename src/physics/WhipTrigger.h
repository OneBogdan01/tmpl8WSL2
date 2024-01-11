#pragma once
#include "GameObject.h"

class WhipTrigger : public GameObject
{
public:
	explicit WhipTrigger(ColliderType _type);
	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
};
