#pragma once


#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "md2_loader/Md2.h"


#include "Timer.h"
#include "model_loading/SkinnedMesh.h"
#include "physics/GameValues.h"


//inspired from Abhishek's presentation
class PlayerCharacter
{
public:
	PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition);
	~PlayerCharacter();
	void Draw();
	void SetBoneTransform(uint Index, const Matrix4f& Transform);
	glm::mat4 GetModelMatrix() const;
	void InterpolateFrames(float deltaTime);
	void HandleInput();
	void Update(float deltaTime);

private:
	//md2model::Md2 model = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
	btPairCachingGhostObject* ghostObject = nullptr;
	btKinematicCharacterController* characterController;
	SkinnedMesh* playerModel = nullptr;
	Shader* shader = nullptr;
	float dirX = 0;
	btVector3 dir = btVector3(0, 0, 0);
	float speed = 0.1f;
	int displayIndex = 32;
	Timer timer;
	md2model::Md2 player = md2model::Md2("assets/excalibur/tris.md2", "assets/excalibur/alphaone.png");
	//GLuint m_boneLocation[MAX_BONES];
	const int startFrame = 0;
	const int endFrame = 197;
	int renderFrame = startFrame;
	// Rendering loop
	float interpolation = 0.0f;
	int bufferIndex = 0;
};
