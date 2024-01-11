#include "CustomCollisionDispatcher.h"
//only if the kinematic character controller is involved
bool CustomCollisionDispatcher::needsCollision(const btCollisionObject* body0, const btCollisionObject* body1)
{
	// player
	bool isKinmatic1 = body0->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT;
	bool isKinmatic2 = body1->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT;
	// trigger
	bool isGhost1 = body0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;
	bool isGhost2 = body1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;

	return isKinmatic1 || isKinmatic2 || isGhost1 || isGhost2;
}

bool CustomCollisionDispatcher::needsResponse(const btCollisionObject* body0, const btCollisionObject* body1)
{
	// player
	bool isKinmatic1 = body0->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT;
	bool isKinmatic2 = body1->getCollisionFlags() & btCollisionObject::CF_CHARACTER_OBJECT;
	// trigger
	bool isGhost1 = body0->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;
	bool isGhost2 = body1->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE;

	return isKinmatic1 || isKinmatic2 || isGhost1 || isGhost2;
}
