#pragma once

#include <glm/vec3.hpp>

#include "Timer.h"
#include "glm/fwd.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 GetViewMat() const;
	glm::mat4 GetProjectionMat() const;
	void UpdateRotation();
	void Init();
	void SetPosition(const glm::vec3& pos);
	const glm::vec3 GetPosition() const;
	void MoveZ(float multiplier);
	void MoveX(float multiplier);
	void MovementCamera(float deltaTime);
	void InterpolateTo(float deltaTime);
	void Update(float deltaTime);
	void HandleInput(float deltaTime);
	//void SetViewMatrix(Shader* shader);
	void RotateMouse(const glm::vec2& p);
	void InterpolateRotation(glm::vec3* lookAt);
private:
	//void GetCameraAxis(const float3& cameraTarget);
	//	float lastX = 0, lastY = 0;
	glm::vec3 position = glm::vec3(0.0f, 17.0f, 18.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 dir = glm::vec3(0.0f);
	glm::vec3 translation = glm::vec3(0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const float camSpeed = 20.0f;
	Timer timer;
	bool interpolating = false;
	const float timeToRotate = 2.0f;
	glm::quat startRotation;
	glm::quat endRotation;
	glm::quat currentRotation;
	//euler angles
	float yaw = -90.0f;
	float pitch = 20.0f;
	float yawSpeed = 50.0f;
	float pitchSpeed = 50.0f;
	float fov = 45.0f;
};
