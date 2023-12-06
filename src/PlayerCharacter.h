#pragma once


#include <btBulletDynamicsCommon.h>

#include "md2_loader/Md2.h"
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "model_loading/StaticModel.h"

//inspired from Abhishek's presentation
class PlayerCharacter
{
public:
	PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition);
	~PlayerCharacter();
	void Draw();
	btTransform GetTransform();
	void InterpolateFrames(float deltaTime);
	void Update(float deltaTime);
	void Jump();
	void GetMoveInput(float input);

private:
	//md2model::Md2 model = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
	btPairCachingGhostObject* ghostObject = nullptr;
	btKinematicCharacterController* characterController;
	StaticModel* playerModel;
	Shader* shader = nullptr;
	float dirX = 0;
	btVector3 dir = btVector3(0, 0, 0);
	float speed = 0.1f;
};
