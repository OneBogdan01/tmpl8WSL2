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
    playerModel = new SkinnedMesh();
    playerModel->LoadMesh("assets/Run.dae");
    StaticModel model("assets/Run.dae");
    //debug
    UtilityAssimp util;
    util.PrintMeshInformation("assets/Run.dae");
    //create collision shape
    btConvexShape* collider = World::CreateBoundingBoxModel(model.GetMeshes(), 1.0f);

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
        "shaders/Skinned.vert",
        "shaders/Skinned.frag");
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

    for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation); i++)
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
    }
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
    glm::vec3 position = glm::vec3(0, 0, -5.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(1.0f));
    shader->Bind();

    shader->SetMat4x4("view", cam.GetViewMat());
    shader->SetMat4x4("projection", cam.GetProjectionMat());
    shader->SetMat4x4("model", model);
    shader->SetFloat3("viewPos", camPos.x, camPos.y, camPos.z);

    vector<Matrix4f> transforms;
    float deltaTime = timer.elapsed();
    playerModel->GetBoneTransforms(deltaTime, transforms);

    for (uint i = 0; i < transforms.size(); i++)
    {
        SetBoneTransform(i, transforms[i]);
    }

    playerModel->Render();

    shader->Unbind();
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
    //InterpolateFrames(deltaTime);
    if (Game::GetInputManager().IsJustPressed(Action::Jump))
    {
        displayIndex++;
        displayIndex = displayIndex % playerModel->NumBones();
    }
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
