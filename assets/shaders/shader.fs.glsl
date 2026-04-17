#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform float ambientStrength;

uniform vec3 pointLightPos;
uniform vec3 pointLightColor;

uniform vec3 viewPos;
uniform bool isPiece;

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = ambientStrength * dirLightColor;
    vec3 lightDir = normalize(-dirLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * dirLightColor;

    float spec = 0.0;
    if (isPiece) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    }
    vec3 specular = 0.5 * spec * dirLightColor;
    vec3 resultDir = ambient + diffuse + specular;

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

// eclairage de contour
    vec3 rim = vec3(0.0);
    if (isPiece) {
        float rimFactor = 1.0 - max(dot(viewDir, norm), 0.0);
        rimFactor = smoothstep(0.6, 1.0, rimFactor);
        rim = rimFactor * pointLightColor * attenuation;
    }

    vec3 finalColor = (resultDir + resultPoint + rim) * Color;
    FragColor = vec4(finalColor, 1.0);
}
