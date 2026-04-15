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

  struct Uniforms {
    GLint model{-1};
    GLint view{-1};
    GLint projection{-1};
    GLint color{-1};
    GLint viewPos{-1};
    GLint dirLightDir{-1};
    GLint dirLightColor{-1};
    GLint ambientStrength{-1};
    GLint pointLightPos{-1};
    GLint pointLightColor{-1};
    GLint isPiece{-1};
  } uniforms;

  GLuint shaderProgram{0};
  Mesh cubeMesh;
  std::map<std::string, GpuModel> pieceModels;

  void initMesh(Mesh &mesh, const std::vector<float> &vertices,
                const std::vector<unsigned int> &indices);
  void drawMesh(const Mesh &mesh);

  GLuint fbo{0}, textureColorBuffer{0}, rbo{0};
  int currentWidth{0}, currentHeight{0};

  const std::string vertexShaderPath = "../../assets/shaders/shader.vs.glsl";
  const std::string fragmentShaderPath = "../../assets/shaders/shader.fs.glsl";
};