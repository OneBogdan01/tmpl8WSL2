#include "PlayerCharacter.h"

#include <iostream>

#include "game.h"
#include "glm/gtx/rotate_vector.hpp"
#include "model_loading/StaticModel.h"
#include "physics/GameValues.h"
#include "tiles/Chunk.h"
#include "utilities/RandomNumberGenerator.h"


btTransform PlayerCharacter::SetPositionTransform(const btVector3& startingPosition)
{
	btTransform playerTransform;

	playerTransform.setIdentity();
	playerTransform.setOrigin(startingPosition);
	return playerTransform;
}

void PlayerCharacter::Die()
{
	playerCharacterGhost->setWorldTransform(originalTransform);
}

void PlayerCharacter::SetUpModel()
{
	playerModel = new SkinnedModel("assets/Run.dae");


	runAnimation = new Animation("assets/Run.dae", playerModel);
	animator = new Animator(runAnimation);
}

void PlayerCharacter::SetGravity()
{
#ifdef _WINDOWS

	characterController->setGravity(gravity);
#else
	characterController->setGravity(btVector3(0, -gravity, 0));
#endif
}

void PlayerCharacter::SetNoGravity()
{
#ifdef _WINDOWS

	characterController->setGravity(btScalar(0));
#else
	characterController->setGravity(btVector3(0, 0, 0));
#endif
}

void PlayerCharacter::PlayerCharacterSetUp(btDiscreteDynamicsWorld* dynamicsWorld, const btTransform playerTransform, StaticModel staticPlayerMode)
{
	btConvexShape* collider = World::CreateBoundingCapsuleModel(staticPlayerMode.GetMeshes(), 1.5f);
	// Create a ghost object for collision detection
	playerCharacterGhost = new btPairCachingGhostObject();
	playerCharacterGhost->setCollisionShape(collider);
	playerCharacterGhost->setWorldTransform(playerTransform);
	playerCharacterGhost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	playerCharacterGhost->setUserPointer(playerCallback);
	characterController = new btKinematicCharacterController(playerCharacterGhost, collider, 0.01f);
	SetGravity();
	//makes the player fall on the edge of the platform
	characterController->setMaxSlope(btScalar(0.0f));
	//characterController->setStepHeight(btScalar(0.0f));
	dynamicsWorld->addCollisionObject(playerCharacterGhost, btBroadphaseProxy::CharacterFilter,
		btBroadphaseProxy::AllFilter);
	dynamicsWorld->addAction(characterController);

	characterController->setMaxJumpHeight(3.0f);
	///set them sometime
	characterController->setFallSpeed(50.0f);
	characterController->setJumpSpeed(40.0f);
	originalTransform = characterController->getGhostObject()->getWorldTransform();

}

PlayerCharacter::PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition)
{

	onDeath.connect(&PlayerCharacter::Die, this);
	inputManager = &Game::GetInputManager();
	playerCallback = new PlayerCollisions(GameObject::Player, &onDeath);
	playerCallback->onRope->connect(&PlayerCharacter::SetRopeP, this);
	const btTransform playerTransform = SetPositionTransform(startingPosition);
	ropetimer = new Timer();
	whipTimer = new Timer();
	animationShader = new Shader("assets/shaders/Skinned.vert",
		"assets/shaders/Skinned.frag");

	Game::lightManager->SetLightProperties(*animationShader);
	animationShader->Bind();

	animationShader->SetVec3("material.specular", glm::vec3(0.5f));
	animationShader->SetVec3("dirLight.ambient", glm::vec3(0.1f));
	animationShader->SetVec3("dirLight.diffuse", glm::vec3(1.0f, 0.71f, 0.75f));
	animationShader->SetFloat("material.shininess", 64.0f);
	animationShader->Unbind();

	//create collision shape

	StaticModel staticPlayerMode("assets/Run.dae", true);


	PlayerCharacterSetUp(dynamicsWorld, playerTransform, staticPlayerMode);
	whipTrigger = new WhipTrigger(GameObject::Whip);
	AddATriggerBox();

}

