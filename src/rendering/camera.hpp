#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMode { Trackball, FirstPerson };

class Camera {
public:
  Camera();

  // Calcul de la matrice View
  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix(float aspectRatio) const;
  glm::vec3 getPosition() const;

  // Mode trackball
  void rotateTrackball(float deltaTheta, float deltaPhi);
  void zoomTrackball(float deltaRadius);

  // Mode premiere personne
  void setSubjectivePosition(glm::vec3 pos);
  void setSubjectiveOrientation(float newYaw, float newPitch);
  void rotateSubjective(float deltaYaw, float deltaPitch);

  // Basculer
  void setMode(CameraMode m);
  CameraMode getMode() const { return mode; }

private:
  CameraMode mode{CameraMode::Trackball};

  // Centre du plateau d'échecs (les pièces vont de x=0 à 7, z=0 à 7)
  glm::vec3 target{3.5f, 0.5f, 3.5f};

  // Variables du mode trackball
  float radius{12.0f};
  float theta{glm::radians(45.0f)}; // Angle vertical
  float phi{glm::radians(
      180.0f)}; // Angle horizontal (tourné pour voir les blancs en bas)

  // Variables du mode subjectif
  glm::vec3 subjectivePos{0.0f, 2.0f, 0.0f};
  float yaw{-90.0f};
  float pitch{0.0f};
};
