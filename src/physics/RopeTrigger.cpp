#include "RopeTrigger.h"


RopeTrigger::RopeTrigger(ColliderType type, size_t index) : GameObject(type),
                                                            index(index)
{
}

VoidEventUInt& RopeTrigger::GetEvent()
{
	return onCollect;
}

void RopeTrigger::CollidedWith(GameObject* p_obj1)
{
	if (p_obj1->GetType() == Player)
	{
		std::cout << "Rope Triggered" << std::endl;
		onCollect(index);
	}
}

void RopeTrigger::SeparatedFrom(GameObject* p_obj1)
{
}
