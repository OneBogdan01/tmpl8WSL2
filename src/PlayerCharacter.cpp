#include "PlayerCharacter.h"

#include <iostream>

#include "game.h"
const int startFrame = 0;
const int endFrame = 197;
int renderFrame = startFrame;
// Rendering loop
float interpolation = 0.0f;
int bufferIndex = 0;

PlayerCharacter::PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition)
{
	btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(startingPosition);
	//get the mesh info
	model2 = new Model("assets/Run.dae");

	//create collision shape
	btConvexShape* collider = World::CreateBoundingBoxModel(model2->GetMeshes(), 1.0f);

	// Create a ghost object for collision detection
	ghostObject = new btPairCachingGhostObject();
	ghostObject->setCollisionShape(collider);
	ghostObject->setWorldTransform(playerTransform);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	characterController = new btKinematicCharacterController(ghostObject, collider, 0.05f);
	characterController->setGravity(dynamicsWorld->getGravity());

	dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter,
	                                  btBroadphaseProxy::AllFilter);
	dynamicsWorld->addAction(characterController);
	characterController->setMaxJumpHeight(2.0f);
	shader = new Shader(
		"shaders/ModelLoading.vert",
		"shaders/ModelLoading.frag");
	shader->Bind();
	shader->SetFloat3("objectColor", 1.0f, 0.5f, 0.31f);
	shader->SetFloat3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->Unbind();
}

PlayerCharacter::~PlayerCharacter()
{
	delete shader;
}

void PlayerCharacter::Draw()
{
	Camera& cam = Game::GetCamera();
	const btVector3 pos = GetTransform().getOrigin();
	const glm::vec3 camPos = Game::GetCameraPosition();

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(pos.x(), pos.y(), pos.z());
	model = glm::translate(model, position);
	//model = glm::scale(model, glm::vec3(1.0f));
	shader->Bind();

	shader->SetMat4x4("view", cam.GetViewMat());
	shader->SetMat4x4("projection", cam.GetProjectionMat());
	shader->SetMat4x4("model", model);
	shader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);
	shader->SetFloat3("lightPos", Game::GetLightPos().x, Game::GetLightPos().y, Game::GetLightPos().z);
	shader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	shader->SetFloat("material.shininess", 32.0f);
	shader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader->SetFloat3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	shader->SetFloat3("light.specular", 1.0f, 1.0f, 1.0f);

	shader->SetInt("material.diffuse", 0);
	model2->Draw(*shader);
	shader->Unbind();
}

btTransform PlayerCharacter::GetTransform()
{
	return characterController->getGhostObject()->getWorldTransform();
}

void PlayerCharacter::InterpolateFrames(float deltaTime)
{
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

void PlayerCharacter::Update(float deltaTime)
{
	if (dirX != 0)
	{
		if (characterController->onGround())
		{
			characterController->setWalkDirection(btVector3(0.0001f, 0, 0).normalized() * speed);
			//characterController->setWalkDirection(btVector3(dirX, 0, 0).normalized() * speed);
		}
	}
	else
	{
		characterController->setWalkDirection(btVector3(0, 0, 0));
	}
	//animation stuff
	InterpolateFrames(deltaTime);
}

void PlayerCharacter::Jump()
{
	if (characterController->onGround())
	{
		characterController->jump();
	}
}

void PlayerCharacter::GetMoveInput(float input)
{
	dirX = input;
	std::cout << dirX << "\n";
	btTransform t = GetTransform();
	btVector3 pos = t.getOrigin();
	btQuaternion q = t.getRotation();

	btVector3 rightDir = t.getBasis().getColumn(0); // X-axis

	// Assuming positive input moves to the right and negative input moves to the left
	// You may need to adjust this based on your input system
	dir = btVector3(0, 0, 0);
	if (input > 0)
	{
		dir += rightDir;
	}
	if (input < 0)
	{
		dir -= rightDir;
	}
}
