#include "PlayerCharacter.h"

#include <iostream>
#include "model_loading/UtilityAssimp.h"
#include "game.h"
#include "model_loading/StaticModel.h"

PlayerCharacter::PlayerCharacter(btDiscreteDynamicsWorld* dynamicsWorld, const btVector3& startingPosition)
{
	btTransform playerTransform;
	playerTransform.setIdentity();
	playerTransform.setOrigin(startingPosition);
	//get the mesh info
	//playerModel = new SkinnedMesh();
	//playerModel->LoadMesh("assets/Run.dae");
	//debug
	/*UtilityAssimp util;
	util.PrintMeshInformation("assets/Run.dae");*/
	//create collision shape
	StaticModel staticPlayerMode("assets/excalibur/tris.md2", true);

	btConvexShape* collider = World::CreateBoundingBoxModel(staticPlayerMode.GetMeshes(), 0.1f);

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
	characterController->setMaxJumpHeight(3.0f);
	///set them sometime
	characterController->setFallSpeed(5.0f);
	characterController->setJumpSpeed(14.0f);
	shader = new Shader(
		"shaders/ModelLoading.vert",
		"shaders/ModelLoading.frag");
	shader->Bind();
	shader->SetFloat3("lightPos", Game::GetLightPos().x, Game::GetLightPos().y, Game::GetLightPos().z);
	shader->SetFloat3("material.specular", 0.5f, 0.5f, 0.5f);
	shader->SetFloat("material.shininess", 32.0f);
	shader->SetFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	shader->SetFloat3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken diffuse light a bit
	shader->SetFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	shader->SetInt("material.diffuse", 0);
	shader->SetFloat3("objectColor", 1.0f, 0.5f, 0.0f);
	shader->SetFloat3("lightColor", 1.0f, 1.0f, 1.0f);

	/*for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++)
	{
		char Name[128];
		memset(Name, 0, sizeof(Name));
		SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
		GLuint Location = glGetUniformLocation(shader->ID, Name);

		if (Location == INVALID_UNIFORM_LOCATION)
		{
			fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", "gBones");
		}
		m_boneLocation[i] = Location;
		printf("m_boneLocation[%d] = %d\n", i, m_boneLocation[i]);
	}*/
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
	model = glm::scale(model, glm::vec3(1.0f));
	shader->Bind();

	shader->SetMat4x4("view", cam.GetViewMat());
	shader->SetMat4x4("projection", cam.GetProjectionMat());
	shader->SetMat4x4("model", model);
	shader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);

	/*vector<Matrix4f> transforms;
	float deltaTime = timer.elapsed();
	playerModel->GetBoneTransforms(deltaTime, transforms);

	for (uint i = 0; i < transforms.size(); i++)
	{
		SetBoneTransform(i, transforms[i]);
	}*/
	shader->Unbind();
	player.SetPosition(position);
	player.Draw(renderFrame, 0, interpolation, Game::camera->GetViewMat(), Game::camera->GetProjectionMat());


}

void PlayerCharacter::SetBoneTransform(uint Index, const Matrix4f& Transform)
{
	assert(Index < MAX_BONES);
	if (Index >= MAX_BONES)
	{
		return;
	}
	//Transform.Print();
	glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, (const GLfloat*)Transform);
}

btTransform PlayerCharacter::GetTransform()
{
	return characterController->getGhostObject()->getWorldTransform();
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

void PlayerCharacter::HandleInput()
{
	KeyboardManager& inputManager = Game::GetInputManager();

	dir = btVector3(0, 0, 0);
	int dirX = 0;
	dirX += inputManager.IsPressed(Action::MoveLeft) ? -1 : 0;
	dirX += inputManager.IsPressed(Action::MoveRight) ? 1 : 0;
	dir += btVector3(static_cast<float>(dirX), 0, 0);
	if (dirX != 0)
	{
		if (characterController->onGround())
		{
			characterController->setWalkDirection(btVector3(dir).normalized() * speed);

		}
		else
		{
			//maybe slower
			characterController->setWalkDirection(btVector3(dir).normalized() * speed);

		}
	}
	else
	{
		characterController->setWalkDirection(btVector3(0, 0, 0));
	}
	if (inputManager.IsJustPressed(Action::Jump))
		if (characterController->onGround())
		{
			characterController->jump();
		}
	cout << dirX << "\n";

}


void PlayerCharacter::Update(float deltaTime)
{
	HandleInput();

	InterpolateFrames(deltaTime);

}



