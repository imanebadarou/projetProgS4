#include "rendering/camera.hpp"
#include <algorithm>

Camera::Camera() {}

glm::mat4 Camera::getViewMatrix() const {
  if (mode == CameraMode::Trackball) {
    float x = radius * sin(theta) * cos(phi);
    float y = radius * cos(theta);
    float z = radius * sin(theta) * sin(phi);

    glm::vec3 pos = target + glm::vec3(x, y, z);
    return glm::lookAt(pos, target, glm::vec3(0.0f, 1.0f, 0.0f));
  } else {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    return glm::lookAt(subjectivePos, subjectivePos + front,
                       glm::vec3(0.0f, 1.0f, 0.0f));
  }
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
  float fov = (mode == CameraMode::FirstPerson) ? 75.0f : 45.0f;
  return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
}

glm::vec3 Camera::getPosition() const {
  if (mode == CameraMode::Trackball) {
    float x = radius * sin(theta) * cos(phi);
    float y = radius * cos(theta);
    float z = radius * sin(theta) * sin(phi);
    return target + glm::vec3(x, y, z);
  }
  return subjectivePos;
}

void Camera::rotateTrackball(float deltaTheta, float deltaPhi) {
  if (mode != CameraMode::Trackball)
    return;

  phi += deltaPhi;
  theta += deltaTheta;

  // Ne pas passer sous le plateau (y < 0 => theta au delà de PI/2)
  const float epsilon = 0.01f;
  theta = std::clamp(theta, epsilon, glm::radians(89.0f));
}

void Camera::zoomTrackball(float deltaRadius) {
  if (mode != CameraMode::Trackball)
    return;
  radius -= deltaRadius;
  radius = std::max(2.0f, radius);
}

void Camera::setSubjectivePosition(glm::vec3 pos) { subjectivePos = pos; }

void Camera::setSubjectiveOrientation(float newYaw, float newPitch) {
  yaw = newYaw;
  pitch = newPitch;
}

void Camera::rotateSubjective(float deltaYaw, float deltaPitch) {
  if (mode != CameraMode::FirstPerson)
    return;
  yaw += deltaYaw;
  pitch += deltaPitch;
}

void Camera::setMode(CameraMode m) { mode = m; }
