#pragma once

#include "BulletWrapper.h"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "template.h"
#include "Timer.h"
#include "model_loading/SkinnedMesh.h"

#include "physics/PlayerCollisions.h"

//must be at the end to avoid linker errors
#include "EventArgs.h"
#include "animator/Animation.h"
#include "animator/Animator.h"

class KeyboardManager;

//inspired from Abhishek's presentation

///TODO cleanup

class PlayerCharacter
{
public:
	btTransform SetPositionTransform(const btVector3& startingPosition);
	PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition);
	~PlayerCharacter();
	void Die();
	void SetUpModel();
	void SetGravity();
	void SetNoGravity();
	void CheckForFall();
	void Draw();
	//void SetBoneTransform(uint Index, const Matrix4f& Transform);
	glm::mat4 GetModelMatrix() const;
	void InterpolateFrames(float deltaTime);
	void HandleInput(float deltaTime);
	void MoveCharacter(float deltaTime);
	void Update(float deltaTime);
	VoidEvent& GetEvent();
	void SetRopeP(glm::vec3* ropeP);
private:
	glm::vec3* ropeP = nullptr;
	bool swinging = false;
	btPairCachingGhostObject* playerCharacterGhost = nullptr;
	btKinematicCharacterController* characterController;

	Shader* animationShader = nullptr;

	float dirX = 0;
	btVector3 dir = btVector3(0, 0, 0);
	float speed = 20.0f;
	float jumpSpeed = 13.0f;
	int displayIndex = 32;
	Timer timer;

	//GLuint m_boneLocation[MAX_BONES];
	const int startFrame = 0;
	const int endFrame = 197;
	int renderFrame = startFrame;
	// Rendering loop
	float interpolation = 0.0f;
	int bufferIndex = 0;
	PlayerCollisions* playerCallback = nullptr;
	glm::vec3 position;
	btTransform originalTransform;
	KeyboardManager* inputManager = nullptr;
	bool jumped = false;
	bool onGround = false;
	float minX = -5.0f;
	float maxX = 5.0f;
	 btScalar gravity=100;
	//voidEvent
	VoidEvent onDeath;
	SkinnedModel* playerModel;
	Animation* runAnimation;
	Animator* animator;
	Timer* ropetimer = nullptr;
};
