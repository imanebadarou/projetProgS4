#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 instanceModel;
layout (location = 6) in vec4 instanceColor;

uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;
out vec3 ModelPos; // Utilisé pour le calcul du marbre

void main() {
    ModelPos = aPos;
    FragPos = vec3(instanceModel * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(instanceModel))) * aNormal;
    Color = instanceColor.rgb;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
