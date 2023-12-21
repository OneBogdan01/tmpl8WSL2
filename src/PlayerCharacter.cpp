#include "PlayerCharacter.h"

#include <iostream>

#include "game.h"
#include "model_loading/StaticModel.h"
#include "physics/GameValues.h"
#include "tiles/Chunk.h"


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

PlayerCharacter::PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition)
{
	onDeath.connect(&PlayerCharacter::Die, this);
	inputManager = &Game::GetInputManager();

	const btTransform playerTransform = SetPositionTransform(startingPosition);

	//get the mesh info
	//playerModel = new SkinnedMesh();
	//playerModel->LoadMesh("assets/Run.dae");
	//debug
	/*UtilityAssimp util;
	util.PrintMeshInformation("assets/Run.dae");*/
	//create collision shape
#ifdef _WINDOWS
	StaticModel staticPlayerMode("assets\\excalibur\\tris.md2", true);

#else
	StaticModel staticPlayerMode("assets/excalibur/tris.md2", true);

#endif

	btConvexShape* collider = World::CreateBoundingCapsuleModel(staticPlayerMode.GetMeshes(), 0.08f);
	// Create a ghost object for collision detection
	playerCharacterGhost = new btPairCachingGhostObject();
	playerCharacterGhost->setCollisionShape(collider);
	playerCharacterGhost->setWorldTransform(playerTransform);
	playerCharacterGhost->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	playerCharacterGhost->setUserPointer(&playerCallback);
	characterController = new btKinematicCharacterController(playerCharacterGhost, collider, 0.01f);
#ifdef _WINDOWS

	btScalar gravity = -dynamicsWorld->getGravity().getY() * 10;
	characterController->setGravity(gravity);
#else
	characterController->setGravity(dynamicsWorld->getGravity() * 10);
#endif

	//makes the player fall on the edge of the platform
	characterController->setMaxSlope(btScalar(0.0f));
	//characterController->setStepHeight(btScalar(0.0f));
	dynamicsWorld->addCollisionObject(playerCharacterGhost, btBroadphaseProxy::CharacterFilter,
	                                  btBroadphaseProxy::AllFilter);
	dynamicsWorld->addAction(characterController);

	characterController->setMaxJumpHeight(2.0f);
	///set them sometime
	characterController->setFallSpeed(50.0f);
	characterController->setJumpSpeed(40.0f);

	originalTransform = characterController->getGhostObject()->getWorldTransform();
}

PlayerCharacter::~PlayerCharacter()
{
	delete shader;
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
	Camera& cam = Game::GetCamera();
	const glm::vec3 camPos = Game::GetCameraPosition();

	glm::mat4 model = GetModelMatrix();
	position = glm::vec3(model[3]);
	CheckForFall();

	player.SetPosition(position);
	player.Draw(renderFrame, 0, interpolation, Game::camera->GetViewMat(), Game::camera->GetProjectionMat());
}


glm::mat4 PlayerCharacter::GetModelMatrix() const
{
	const GameValues* values = nullptr;
	values = new GameValues(characterController->getGhostObject()->getWorldTransform());

	btTransform trans;
	values->getWorldTransform(trans);
	float mat4[16]{0.0f};
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
}


void PlayerCharacter::MoveCharacter(float deltaTime)
{
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

	btTransform currentTransform = playerCharacterGhost->getWorldTransform();
	btVector3 currentPosition = currentTransform.getOrigin();

	if (characterController->onGround())
	{
		onGround = true;
	}
	if (!inputManager->IsPressed(Jump))
	{
		jumped = false;
	}

	if (inputManager->IsPressed(Jump) && !jumped && onGround)
	{
		jumped = true;
		onGround = false;
		std::cout << "jump" << std::endl;
		playerCallback.onGround = false;
		characterController->jump();
	}


	currentPosition.setX(btClamped(currentPosition.getX(), minX, maxX));


	currentTransform.setOrigin(currentPosition);
	playerCharacterGhost->setWorldTransform(currentTransform);
}

void PlayerCharacter::Update(float deltaTime)
{
	HandleInput(deltaTime);
	MoveCharacter(deltaTime);
	InterpolateFrames(deltaTime);
}

VoidEvent& PlayerCharacter::GetEvent()
{
	return onDeath;
}
