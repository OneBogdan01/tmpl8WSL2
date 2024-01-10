/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  https://bulletphysics.org

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

///-----includes_start-----
#include "World.h"

#include <algorithm>
#include <iterator>
#include <stdio.h>

#include "CustomCollisionDispatcher.h"
#include "DebugRenderer.h"
#include "GameObject.h"


/// This is a Hello World program for running a basic Bullet physics simulation
void World::RenderDebug()
{
	dynamicsWorld->debugDrawWorld();
	debugDrawer->RenderDebug();
}

void World::Update(float deltaTime)
{
	//RenderDebug();


	dynamicsWorld->stepSimulation(1.0f / 60.0f, 5, 1.0f / 60.0f); //set it to 5! for the PI
	////print positions of all objects
	//for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	//{
	//	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
	//	btRigidBody* body = btRigidBody::upcast(obj);
	//	btTransform trans;
	//	if (body && body->getMotionState())
	//	{
	//		body->getMotionState()->getWorldTransform(trans);
	//	}
	//	else
	//	{
	//		trans = obj->getWorldTransform();
	//	}
	//	printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()),
	//	       float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	//}
	CheckForCollisionEvents();
}

void World::AddRigidBody(btRigidBody* rb)
{
	if (rb)
	{
		rigidBodies_.push_back(rb);
		dynamicsWorld->addRigidBody(rb);
	}
}

void World::RemoveRigidBody(btRigidBody* rb)
{
	// Make sure passed value is not null and that it exists in the vector.
	if (rb)
	{
		auto iter{ std::find(rigidBodies_.begin(), rigidBodies_.end(), rb) };

		if (iter != rigidBodies_.end())
		{
			// Remove the rigidbody from vector and world.
			rigidBodies_.erase(iter);
			dynamicsWorld->removeRigidBody(rb);
		}
	}
}

///-----stepsimulation_end-----

//cleanup in the reverse order of creation/initialization


//uint World::AddARigidbody(const btVector3& startinPos)
//{
//	//create a dynamic rigidbody
//
//	//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
//	btCollisionShape* colShape = new btBoxShape(btVector3(1, 1, 1));
//	collisionShapes.push_back(colShape);
//
//	/// Create Dynamic Objects
//	btTransform startTransform;
//	startTransform.setIdentity();
//
//	btScalar mass(1.0f);
//
//	//rigidbody is dynamic if and only if mass is non zero, otherwise static
//	bool isDynamic = (mass != 0.f);
//
//	btVector3 localInertia(0, 0, 0);
//	if (isDynamic)
//		colShape->calculateLocalInertia(mass, localInertia);
//
//	startTransform.setOrigin(startinPos);
//
//	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
//	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
//	btRigidBody* body = new btRigidBody(rbInfo);
//
//	dynamicsWorld->addRigidBody(body);
//	return ID++;
//}
//
btConvexHullShape* World::CreateBoundingConvexModel(StaticMesh& mesh, float scale)
{
	btConvexHullShape* colShape = new btConvexHullShape();

	for (const auto& vertex : mesh.vertices)
	{
		btVector3 vertexVec(vertex.Position.x, vertex.Position.y, vertex.Position.z);


		colShape->addPoint(vertexVec);
	}


	colShape->setLocalScaling(btVector3(scale, scale, scale));



	return colShape;



}

btBoxShape* World::CreateBoundingBoxModel(const std::vector<StaticMesh>& meshes, float scale)
{
	btVector3 modelMin(FLT_MAX, FLT_MAX, FLT_MAX); // Initialize to positive infinity
	btVector3 modelMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& mesh : meshes)
	{
		for (const auto& vertex : mesh.vertices)
		{
			btVector3 vertexVec(vertex.Position.x, vertex.Position.y, vertex.Position.z);

			// Update min values
			modelMin.setMin(vertexVec);

			// Update max values
			modelMax.setMax(vertexVec);
		}
	}


	const btVector3 boxHalfExtents = (modelMax - modelMin) * 0.5f;

	btBoxShape* col = new btBoxShape(boxHalfExtents);
	col->setLocalScaling(btVector3(scale, scale, scale));

	return col;
}

btCapsuleShape* World::CreateBoundingCapsuleModel(const std::vector<StaticMesh>& meshes, float scale)
{
	btVector3 modelMin(FLT_MAX, FLT_MAX, FLT_MAX); // Initialize to positive infinity
	btVector3 modelMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& mesh : meshes)
	{
		for (const auto& vertex : mesh.vertices)
		{
			btVector3 vertexVec(vertex.Position.x, vertex.Position.y, vertex.Position.z);

			// Update min values
			modelMin.setMin(vertexVec);

			// Update max values
			modelMax.setMax(vertexVec);
		}
	}


	// Calculate the capsule radius and half-height
	const float capsuleRadius = (modelMax.getY() - modelMin.getY()) * 0.5f;
	const float capsuleHalfHeight = (modelMax.getX() - modelMin.getX()) * 0.5f;
	//the model is rotated on the Z axis
#ifdef _WINDOWS
	btCapsuleShape* col = new btCapsuleShape(capsuleRadius, capsuleHalfHeight);

#else
	btCapsuleShapeZ* col = new btCapsuleShapeZ(capsuleRadius, capsuleHalfHeight);

#endif

	col->setLocalScaling(btVector3(scale, scale, scale));

	return col;
}

