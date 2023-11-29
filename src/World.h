#pragma once
#include <btBulletDynamicsCommon.h>
#include <vector>

#include "model_loading/Mesh.h"
class DebugRenderer;

class World
{
public:
	World();
	~World();
	void Update(float deltaTime);
	uint AddARigidbody(const btVector3& startinPos);
	uint AddAModelRigidbody(const btVector3& startingPos, const std::vector<Mesh>& meshes, float scale = 1.0f);
	btVector3 GetRigidBodyPosition(const uint ID) const;

private:
	btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btSequentialImpulseConstraintSolver* solver;
	btDefaultCollisionConfiguration* collisionConfiguration;

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher;

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache;
	bool debugPositionInitialized = false;
	DebugRenderer* debugDrawer = nullptr;
	inline static uint ID = 0;
};
