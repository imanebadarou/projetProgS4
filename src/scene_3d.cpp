#include "scene_3d.hpp"
#include "obj_loader.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Scene3D::Scene3D() {}

Scene3D::~Scene3D() {
  if (cubeMesh.vao)
    glDeleteVertexArrays(1, &cubeMesh.vao);
  if (cubeMesh.vbo)
    glDeleteBuffers(1, &cubeMesh.vbo);
  if (cubeMesh.ebo)
    glDeleteBuffers(1, &cubeMesh.ebo);

  for (auto &pair : pieceModels) {
    if (pair.second.vao)
      glDeleteVertexArrays(1, &pair.second.vao);
    if (pair.second.vbo)
      glDeleteBuffers(1, &pair.second.vbo);
  }

  if (shaderProgram)
    glDeleteProgram(shaderProgram);
  if (fbo)
    glDeleteFramebuffers(1, &fbo);
  if (textureColorBuffer)
    glDeleteTextures(1, &textureColorBuffer);
  if (rbo)
    glDeleteRenderbuffers(1, &rbo);
}

void Scene3D::init() {
  shaderProgram = createProgram(vertexShaderSrc, fragmentShaderSrc);

  const std::vector<float> vertices = {
      // positions          // normals
      // back
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
      -1.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, -0.5f, 0.5f, -0.5f, 0.0f,
      0.0f, -1.0f,
      // front
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
      // left
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, -0.5f, 0.5f, -0.5f, -1.0f, 0.0f,
      0.0f, -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, 0.5f, -1.0f,
      0.0f, 0.0f,
      // right
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
      // bottom
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,
      0.0f, 0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, -0.5f, -0.5f, 0.5f, 0.0f,
      -1.0f, 0.0f,
      // top
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f};

  const std::vector<unsigned int> indices = {
      0,  1,  2,  2,  3,  0,  // back
      4,  5,  6,  6,  7,  4,  // front
      8,  9,  10, 10, 11, 8,  // left
      12, 13, 14, 14, 15, 12, // right
      16, 17, 18, 18, 19, 16, // bottom
      20, 21, 22, 22, 23, 20  // top
  };

  initMesh(cubeMesh, vertices, indices);

  // Chargement propre de tous les modèles 3D des pièces
  std::vector<std::string> colors = {"white", "black"};
  std::vector<std::string> names = {"pawn",   "rook",  "knight",
                                    "bishop", "queen", "king"};

  for (const auto &color : colors) {
    for (const auto &name : names) {
      std::string key = color + "-" + name;
      std::string path = "../../assets/3Dmodels/" + key + ".obj";

      ModelData data = loadOBJ(path);
      if (data.vertexCount > 0) {
        GpuModel gpu;
        gpu.vertexCount = data.vertexCount;

        glGenVertexArrays(1, &gpu.vao);
        glGenBuffers(1, &gpu.vbo);

        glBindVertexArray(gpu.vao);
        glBindBuffer(GL_ARRAY_BUFFER, gpu.vbo);
        glBufferData(GL_ARRAY_BUFFER, data.vertices.size() * sizeof(float),
                     data.vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        pieceModels[key] = gpu;
        // std::cout << "Successfully loaded: " << path << " (" <<
        // data.vertexCount << " vertices)" << std::endl;
      } else {
        std::cerr << "Failed to load model: " << path << std::endl;
      }
    }
  }
}

void Scene3D::resize(int width, int height) {
  if (width == currentWidth && height == currentHeight)
    return;

  currentWidth = width;
  currentHeight = height;

  if (fbo)
    glDeleteFramebuffers(1, &fbo);
  if (textureColorBuffer)
    glDeleteTextures(1, &textureColorBuffer);
  if (rbo)
    glDeleteRenderbuffers(1, &rbo);

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  glGenTextures(1, &textureColorBuffer);
  glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         textureColorBuffer, 0);

  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, rbo);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene3D::pushAnimation(coords from, coords to) {
  currentAnim.active = true;
  currentAnim.source = from;
  currentAnim.target = to;
  currentAnim.startTime = glfwGetTime();
}

bool Scene3D::isAnimationActive() const { return currentAnim.active; }

