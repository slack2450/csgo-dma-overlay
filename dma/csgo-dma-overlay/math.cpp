#pragma warning(push, 0)

#include "math.hpp"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#pragma warning(pop)

#define FORWARD_DIRECTION_UNIT glm::vec3(1.0f, 0.0f, 0.0f)
#define RIGHT_DIRECTION_UNIT glm::vec3(0.0f, -1.0f, 0.0f)
#define UP_DIRECTION_UNIT glm::vec3(0.0f, 0.0f, 1.0f)

namespace math
{

    glm::mat4 createProjectionViewMatrix(const glm::vec3& cameraPosition, const glm::vec3& cameraEulerAngles, float aspectRatio, float fieldOfViewYDegrees, float zNear, float zFar) {
        float pitch = -cameraEulerAngles.x * glm::pi<float>() / 180.0f;
        float yaw = cameraEulerAngles.y * glm::pi<float>() / 180.0f;
        float roll = cameraEulerAngles.z * glm::pi<float>() / 180.0f;
        glm::quat cameraRotation = glm::toQuat(glm::rotate(glm::rotate(glm::mat4(1.0f), yaw, UP_DIRECTION_UNIT), pitch, RIGHT_DIRECTION_UNIT));
        return createProjectionViewMatrix(cameraPosition, cameraRotation, aspectRatio, fieldOfViewYDegrees, zNear, zFar);
    }

    glm::mat4 createProjectionViewMatrix(const glm::vec3& cameraPosition, const glm::quat& cameraRotation, float aspectRatio, float fieldOfViewYDegrees, float zNear, float zFar) {
        float fieldOfViewYRadians = fieldOfViewYDegrees * glm::pi<float>() / 180.0f;
        glm::vec3 cameraUpDirectionUnit = glm::normalize(glm::vec3(cameraRotation * glm::vec4(UP_DIRECTION_UNIT, 1.0f))); /* affine transform, no need for homogeneous division */
        glm::mat4 projectionMatrix = glm::perspective(fieldOfViewYRadians, aspectRatio, zNear, zFar);
        glm::vec3 cameraTarget = cameraPosition + glm::vec3(cameraRotation * glm::vec4(FORWARD_DIRECTION_UNIT, 1.0f)); /* affine transform, no need for homogeneous division */
        glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUpDirectionUnit);
        return projectionMatrix * viewMatrix;
    }

    glm::vec2 transformWorldPointIntoScreenSpace(const glm::vec3& worldPoint, const glm::mat4& projectionViewMatrix, float screenWidth, float screenHeight) {
        glm::vec4 screenPointHomogenousSpace = projectionViewMatrix * glm::vec4(worldPoint, 1.0f);
        glm::vec3 screenPoint = glm::vec3(screenPointHomogenousSpace.x, screenPointHomogenousSpace.y, screenPointHomogenousSpace.z) / screenPointHomogenousSpace.w;
        if ((screenPoint.x < -1.0f) || (screenPoint.x > 1.0f) || (screenPoint.y < -1.0f) || (screenPoint.y > 1.0f) || (screenPoint.z < -1.0f) || (screenPoint.z > 1.0f))
            return glm::vec2(std::nanf(""), std::nanf(""));
        return glm::vec2(screenWidth * (screenPoint.x + 1.0f) / 2.0f, screenHeight * (1.0f - (screenPoint.y + 1.0f) / 2.0f));
    }

    bool isWorldPointVisibleOnScreen(const glm::vec3& worldPoint, const glm::mat4& projectionViewMatrix) {
        return !glm::any(glm::isnan(transformWorldPointIntoScreenSpace(worldPoint, projectionViewMatrix, 1.0f, 1.0f)));
    }

}