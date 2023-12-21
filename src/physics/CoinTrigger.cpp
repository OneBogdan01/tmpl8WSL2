#include "CoinTrigger.h"

#include <iostream>

CoinTrigger::CoinTrigger(ColliderType type): GameObject(type)
{
}

void CoinTrigger::CollidedWith(GameObject* p_obj1)
{
	if (p_obj1->GetType() == Player)
	{
		std::cout << "Colliding with player" << std::endl;
		//delete this;
		//		//this->~CoinTrigger();
		//				//this->~GameObject();
		//						//delete this;
		//								//delete this;
		//										//delete this;
		//												//delete}
	}
}

void CoinTrigger::SeparatedFrom(GameObject* p_obj1)
{
}

CoinTrigger::~CoinTrigger()
{
}
