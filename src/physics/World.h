#pragma once
#include "BulletWrapper.h"
#include <set>
#include <vector>

#include "model_loading/StaticMesh.h"
class DebugRenderer;
//modified from Erik Cupark
// QoL
using CollisionPair = std::pair<const btRigidBody*, const btRigidBody*>;
using CollisionPairs = std::set<CollisionPair>;

class World
{
public:
    World();
    ~World();
    void Update(float deltaTime);

    void AddRigidBody(btRigidBody* rb);


    void RemoveRigidBody(btRigidBody* rb);

    static btBoxShape* CreateBoundingBoxModel(const std::vector<StaticMesh>& meshes, float scale = 1.0f);
    static btCapsuleShape* CreateBoundingCapsuleModel(const std::vector<StaticMesh>& meshes, float scale);
    btVector3 GetRigidBodyPosition(unsigned int ID) const;
    btDiscreteDynamicsWorld* GetDynamicWorld() const;
    void AddRigidBody(btRigidBody* rb) const;
    void Init();

private:
    btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
    btAlignedObjectArray<btCollisionShape*> collisionShapes;
    btSequentialImpulseConstraintSolver* solver;
    btDefaultCollisionConfiguration* collisionConfiguration;

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher;

    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    btBroadphaseInterface* overlappingPairCache;


    static void CollisionEvent(const btRigidBody* pBody0, const btRigidBody* pBody1);
    void CheckForCollisionEvents();


    static void SeparationEvent(const btRigidBody* pBody0, const btRigidBody* pBody1);
    DebugRenderer* debugDrawer = nullptr;
    inline static unsigned int ID = 0;
    // Collision tracking.
    CollisionPairs pairsLastUpdate_;

    // Object tracking.
    std::vector<btRigidBody*> rigidBodies_;
};
