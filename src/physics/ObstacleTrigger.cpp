#include "ObstacleTrigger.h"

ObstacleTrigger::ObstacleTrigger(ColliderType type, size_t index) : GameObject(type), index(index)
{
}

VoidEventUInt& ObstacleTrigger::GetEvent()
{
	return onHit;
}

void ObstacleTrigger::CollidedWith(GameObject* p_obj1)
{
	if (p_obj1->GetType() == Player)
	{
		//deactivate the obstacle
		onHit(index);
	}
}

void ObstacleTrigger::SeparatedFrom(GameObject* p_obj1)
{
}
