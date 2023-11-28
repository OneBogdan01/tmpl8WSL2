#include "Camera.h"

#include "template.h"
#include "glm/ext/matrix_transform.hpp"


//void Camera::GetCameraAxis(const float3& cameraTarget)
//{
//	cameraFront = normalize(position - cameraTarget);
//	cameraRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), cameraFront));
//	cameraUp = cross(cameraFront, cameraRight);
//}

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::LookAt() const
{
	return glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::Init()
{
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position = pos;
}

const glm::vec3 Camera::GetPosition() const
{
	return position;
}


void Camera::MoveZ(float multiplier)
{
	cameraFront.y = 0;
	translation += camSpeed * cameraFront * multiplier;
}

void Camera::MoveX(float multiplier)
{
	translation += normalize(cross(cameraFront, cameraUp)) * camSpeed * multiplier;
}

void Camera::Update(float deltaTime)
{
	dir.x = cos(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	dir.y = sin(TO_RADIANS * (pitch));
	dir.z = sin(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	cameraFront = normalize(dir);
	position = position + translation * deltaTime;
	translation = glm::vec3(0);
}


void Camera::RotateMouse(const glm::vec2& p)
{
	float xoffset = p.x;
	float yoffset = -p.y; // reversed since y-coordinates range from bottom to top


	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}
