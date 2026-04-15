#pragma once

#include "camera.hpp"
#include "game_logic.hpp"
#include "gl_utils.hpp"
#include "utils.hpp"
#include <array>
#include <map>
#include <string>
#include <vector>

class Scene3D {
public:
  Scene3D();
  ~Scene3D();

  void init();
  void resize(int width, int height);
  GLuint renderToTexture(const Camera &camera, int width, int height,
                         GameLogic &game, coords hoveredSquare,
                         coords selectedSquare,
                         const std::vector<std::array<int, 2>> &validMoves);

  void pushAnimation(coords from, coords to);
  bool isAnimationActive() const;
  bool getAnimatedWorldPositionFromSource(coords source,
                                          glm::vec3 &outWorldPos) const;

private:
  struct AnimationState {
    bool active{false};
    coords source{-1, -1};
    coords target{-1, -1};
    double startTime{0.0};
    double duration{0.5}; // 0.5 secondes
  } currentAnim;

  struct GpuModel {
    GLuint vao{0};
    GLuint vbo{0};
    int vertexCount{0};
  };

  struct Mesh {
    GLuint vao{0}, vbo{0}, ebo{0};
    int indexCount{0};
  };

  GLuint shaderProgram{0};
  Mesh cubeMesh;
  std::map<std::string, GpuModel> pieceModels;

  void initMesh(Mesh &mesh, const std::vector<float> &vertices,
                const std::vector<unsigned int> &indices);
  void drawMesh(const Mesh &mesh);

  GLuint fbo{0}, textureColorBuffer{0}, rbo{0};
  int currentWidth{0}, currentHeight{0};

  const std::string vertexShaderSrc = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform vec3 color;

        out vec3 FragPos;
        out vec3 Normal;
        out vec3 Color;

        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            Color = color;
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

  const std::string fragmentShaderSrc = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;
        in vec3 Color;

        // Light 1 : Directionnelle (ambiance globale)
        uniform vec3 dirLightDir;
        uniform vec3 dirLightColor;
        uniform float ambientStrength;

        // Light 2 : Point light mobile
        uniform vec3 pointLightPos;
        uniform vec3 pointLightColor;

        uniform vec3 viewPos;
        uniform bool isPiece;

        void main() {
            vec3 norm = normalize(Normal);
            vec3 viewDir = normalize(viewPos - FragPos);

            // 1. Directional Light
            vec3 ambient = ambientStrength * dirLightColor;
            vec3 lightDir = normalize(-dirLightDir);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * dirLightColor;

            float spec = 0.0;
            if (isPiece) { // Blinn-Phong pour les pièces
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
            }
            vec3 specular = 0.5 * spec * dirLightColor;
            
            vec3 resultDir = ambient + diffuse + specular;

            // 2. Point Light (mobile)
            vec3 lightDirPt = normalize(pointLightPos - FragPos);
            float diffPt = max(dot(norm, lightDirPt), 0.0);
            
            float distance = length(pointLightPos - FragPos);
            float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
            
            vec3 diffusePt = diffPt * pointLightColor * attenuation;
            
            float specPt = 0.0;
            if (isPiece) {
                vec3 halfwayDirPt = normalize(lightDirPt + viewDir);  
                specPt = pow(max(dot(norm, halfwayDirPt), 0.0), 64.0);
            }
            vec3 specularPt = 1.0 * specPt * pointLightColor * attenuation;
            
            vec3 resultPoint = diffusePt + specularPt;
            
            // Rim lighting (bonus effet stylisé sur les bords)
            vec3 rim = vec3(0.0);
            if (isPiece) {
                float rimFactor = 1.0 - max(dot(viewDir, norm), 0.0);
                rimFactor = smoothstep(0.6, 1.0, rimFactor);
                rim = rimFactor * pointLightColor * attenuation;
            }

            vec3 finalColor = (resultDir + resultPoint + rim) * Color;
            FragColor = vec4(finalColor, 1.0);
        }
    )";
};