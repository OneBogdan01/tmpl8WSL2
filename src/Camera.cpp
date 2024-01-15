#include "Camera.h"

#include "game.h"
#include "KeyboardManager.h"
#include "glm/ext/matrix_clip_space.hpp"
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

glm::mat4 Camera::GetViewMat() const
{
	// Assuming your quaternion is named "cameraOrientation"
	glm::mat4 viewMatrix = glm::mat4_cast(currentRotation);

	// Translate the view matrix to the camera position
	viewMatrix = glm::translate(viewMatrix, -position);

	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMat() const
{
	return glm::perspective(glm::radians(fov),
		static_cast<float>(SCRWIDTH) / static_cast<float>(SCRHEIGHT),
		0.1f, 100.0f);
}

void Camera::UpdateRotation()
{

	startRotation = glm::quatLookAt(cameraFront, cameraUp);

	currentRotation = startRotation;

}

void Camera::Init()
{
	//we can concatenate the rotations
	UpdateRotation();
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
	translation += camSpeed * (cameraFront)*multiplier;
}

void Camera::MoveX(float multiplier)
{
	translation += normalize(cross(cameraFront, cameraUp)) * camSpeed * multiplier;
}

void Camera::MovementCamera(float deltaTime)
{
	if (Game::freeCam)
		HandleInput(deltaTime);
	dir.x = cos(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	dir.y = sin(TO_RADIANS * (pitch));
	dir.z = sin(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));

	cameraFront = normalize(dir);
	UpdateRotation();

	position = position + translation * deltaTime;

	translation = glm::vec3(0);
}

void Camera::InterpolateTo(float deltaTime)
{
	if (!interpolating)
		return;
	float t = timer.elapsed();
	float step = t / timeToRotate;
	std::cout << step << "\n";

	currentRotation = glm::slerp(startRotation, endRotation, step);
	if (t > timeToRotate)
	{
		interpolating = false;

		std::cout << "Finished camera interpolation'\n";
		return;
	}

}

void Camera::Update(float deltaTime)
{
	MovementCamera(deltaTime);
	InterpolateTo(deltaTime);
}


void Camera::HandleInput(const float deltaTime)
{
	// Get inputs.
	const KeyboardManager& km = Game::GetInputManager();
	int xMovement{ km.IsPressed(Action::MoveLeft) ? -1 : 0 + km.IsPressed(Action::MoveRight) ? 1 : 0 };
	int zMovement{ km.IsPressed(Action::MoveForward) ? 1 : 0 + km.IsPressed(Action::MoveBackward) ? -1 : 0 };

	int yawAmount{ km.IsPressed(Action::YawLeft) ? 1 : 0 + km.IsPressed(Action::YawRight) ? -1 : 0 };
	int pitchAmount{ km.IsPressed(Action::PitchUp) ? 1 : 0 + km.IsPressed(Action::PitchDown) ? -1 : 0 };

	// Apply inputs.
	MoveX(static_cast<float>(xMovement));
	MoveZ(static_cast<float>(zMovement));

	yaw += static_cast<float>(yawAmount) * yawSpeed * deltaTime;
	pitch -= static_cast<float>(pitchAmount) * pitchSpeed * deltaTime;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
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
}

void Camera::InterpolateRotation(glm::vec3* lookAt)
{
	lookAt->x = 0;
	lookAt->z = 0;
	//still interpolating
	if (interpolating)
		return;
	interpolating = true;
	timer.reset();
	endRotation = glm::quatLookAt(glm::normalize(*lookAt - position), cameraUp);

	std::cout << "Init camera interpolation'\n";
}


