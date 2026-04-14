#include "scene_3d.hpp"
#include <glm/gtc/type_ptr.hpp>

Scene3D::Scene3D() {}

Scene3D::~Scene3D() {
    if (cubeVAO) glDeleteVertexArrays(1, &cubeVAO);
    if (cubeVBO) glDeleteBuffers(1, &cubeVBO);
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

GLuint Scene3D::renderToTexture(const Camera& camera, int width, int height, GameLogic& game, 
                               coords hoveredSquare, coords selectedSquare, const std::vector<std::array<int, 2>>& validMoves) {
    if (width <= 0 || height <= 0) return 0;
    
    resize(width, height);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, width, height);
    
    // Clear the FBO with a specific background color
    glClearColor(0.2f, 0.2f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glUseProgram(shaderProgram);

    float aspectRatio = (float)width / (float)height;
    glm::mat4 projection = camera.getProjectionMatrix(aspectRatio);
    glm::mat4 view = camera.getViewMatrix();

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

    // Lumière adaptative en fonction du tour
    glm::vec3 lightColor = (game.getCurrentTurn() == Color::white) ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.3f, 0.3f, 0.7f);
    glm::vec3 lightPos = glm::vec3(3.5f, 5.0f, 3.5f);
    
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPosition()));

    glBindVertexArray(cubeVAO);

    for (int x = 0; x < 8; ++x) {
        for (int z = 0; z < 8; ++z) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3((float)x, -0.1f, (float)z));
            model = glm::scale(model, glm::vec3(1.0f, 0.2f, 1.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

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
                glm::mat4 pieceModel = glm::mat4(1.0f);
                pieceModel = glm::translate(pieceModel, glm::vec3((float)x, 0.4f, (float)z));
                pieceModel = glm::scale(pieceModel, glm::vec3(0.6f, 0.8f, 0.6f));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(pieceModel));

                glm::vec3 pieceColor = (p->getColor() == Color::white) ? glm::vec3(1.0f, 0.95f, 0.8f) : glm::vec3(0.1f, 0.1f, 0.1f);
                glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(pieceColor));
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
    }

    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return textureColorBuffer;
}
