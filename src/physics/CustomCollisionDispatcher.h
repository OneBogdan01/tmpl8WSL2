#pragma once
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

class CustomCollisionDispatcher : public btCollisionDispatcher
{
public:
	explicit CustomCollisionDispatcher(btCollisionConfiguration* collisionConfiguration)
		: btCollisionDispatcher(collisionConfiguration)
	{
	}

	bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1) override;
	bool needsResponse(const btCollisionObject* body0, const btCollisionObject* body1) override;
};
