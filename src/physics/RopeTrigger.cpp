#include "RopeTrigger.h"


RopeTrigger::RopeTrigger(ColliderType type, size_t index) : GameObject(type),
                                                            index(index)
{
}

VoidEvent& RopeTrigger::GetEvent()
{
	return onCollect;
}

VoidEventUInt& RopeTrigger::GetEventDisable()
{
	return onDisable;
}	

void RopeTrigger::CollidedWith(GameObject* p_obj1)
{
	if (p_obj1->GetType() == Player)
	{
		std::cout << "Rope Triggered" << std::endl;
		onCollect();
	}
}

void RopeTrigger::SeparatedFrom(GameObject* p_obj1)
{
	p_obj1;
	/*if (p_obj1->GetType() == Player)
	{
		std::cout << "Rope Disabled" << std::endl;
		onDisable(index);
	}*/
}
