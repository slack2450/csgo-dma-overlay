#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/quaternion.hpp>

namespace math
{
	glm::mat4 createProjectionViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& cameraEulerAngles, float aspectRatio = 16.0f / 9.0f, float fieldOfViewYDegrees = 75.0f, float zNear = 1.f, float zFar = 4000.0f);

	glm::mat4 createProjectionViewMatrix(const glm::vec3& cameraPosition, const glm::quat& cameraRotation, float aspectRatio = 16.0f / 9.0f, float fieldOfViewYDegrees = 75.0f, float zNear = 1.f, float zFar = 4000.0f);

	glm::vec2 transformWorldPointIntoScreenSpace(const glm::vec3& worldPoint, const glm::mat4& projectionViewMatrix, float screenWidth, float screenHeight);
}