btVector3 World::GetRigidBodyPosition(const uint ID) const
{
	btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[ID];
	btRigidBody* body = btRigidBody::upcast(obj);
	btTransform trans;
	if (body && body->getMotionState())
	{
		body->getMotionState()->getWorldTransform(trans);
	}
	else
	{
		trans = obj->getWorldTransform();
	}
	return trans.getOrigin();
}

btDiscreteDynamicsWorld* World::GetDynamicWorld() const
{
	return dynamicsWorld;
}

void World::AddRigidBody(btRigidBody* rb) const
{
	dynamicsWorld->addRigidBody(rb);
}


void World::Init()
{
	///-----includes_end-----
	///-----initialization_start-----

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	///This thing is callback for collision detection
	dispatcher = new CustomCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(
		dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -0, 0));

	debugDrawer = new DebugRenderer();

	// Create your custom debug drawer
	dynamicsWorld->setDebugDrawer(debugDrawer);


	///-----initialization_end-----
	/////keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!


	///create a few basic rigid bodies

	//the ground is a cube of side 100 at position y = -56.
	//the sphere will hit it at y = -6, with center at -5

	//btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(10.), btScalar(50.)));

	//collisionShapes.push_back(groundShape);

	//btTransform groundTransform;
	//groundTransform.setIdentity();
	//groundTransform.setOrigin(btVector3(0, -20, 0));

	//btScalar mass(0.);

	////rigidbody is dynamic if and only if mass is non zero, otherwise static
	//bool isDynamic = (mass != 0.f);

	//btVector3 localInertia(0, 0, 0);
	//if (isDynamic)
	//	groundShape->calculateLocalInertia(mass, localInertia);

	////using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	//btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
	//btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	//btRigidBody* body = new btRigidBody(rbInfo);

	////add the body to the dynamics world
	//dynamicsWorld->addRigidBody(body);
	//ID++;
}

World::World()
= default;

World::~World()
{
	///-----cleanup_start-----

	//remove the rigidbodies from the dynamics world and delete them
	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}


// [Credt] https://resources.oreilly.com/examples/9781783281879/-/blob/master/Chapter6.1_CollisionEvents/Chapter6.1/BulletOpenGLApplication.cpp
void World::CheckForCollisionEvents()
{
	// Keep a list of the collision pairs we found during the current update
	CollisionPairs pairsThisUpdate;

	// Iterate through all of the manifolds in the dispatcher
	for (int i = 0; i < dispatcher->getNumManifolds(); ++i)
	{
		// Get the manifold.
		btPersistentManifold* pManifold = dispatcher->getManifoldByIndexInternal(i);

		// Ignore manifolds that have no contact allColors.
		if (pManifold->getNumContacts() > 0)
		{
			// Get the two rigid bodies involved in the collision.
			const btRigidBody* pBody0 = static_cast<const btRigidBody*>(pManifold->getBody0());
			const btRigidBody* pBody1 = static_cast<const btRigidBody*>(pManifold->getBody1());

			// Always create the pair in a predictable order (use the pointer value..).
			// (This is because we will add them to a set, which takes unique items only.
			// So pair(objA, objB) would be different than pair(objB, objA) - but they are
			// the same to us in this situation, so we make sure they are ordered consistently).
			const bool swapped = pBody0 > pBody1;
			const btRigidBody* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btRigidBody* pSortedBodyB = swapped ? pBody0 : pBody1;

			// Create the pair
			CollisionPair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			// Insert the pair into the current set
			pairsThisUpdate.insert(thisPair);

			// If this pair doesn't exist in the list from the previous update, 
			// it is a new pair and we must send a collision event.
			if (pairsLastUpdate_.find(thisPair) == pairsLastUpdate_.end())
			{
				CollisionEvent(pBody0, pBody1);
			}
		}
	} // Manifold loop

	// Create another list for pairs that were removed this update.
	CollisionPairs removedPairs;

	// This handy function gets the difference beween two sets. 
	// It takes the difference between collision pairs from the last update, 
	// and this update and pushes them into the removed pairs list.
	std::set_difference(
		pairsLastUpdate_.begin(), pairsLastUpdate_.end(),
		pairsThisUpdate.begin(), pairsThisUpdate.end(),
		std::inserter(removedPairs, removedPairs.begin())
	);

	// Iterate through all of the removed pairs sending separation events for them.
	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter)
	{
		SeparationEvent(iter->first, iter->second);
	}

	// In the next iteration we'll want to compare against the pairs we found in this iteration.
	pairsLastUpdate_ = pairsThisUpdate;
}

void World::CollisionEvent(const btRigidBody* pBody0, const btRigidBody* pBody1)
{
	// Find the two colliding objects.
	GameObject* pObj0 = static_cast<GameObject*>(pBody0->getUserPointer());
	GameObject* pObj1 = static_cast<GameObject*>(pBody1->getUserPointer());

	// Exit if we didn't find anything
	if (!pObj0 || !pObj1)
	{
		return;
	}

	// Tell each object that they have started to collide. Let them figure it out later.
	pObj0->CollidedWith(pObj1);
	pObj1->CollidedWith(pObj0);
}


void World::SeparationEvent(const btRigidBody* pBody0, const btRigidBody* pBody1)
{
	// Find the two separating objects.
	GameObject* pObj0 = static_cast<GameObject*>(pBody0->getUserPointer());
	GameObject* pObj1 = static_cast<GameObject*>(pBody1->getUserPointer());

	// Exit if we didn't find anything
	if (!pObj0 || !pObj1)
	{
		return;
	}

	// Tell each object that they have started to separate. Let them figure it out later.
	pObj0->SeparatedFrom(pObj1);
	pObj1->SeparatedFrom(pObj0);
}
