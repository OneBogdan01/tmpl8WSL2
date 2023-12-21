#pragma once
#include "GameObject.h"

class CoinTrigger : public GameObject
{
public:
	explicit CoinTrigger(ColliderType type);

	void CollidedWith(GameObject* p_obj1) override;
	void SeparatedFrom(GameObject* p_obj1) override;
	~CoinTrigger() override;
};
