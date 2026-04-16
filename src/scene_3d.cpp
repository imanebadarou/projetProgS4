#include "scene_3d.hpp"
#include "obj_loader.hpp"
#include "texture_manager.hpp"
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

  if (instanceVbo)
    glDeleteBuffers(1, &instanceVbo);

  if (skyboxMesh.vao)
    glDeleteVertexArrays(1, &skyboxMesh.vao);
  if (skyboxMesh.vbo)
    glDeleteBuffers(1, &skyboxMesh.vbo);
  if (skyboxTexture)
    glDeleteTextures(1, &skyboxTexture);

  if (shaderProgram)
    glDeleteProgram(shaderProgram);
  if (skyboxShader)
    glDeleteProgram(skyboxShader);
  if (fbo)
    glDeleteFramebuffers(1, &fbo);
  if (textureColorBuffer)
    glDeleteTextures(1, &textureColorBuffer);
  if (rbo)
    glDeleteRenderbuffers(1, &rbo);
}

void Scene3D::init() {
  shaderProgram = createProgramFromFiles(vertexShaderPath, fragmentShaderPath);
  if (shaderProgram == 0) {
    std::cerr << "Impossible de creer le shader program 3D." << std::endl;
    return;
  }

  uniforms.model = glGetUniformLocation(shaderProgram, "model");
  uniforms.view = glGetUniformLocation(shaderProgram, "view");
  uniforms.projection = glGetUniformLocation(shaderProgram, "projection");
  uniforms.color = glGetUniformLocation(shaderProgram, "color");
  uniforms.viewPos = glGetUniformLocation(shaderProgram, "viewPos");
  uniforms.dirLightDir = glGetUniformLocation(shaderProgram, "dirLightDir");
  uniforms.dirLightColor = glGetUniformLocation(shaderProgram, "dirLightColor");
  uniforms.ambientStrength =
      glGetUniformLocation(shaderProgram, "ambientStrength");
  uniforms.pointLightPos = glGetUniformLocation(shaderProgram, "pointLightPos");
  uniforms.pointLightColor =
      glGetUniformLocation(shaderProgram, "pointLightColor");
  uniforms.isPiece = glGetUniformLocation(shaderProgram, "isPiece");

  glGenBuffers(1, &instanceVbo);
  uniforms.isPiece = glGetUniformLocation(shaderProgram, "isPiece");

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
  setupInstancedAttributes(cubeMesh.vao);

  const std::vector<float> skyboxVertices = {
    -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
  };

  glGenVertexArrays(1, &skyboxMesh.vao);
  glGenBuffers(1, &skyboxMesh.vbo);
  glBindVertexArray(skyboxMesh.vao);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxMesh.vbo);
  glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(0);

  skyboxShader = createProgramFromFiles(skyboxVertexShaderPath, skyboxFragmentShaderPath);
  skyboxTexture = TextureManager::loadTexture("../../assets/skybox/skybox.png");

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

        setupInstancedAttributes(gpu.vao);

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
  if (width <= 0 || height <= 0 || shaderProgram == 0)
    return 0;

  resize(width, height);

  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glViewport(0, 0, width, height);

  glUseProgram(shaderProgram);

  float aspectRatio = (float)width / (float)height;
  glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
  glm::mat4 view = camera.getViewMatrix();

  glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE,
                     glm::value_ptr(projection));
  glUniformMatrix4fv(uniforms.view, 1, GL_FALSE, glm::value_ptr(view));
  glUniform3fv(uniforms.viewPos, 1, glm::value_ptr(camera.getPosition()));

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

  // Rendu de la Skybox
  glDepthFunc(GL_LEQUAL);
  glUseProgram(skyboxShader);
  glUniform1i(glGetUniformLocation(skyboxShader, "skybox"), 0);
  glm::mat4 skyboxView = glm::mat4(glm::mat3(view));
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  glBindVertexArray(skyboxMesh.vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, skyboxTexture);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
  glDepthFunc(GL_LESS);

  glUseProgram(shaderProgram);

  // Position de la lumière mobile flottante
  float time = (float)glfwGetTime();
  glm::vec3 pointLightPos = glm::vec3(3.5f + std::sin(time * 0.8f) * 4.0f,
                                      2.0f + std::sin(time * 2.0f) * 0.5f,
                                      3.5f + std::cos(time * 0.8f) * 4.0f);

  glUniform3fv(uniforms.dirLightDir, 1, glm::value_ptr(dirLightDir));
  glUniform3fv(uniforms.dirLightColor, 1, glm::value_ptr(dirLightColor));
  glUniform1f(uniforms.ambientStrength, ambientStrength);

  glUniform3fv(uniforms.pointLightPos, 1, glm::value_ptr(pointLightPos));
  glUniform3fv(uniforms.pointLightColor, 1, glm::value_ptr(pointLightColor));

  bool validMoveMask[8][8] = {};
  for (const auto &move : validMoves) {
    if (move[0] >= 0 && move[0] < 8 && move[1] >= 0 && move[1] < 8) {
      validMoveMask[move[0]][move[1]] = true;
    }
  }

  std::vector<InstanceData> boardInstances;
  boardInstances.reserve(65); // 64 cases + 1 bordure
  std::map<std::string, std::vector<InstanceData>> pieceBatches;

  // Ajout d'une bordure sous l'échiquier
  InstanceData borderInstance;
  borderInstance.model = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.15f, 3.5f));
  borderInstance.model = glm::scale(borderInstance.model, glm::vec3(8.5f, 0.25f, 8.5f));
  borderInstance.color = glm::vec4(0.2f, 0.1f, 0.05f, 1.0f); // Brun bois sombre
  boardInstances.push_back(borderInstance);

  for (int x = 0; x < 8; ++x) {
    for (int z = 0; z < 8; ++z) {
      bool isBlackSquare = ((x + z) % 2 == 0);
      glm::vec3 tileColor;

      if (selectedSquare.x == x && selectedSquare.y == z) {
        tileColor = glm::vec3(0.8f, 0.8f, 0.2f);
      } else if (validMoveMask[x][z]) {
        tileColor = glm::vec3(0.2f, 0.8f, 0.2f);
      } else if (hoveredSquare.x == x && hoveredSquare.y == z) {
        tileColor = glm::vec3(0.5f, 0.5f, 0.5f);
      } else {
        tileColor = isBlackSquare ? glm::vec3(0.3f, 0.3f, 0.3f)
                                  : glm::vec3(0.9f, 0.9f, 0.9f);
      }

      InstanceData tileInstance;
      tileInstance.model =
          glm::translate(glm::mat4(1.0f), glm::vec3((float)x, -0.1f, (float)z));
      tileInstance.model =
          glm::scale(tileInstance.model, glm::vec3(1.0f, 0.2f, 1.0f));
      tileInstance.color = glm::vec4(tileColor, 1.0f);
      boardInstances.push_back(tileInstance);

      Piece const *p = game.getBoard().getPieceFromPos({x, z});
      if (!p) {
        continue;
      }

      glm::vec3 drawPos((float)x, 0.4f, (float)z);
      bool isAnimatingThisPiece = false;
      if (currentAnim.active && currentAnim.target.x == x &&
          currentAnim.target.y == z) {
        float t = static_cast<float>((glfwGetTime() - currentAnim.startTime) / currentAnim.duration);
        if (t >= 1.0f) {
          // Animation terminée, mais on ne désactive pas ici car on est en plein milieu du rendu des instances
        } else {
          isAnimatingThisPiece = true;
          float tSmooth = glm::smoothstep(0.0f, 1.0f, t);
          float startX = static_cast<float>(currentAnim.source.x);
          float startZ = static_cast<float>(currentAnim.source.y);
          float curX = startX + ((float)x - startX) * tSmooth;
          float curZ = startZ + ((float)z - startZ) * tSmooth;
          float jumpHeight = 1.0f;
          float curY = 0.4f + std::sin(tSmooth * glm::pi<float>()) * jumpHeight;
          drawPos = glm::vec3(curX, curY, curZ);
        }
      }

      // On cache la pièce si la caméra est "dedans" (mode pièce)
      if (camera.getMode() == CameraMode::FirstPerson) {
          glm::vec3 camPos = camera.getPosition();
          float distSq = (drawPos.x - camPos.x) * (drawPos.x - camPos.x) + 
                         (drawPos.z - camPos.z) * (drawPos.z - camPos.z);
          if (distSq < 0.1f) {
              continue;
          }
      }

      glm::mat4 pieceModel = glm::translate(glm::mat4(1.0f), drawPos);
      pieceModel = glm::scale(pieceModel, glm::vec3(0.6f, 0.8f, 0.6f));

      if (isAnimatingThisPiece) {
        float t = static_cast<float>((glfwGetTime() - currentAnim.startTime) / currentAnim.duration);
        float tSmooth = glm::smoothstep(0.0f, 1.0f, t);
        pieceModel = glm::rotate(pieceModel, tSmooth * glm::pi<float>() * 2.0f,
                                 glm::vec3(0.0f, 1.0f, 0.0f));
      }

      glm::vec3 pieceColor = (p->getColor() == Color::white)
                                 ? glm::vec3(1.0f, 0.95f, 0.8f)
                                 : glm::vec3(0.1f, 0.1f, 0.1f);
      std::string colorStr =
          (p->getColor() == Color::white) ? "white-" : "black-";
      std::string key = colorStr + GameLogic::getPieceName(p);

      InstanceData pieceInstance;
      pieceInstance.model = pieceModel;
      pieceInstance.color = glm::vec4(pieceColor, 1.0f);
      pieceBatches[key].push_back(pieceInstance);
    }
  }

  // Désactivation propre de l'animation si elle est finie
  if (currentAnim.active) {
      if ((glfwGetTime() - currentAnim.startTime) / currentAnim.duration >= 1.0) {
          currentAnim.active = false;
      }
  }

  uploadInstances(boardInstances);
  glUniform1i(uniforms.isPiece, 0);
  drawInstancedMesh(cubeMesh, static_cast<GLsizei>(boardInstances.size()));

  glUniform1i(uniforms.isPiece, 1);
  for (auto &batch : pieceBatches) {
    auto modelIt = pieceModels.find(batch.first);
    if (modelIt == pieceModels.end() || batch.second.empty()) {
      continue;
    }

    uploadInstances(batch.second);
    drawInstancedModel(modelIt->second,
                       static_cast<GLsizei>(batch.second.size()));
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

void Scene3D::setupInstancedAttributes(GLuint vao) {
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);

  constexpr GLsizei stride = sizeof(InstanceData);
  for (int i = 0; i < 4; ++i) {
    const GLuint location = 2 + static_cast<GLuint>(i);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride,
                          reinterpret_cast<void *>(sizeof(glm::vec4) * i));
    glVertexAttribDivisor(location, 1);
  }

  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride,
                        reinterpret_cast<void *>(sizeof(glm::mat4)));
  glVertexAttribDivisor(6, 1);

  glBindVertexArray(0);
}

void Scene3D::uploadInstances(const std::vector<InstanceData> &instances) {
  glBindBuffer(GL_ARRAY_BUFFER, instanceVbo);
  glBufferData(GL_ARRAY_BUFFER,
               static_cast<GLsizeiptr>(instances.size() * sizeof(InstanceData)),
               instances.data(), GL_STREAM_DRAW);
}

void Scene3D::drawInstancedMesh(const Mesh &mesh, GLsizei instanceCount) {
  if (instanceCount <= 0) {
    return;
  }

  glBindVertexArray(mesh.vao);
  glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0,
                          instanceCount);
}

void Scene3D::drawInstancedModel(const GpuModel &model, GLsizei instanceCount) {
  if (instanceCount <= 0) {
    return;
  }

  glBindVertexArray(model.vao);
  glDrawArraysInstanced(GL_TRIANGLES, 0, model.vertexCount, instanceCount);
}

void Scene3D::drawMesh(const Mesh &mesh) {
  glBindVertexArray(mesh.vao);
  glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}