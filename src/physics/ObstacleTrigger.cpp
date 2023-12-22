#include "ObstacleTrigger.h"

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