bool Scene3D::getAnimatedWorldPositionFromSource(coords source,
                                                 glm::vec3 &outWorldPos) const {
  if (!currentAnim.active)
    return false;

  if (currentAnim.source.x != source.x || currentAnim.source.y != source.y)
    return false;

  float t = static_cast<float>((glfwGetTime() - currentAnim.startTime) /
                               currentAnim.duration);
  t = glm::clamp(t, 0.0f, 1.0f);
  const float tSmooth = glm::smoothstep(0.0f, 1.0f, t);

  const float startX = static_cast<float>(currentAnim.source.x);
  const float startZ = static_cast<float>(currentAnim.source.y);
  const float targetX = static_cast<float>(currentAnim.target.x);
  const float targetZ = static_cast<float>(currentAnim.target.y);

  const float curX = startX + (targetX - startX) * tSmooth;
  const float curZ = startZ + (targetZ - startZ) * tSmooth;

  const float jumpHeight = 1.0f;
  const float curY = 0.4f + std::sin(tSmooth * glm::pi<float>()) * jumpHeight;

  outWorldPos = glm::vec3(curX, curY, curZ);
  return true;
}

GLuint
Scene3D::renderToTexture(const Camera &camera, int width, int height,
                         GameLogic &game, coords hoveredSquare,
                         coords selectedSquare,
                         const std::vector<std::array<int, 2>> &validMoves) {
  if (width <= 0 || height <= 0)
    return 0;

  resize(width, height);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, width, height);

  glUseProgram(shaderProgram);

  float aspectRatio = (float)width / (float)height;
  glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
  glm::mat4 view = camera.getViewMatrix();

  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1,
                     GL_FALSE, glm::value_ptr(projection));
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                     glm::value_ptr(view));
  glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1,
               glm::value_ptr(camera.getPosition()));

  // 1. Configurateur d'ambiance selon le tour
  glm::vec3 dirLightDir, dirLightColor, pointLightColor;
  float ambientStrength;

  if (game.getCurrentTurn() == Color::white) {
    glClearColor(0.6f, 0.75f, 0.9f, 1.0f); // Ciel bleu
    dirLightDir = glm::vec3(-0.5f, -1.0f, -0.3f);
    dirLightColor = glm::vec3(1.0f, 0.95f, 0.9f); // Lumière jaune
    ambientStrength = 0.5f;
    pointLightColor = glm::vec3(1.0f, 0.8f, 0.2f);
  } else {
    glClearColor(1.0f, 0.8f, 0.9f, 1.0f); // Ciel rose
    dirLightDir = glm::vec3(0.5f, -1.0f, 0.5f);
    dirLightColor = glm::vec3(0.7f, 0.9f, 1.0f); // Lumière bleue
    ambientStrength = 0.5f;
    pointLightColor = glm::vec3(0.9f, 0.1f, 0.3f); // Rouge/Violet
  }

  // Effacement de l'écran avec la couleur calculée
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // 2. Position de la lumière mobile flottante
  float time = (float)glfwGetTime();
  glm::vec3 pointLightPos = glm::vec3(3.5f + std::sin(time * 0.8f) * 4.0f,
                                      2.0f + std::sin(time * 2.0f) * 0.5f,
                                      3.5f + std::cos(time * 0.8f) * 4.0f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightDir"), 1,
               glm::value_ptr(dirLightDir));
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightColor"), 1,
               glm::value_ptr(dirLightColor));
  glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"),
              ambientStrength);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLightPos"), 1,
               glm::value_ptr(pointLightPos));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLightColor"), 1,
               glm::value_ptr(pointLightColor));

  for (int x = 0; x < 8; ++x) {
    for (int z = 0; z < 8; ++z) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3((float)x, -0.1f, (float)z));
      model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
      glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                         GL_FALSE, glm::value_ptr(model));
      glUniform1i(glGetUniformLocation(shaderProgram, "isPiece"), 0);

      glm::vec3 tileColor;
      bool isBlackSquare = ((x + z) % 2 == 0);

      bool isValidMove = false;
      for (const auto &move : validMoves) {
        if (move[0] == x && move[1] == z) {
          isValidMove = true;
          break;
        }
      }

      if (selectedSquare.x == x && selectedSquare.y == z) {
        tileColor = glm::vec3(0.8f, 0.8f, 0.2f);
      } else if (isValidMove) {
        tileColor = glm::vec3(0.2f, 0.8f, 0.2f);
      } else if (hoveredSquare.x == x && hoveredSquare.y == z) {
        tileColor = glm::vec3(0.5f, 0.5f, 0.5f);
      } else {
        tileColor = isBlackSquare ? glm::vec3(0.3f, 0.3f, 0.3f)
                                  : glm::vec3(0.9f, 0.9f, 0.9f);
      }

      glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                   glm::value_ptr(tileColor));
      drawMesh(cubeMesh);

      Piece const *p = game.getBoard().getPieceFromPos({x, z});
      if (p) {
        glUniform1i(glGetUniformLocation(shaderProgram, "isPiece"), 1);
        glm::vec3 drawPos((float)x, 0.4f, (float)z);

        // Animation fluide si c'est la pièce visée
        if (currentAnim.active && currentAnim.target.x == x &&
            currentAnim.target.y == z) {
          double t =
              (glfwGetTime() - currentAnim.startTime) / currentAnim.duration;
          if (t >= 1.0) {
            currentAnim.active = false;
          } else {
            // Interpolation smoothstep
            float t_smooth = glm::smoothstep(0.0f, 1.0f, (float)t);
            float startX = (float)currentAnim.source.x;
            float startZ = (float)currentAnim.source.y;

            float curX = startX + ((float)x - startX) * t_smooth;
            float curZ = startZ + ((float)z - startZ) * t_smooth;

            // Sceau parabolique (hauteur dynamique)
            float jumpHeight = 1.0f;
            float curY =
                0.4f + std::sin(t_smooth * glm::pi<float>()) * jumpHeight;

            drawPos = glm::vec3(curX, curY, curZ);
          }
        }

        glm::mat4 pieceModel = glm::mat4(1.0f);
        pieceModel = glm::translate(pieceModel, drawPos);
        pieceModel = glm::scale(pieceModel, glm::vec3(0.6f, 0.8f, 0.6f));

        // Petite rotation d'animation
        if (currentAnim.active && currentAnim.target.x == x &&
            currentAnim.target.y == z) {
          float t_smooth =
              glm::smoothstep(0.0f, 1.0f,
                              (float)((glfwGetTime() - currentAnim.startTime) /
                                      currentAnim.duration));
          pieceModel =
              glm::rotate(pieceModel, t_smooth * glm::pi<float>() * 2.0f,
                          glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1,
                           GL_FALSE, glm::value_ptr(pieceModel));

        // Si on est en mode FirstPerson sur cette pièce, on ne l'affiche pas
        // pour ne pas voir "l'intérieur" du modèle 3D.
        if (camera.getMode() == CameraMode::FirstPerson &&
            x == selectedSquare.x && z == selectedSquare.y) {
          continue;
        }

        glm::vec3 pieceColor = (p->getColor() == Color::white)
                                   ? glm::vec3(1.0f, 0.95f, 0.8f)
                                   : glm::vec3(0.1f, 0.1f, 0.1f);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                     glm::value_ptr(pieceColor));

        std::string colorStr =
            (p->getColor() == Color::white) ? "white-" : "black-";
        std::string key = colorStr + GameLogic::getPieceName(p);

        if (pieceModels.count(key)) {
          glBindVertexArray(pieceModels[key].vao);
          glDrawArrays(GL_TRIANGLES, 0, pieceModels[key].vertexCount);
        } else {
          drawMesh(cubeMesh);
        }
      }
    }
  }

  glBindVertexArray(0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  return textureColorBuffer;
}

void Scene3D::initMesh(Mesh &mesh, const std::vector<float> &vertices,
                       const std::vector<unsigned int> &indices) {
  glGenVertexArrays(1, &mesh.vao);
  glGenBuffers(1, &mesh.vbo);
  glGenBuffers(1, &mesh.ebo);

  glBindVertexArray(mesh.vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  mesh.indexCount = indices.size();
  glBindVertexArray(0);
}

void Scene3D::drawMesh(const Mesh &mesh) {
  glBindVertexArray(mesh.vao);
  glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}