void PlayerCharacter::DrawRandomWhipLine()
{
	btVector3 from = GlmToBtVector3(position);

	for (int i = 0; i < 10; i++) {
		float sign = RandomNumberGenerator::RandomFloat() > 0.5f ? 1.0f : -1.0f;
		float randomCoodinate = RandomNumberGenerator::RandomFloat() * 3 * sign;
		btVector3 nexPoint = btVector3(randomCoodinate + position.x, abs(randomCoodinate) + position.y, -i * 2);
		whipLine.StoreLine(from, nexPoint, btVector3(1, 1, 1));
		from = nexPoint;
	}
	
}

void PlayerCharacter::AddWhip()
{
	DrawRandomWhipLine();
	Game::world.AddRigidBody(whipRB);
	whipTimer->reset();
	whipping = true;
}
void PlayerCharacter::RemoveWhip()
{
	whipLine.DeleteLines();
	Game::world.RemoveRigidBody(whipRB);
	whipping = false;
}

void PlayerCharacter::AddATriggerBox()
{
	//make a collision shape
	// Create a collision shape (e.g., a box shape for a rectangular tile)
	btBoxShape* tileShape = new btBoxShape(btVector3(1, 1, 5));

	// Create a motion state
	btTransform tileTransform;
	tileTransform.setIdentity();
	tileTransform.setOrigin(btVector3(whipPosition.x, whipPosition.y,
		whipPosition.z));


	//whipRB = new btGhostObject();
	//whipRB->setWorldTransform(tileTransform);
	//whipRB->setCollisionShape(tileShape);
	//whipRB->setCollisionFlags(whipRB->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);


	//Game::world.AddTrigger(whipRB);


	//make a collision shape

	// Create a motion state

	btDefaultMotionState* tileMotionState = new btDefaultMotionState(tileTransform);

	// Create a rigid body
	btScalar mass = 0.0; // Mass 0 for static objects
	btVector3 localInertia(0, 0, 0);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, tileMotionState, tileShape, localInertia);

	whipRB = new btRigidBody(rbInfo);
	//set it as kinamatic
	whipRB->setCollisionFlags(whipRB->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	whipRB->setActivationState(DISABLE_DEACTIVATION);

	whipRB->setUserPointer(whipTrigger);

	AddWhip();

}
PlayerCharacter::~PlayerCharacter()
{
	delete animator;
	delete runAnimation;
	delete playerModel;
	delete animationShader;
	delete ropetimer;
}


void PlayerCharacter::CheckForFall()
{
	if (position.y < 0)
	{
		//die logic

		onDeath();
	}
}

void PlayerCharacter::Draw()
{
	glm::mat4 posMat = GetModelMatrix();
	position = glm::vec3(posMat[3]);

	animationShader->Bind();
	animationShader->SetMat4x4("view", Game::camera->GetViewMat());
	animationShader->SetMat4x4("projection", Game::camera->GetProjectionMat());
	auto transforms = animator->GetFinalBoneMatrices();
	for (int i = 0; i < transforms.size(); ++i)
	{
		string path = "finalBonesMatrices[" + std::to_string(i) + "]";

		animationShader->SetMat4x4(path.c_str(), transforms[i]);
	}


	// render the loaded model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position - glm::vec3(0, 1.5f, 0));
	// translate it down so it's at the center of the scene
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.f)); // it's a bit too big for our scene, so scale it down
	animationShader->SetMat4x4("model", model);
	playerModel->Draw(*animationShader);
}


glm::mat4 PlayerCharacter::GetModelMatrix() const
{
	const GameValues* values = nullptr;
	values = new GameValues(characterController->getGhostObject()->getWorldTransform());

	btTransform trans;
	values->getWorldTransform(trans);
	float mat4[16]{ 0.0f };
	trans.getOpenGLMatrix(mat4);
	delete values;
	return {
		mat4[0], mat4[1], mat4[2], mat4[3],
		mat4[4], mat4[5], mat4[6], mat4[7],
		mat4[8], mat4[9], mat4[10], mat4[11],
		mat4[12], mat4[13], mat4[14], mat4[15],
	};
}

