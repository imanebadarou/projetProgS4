#pragma once

#include "camera.hpp"
#include "game_logic.hpp"
#include "gl_utils.hpp"
#include "utils.hpp"
#include <vector>
#include <array>

class Scene3D {
public:
    Scene3D();
    ~Scene3D();

    void init();
    void resize(int width, int height);
    GLuint renderToTexture(const Camera& camera, int width, int height, GameLogic& game, 
                           coords hoveredSquare, coords selectedSquare, const std::vector<std::array<int, 2>>& validMoves);

private:
    GLuint shaderProgram{0};
    GLuint cubeVAO{0}, cubeVBO{0};
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

        uniform vec3 lightPos;
        uniform vec3 viewPos;
        uniform vec3 lightColor;

        void main() {
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * lightColor;
            
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
            float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);  
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
            vec3 specular = specularStrength * spec * lightColor;  
                
            vec3 result = (ambient + diffuse + specular) * Color;
            FragColor = vec4(result, 1.0);
        }
    )";
};
