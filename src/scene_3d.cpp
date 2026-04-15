#include "scene_3d.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "obj_loader.hpp"
#include <iostream>

Scene3D::Scene3D() {}

Scene3D::~Scene3D() {
    if (cubeVAO) glDeleteVertexArrays(1, &cubeVAO);
    if (cubeVBO) glDeleteBuffers(1, &cubeVBO);
    if (pawnVAO) glDeleteVertexArrays(1, &pawnVAO);
    if (pawnVBO) glDeleteBuffers(1, &pawnVBO);
    if (rookVAO) glDeleteVertexArrays(1, &rookVAO);
    if (rookVBO) glDeleteBuffers(1, &rookVBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
    if (fbo) glDeleteFramebuffers(1, &fbo);
    if (textureColorBuffer) glDeleteTextures(1, &textureColorBuffer);
    if (rbo) glDeleteRenderbuffers(1, &rbo);
}

void Scene3D::init() {
    shaderProgram = createProgram(vertexShaderSrc, fragmentShaderSrc);

    float vertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load custom models
    ModelData pawnModel = loadOBJ("../../assets/3Dmodels/white-pawn.obj");
    if(pawnModel.vertexCount > 0) {
        glGenVertexArrays(1, &pawnVAO);
        glGenBuffers(1, &pawnVBO);
        glBindVertexArray(pawnVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pawnVBO);
        glBufferData(GL_ARRAY_BUFFER, pawnModel.vertices.size() * sizeof(float), pawnModel.vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        pawnVertexCount = pawnModel.vertexCount;
        std::cout << "Pawn model loaded (" << pawnVertexCount << " vertices)." << std::endl;
    }

    ModelData rookModel = loadOBJ("../../assets/3Dmodels/white-rook.obj");
    if(rookModel.vertexCount > 0) {
        glGenVertexArrays(1, &rookVAO);
        glGenBuffers(1, &rookVBO);
        glBindVertexArray(rookVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rookVBO);
        glBufferData(GL_ARRAY_BUFFER, rookModel.vertices.size() * sizeof(float), rookModel.vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        rookVertexCount = rookModel.vertexCount;
        std::cout << "Rook model loaded (" << rookVertexCount << " vertices)." << std::endl;
    }
}

void Scene3D::resize(int width, int height) {
    if (width == currentWidth && height == currentHeight) return;
    
    currentWidth = width;
    currentHeight = height;

    if (fbo) glDeleteFramebuffers(1, &fbo);
    if (textureColorBuffer) glDeleteTextures(1, &textureColorBuffer);
    if (rbo) glDeleteRenderbuffers(1, &rbo);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene3D::pushAnimation(coords from, coords to) {
    currentAnim.active = true;
    currentAnim.source = from;
    currentAnim.target = to;
    currentAnim.startTime = glfwGetTime();
}

GLuint Scene3D::renderToTexture(const Camera& camera, int width, int height, GameLogic& game, 
                               coords hoveredSquare, coords selectedSquare, const std::vector<std::array<int, 2>>& validMoves) {
    if (width <= 0 || height <= 0) return 0;
    
    resize(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);

    glUseProgram(shaderProgram);

    float aspectRatio = (float)width / (float)height;
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
    glm::mat4 view = camera.getViewMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPosition()));

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

    glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightDir"), 1, glm::value_ptr(dirLightDir));
    glUniform3fv(glGetUniformLocation(shaderProgram, "dirLightColor"), 1, glm::value_ptr(dirLightColor));
    glUniform1f(glGetUniformLocation(shaderProgram, "ambientStrength"), ambientStrength);

    glUniform3fv(glGetUniformLocation(shaderProgram, "pointLightPos"), 1, glm::value_ptr(pointLightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "pointLightColor"), 1, glm::value_ptr(pointLightColor));

    glBindVertexArray(cubeVAO);

    for (int x = 0; x < 8; ++x) {
        for (int z = 0; z < 8; ++z) {
            glBindVertexArray(cubeVAO);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3((float)x, -0.1f, (float)z));
            model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
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
                tileColor = isBlackSquare ? glm::vec3(0.3f, 0.3f, 0.3f) : glm::vec3(0.9f, 0.9f, 0.9f);
            }

            glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(tileColor));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            Piece const* p = game.getBoard().getPieceFromPos({x, z});
            if (p) {
                glUniform1i(glGetUniformLocation(shaderProgram, "isPiece"), 1);
                glm::vec3 drawPos((float)x, 0.4f, (float)z);
                
                // Animation fluide si c'est la pièce visée
                if (currentAnim.active && currentAnim.target.x == x && currentAnim.target.y == z) {
                    double t = (glfwGetTime() - currentAnim.startTime) / currentAnim.duration;
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
                        float curY = 0.4f + std::sin(t_smooth * glm::pi<float>()) * jumpHeight;
                        
                        drawPos = glm::vec3(curX, curY, curZ);
                    }
                }

                glm::mat4 pieceModel = glm::mat4(1.0f);
                pieceModel = glm::translate(pieceModel, drawPos);
                pieceModel = glm::scale(pieceModel, glm::vec3(0.6f, 0.8f, 0.6f));
                
                // Petite rotation d'animation
                if (currentAnim.active && currentAnim.target.x == x && currentAnim.target.y == z) {
                     float t_smooth = glm::smoothstep(0.0f, 1.0f, (float)((glfwGetTime() - currentAnim.startTime) / currentAnim.duration));
                     pieceModel = glm::rotate(pieceModel, t_smooth * glm::pi<float>() * 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
                }

                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pieceModel));

                glm::vec3 pieceColor = (p->getColor() == Color::white) ? glm::vec3(1.0f, 0.95f, 0.8f) : glm::vec3(0.1f, 0.1f, 0.1f);
                glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(pieceColor));
                
                std::string pieceName = GameLogic::getPieceName(p);
                if (pieceName == "pawn" && pawnVAO != 0) {
                    glBindVertexArray(pawnVAO);
                    glDrawArrays(GL_TRIANGLES, 0, pawnVertexCount);
                } else if (pieceName == "rook" && rookVAO != 0) {
                    glBindVertexArray(rookVAO);
                    glDrawArrays(GL_TRIANGLES, 0, rookVertexCount);
                } else {
                    glBindVertexArray(cubeVAO);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return textureColorBuffer;
}