void PlayerCharacter::InterpolateFrames(float deltaTime)
{
	//animation stuff
	if (interpolation >= 1.0f)
	{
		interpolation = 0.0f;
		if (renderFrame == endFrame)
		{
			renderFrame = startFrame;
			bufferIndex = 0;
		}
		else
		{
			renderFrame++;
			bufferIndex++;
		}
	}
	interpolation += 10.0f * deltaTime;
}

void PlayerCharacter::HandleInput(float deltaTime)
{
	dir = btVector3(0, 0, 0);
	int dirX = 0;
	dirX += inputManager->IsPressed(Action::MoveLeft) ? -1 : 0;
	dirX += inputManager->IsPressed(Action::MoveRight) ? 1 : 0;
	dir += btVector3(static_cast<float>(dirX), 0, 0);
	if (!whipping && inputManager->IsPressed(Action::Whip))
	{
		AddWhip();
	}
}


void PlayerCharacter::SetWhipPosition()
{

	btTransform newTransform;
	whipRB->getMotionState()->getWorldTransform(newTransform);

	newTransform.setOrigin(btVector3(position.x, position.y, position.z - 10));
	whipRB->getMotionState()->setWorldTransform(newTransform);
}

void PlayerCharacter::MoveCharacter(float deltaTime)
{
	CheckForFall();

	btTransform currentTransform = playerCharacterGhost->getWorldTransform();
	btVector3 currentPosition = currentTransform.getOrigin();
	if (whipping) {
		if (whipTimer->elapsed() > .15f)
		{
			RemoveWhip();
		}
		else
		{
			whipLine.DrawLine(false);
			SetWhipPosition();
		}

	}
	if (swinging)
	{
		if (ropetimer->elapsed() > .5f)
		{
			swinging = false;
			currentTransform.setOrigin(currentPosition);

			playerCharacterGhost->setWorldTransform(currentTransform);
			return;
		} //make bullet stop

		characterController->setWalkDirection(btVector3(0, 0, 0));
		currentPosition.setY(ropeP->y);
		currentTransform.setOrigin(currentPosition);


		playerCharacterGhost->setWorldTransform(currentTransform);
		return;
	}
	SetGravity();

	if (dir != btVector3(0, 0, 0))
	{
		if (characterController->onGround())
		{
			characterController->setWalkDirection(btVector3(dir).normalized() * speed * deltaTime);
		}
		else
		{
			//maybe slower
			characterController->setWalkDirection(btVector3(dir).normalized() * jumpSpeed * deltaTime);
		}
	}
	else
	{
		characterController->setWalkDirection(btVector3(0, 0, 0));
	}


	if (characterController->onGround())
	{
		onGround = true;
	}

	if (inputManager->IsPressed(Jump) && !jumped && onGround)
	{
		//jumped = true;
		onGround = false;
		std::cout << "jump" << std::endl;
		//TODO sometimes bullet gets stuck and does not jump here
		characterController->jump();
	}


	currentPosition.setX(btClamped(currentPosition.getX(), minX, maxX));
	//currentPosition.setZ(btClamped(currentPosition.getZ(), 0.0f, 0.0f));


	currentTransform.setOrigin(currentPosition);
	playerCharacterGhost->setWorldTransform(currentTransform);
}

void PlayerCharacter::Update(float deltaTime)
{
	HandleInput(deltaTime);
	InterpolateFrames(deltaTime);
	animator->UpdateAnimation(deltaTime);
	MoveCharacter(deltaTime);
}

VoidEvent& PlayerCharacter::GetEvent()
{
	return onDeath;
}

void PlayerCharacter::SetRopeP(glm::vec3* _ropeP)
{
	SetNoGravity();
	ropetimer->reset();
	//std::cout<<ropetimer->elapsed()<<std::endl;
	if (swinging)
	{
		return;
	}
	ropeP = _ropeP;
	swinging = true;
	//characterController->jump();
}
