#include "CoinTrigger.h"

#include <iostream>

CoinTrigger::CoinTrigger(ColliderType type, size_t index) : GameObject(type),
                                                            index(index)
{
}

VoidEventUInt& CoinTrigger::GetEvent()
{
	return onCollect;
}

void CoinTrigger::CollidedWith(GameObject* p_obj1)
{
	if (p_obj1->GetType() == Player)
	{
		std::cout << "coinc collected" << std::endl;
		onCollect(index);
	}
}

void CoinTrigger::SeparatedFrom(GameObject* /*p_obj1*/)
{
	//p_obj1;
}

CoinTrigger::~CoinTrigger()
{
}
