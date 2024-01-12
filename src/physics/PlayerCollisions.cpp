#include "PlayerCollisions.h"

#include "RopeTrigger.h"


PlayerCollisions::PlayerCollisions(ColliderType type, VoidEvent* ev) :
	GameObject(type)
{
	onHit = ev;
	onRope = new VoidEventVec3();
}


void PlayerCollisions::CollidedWith(GameObject* p_obj1)
{
	//cout << "This is a " << type << endl;
	if (p_obj1->GetType() == Ground)
	{
		onGround = true;
		//std::cout << "Colliding with ground" << std::endl;
	}
	else if (p_obj1->GetType() == Coin)
	{
		//std::cout << "Colliding with coin" << std::endl;
	}
	else if (p_obj1->GetType() == Obstacle)
	{
		(*onHit)();
		//std::cout << "Colliding with obstacle" << std::endl;
	}
	else if (p_obj1->GetType() == Rope)
	{
		auto* rope = dynamic_cast<RopeTrigger*>(p_obj1);
		if (rope->ropeEnd != nullptr)
		{
			(*onRope)(rope->ropeEnd);
		}
		//std::cout << "Colliding with obstacle" << std::endl;
	}
}

void PlayerCollisions::SeparatedFrom(GameObject* p_obj1)
{
}

PlayerCollisions::~PlayerCollisions()
{
}
