#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec3 ModelPos;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform float ambientStrength;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform vec3 viewPos;

// Bruit pseudo-aleatoire
float hash(float n) { return fract(sin(n) * 43758.5453123); }
float noise(vec3 x) {
    vec3 p = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0 + 113.0 * p.z;
    return mix(mix(mix(hash(n + 0.0), hash(n + 1.0), f.x),
                   mix(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
               mix(mix(hash(n + 113.0), hash(n + 114.0), f.x),
                   mix(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

// Mouvement brownien fractal
float fbm(vec3 p) {
    float f = 0.0;
    f += 0.5000 * noise(p); p = p * 2.02;
    f += 0.2500 * noise(p); p = p * 2.03;
    f += 0.1250 * noise(p);
    return f;
}

void main() {
    // 1. Calcul de l'effet Marbré
    float noiseVal = fbm(ModelPos * 4.0);
    float marble = sin(ModelPos.y * 10.0 + noiseVal * 15.0);
    marble = marble * 0.5 + 0.5;
    marble = pow(marble, 0.2); // Veines plus marquées

    vec3 marbleColor = mix(Color * 0.7, Color * 1.3, marble);

    // 2. Éclairage classique
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Lumiere directionnelle
    vec3 ambient = ambientStrength * dirLightColor;
    vec3 lightDir = normalize(-dirLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * dirLightColor;
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    vec3 specular = 0.8 * spec * dirLightColor;

    // Lumiere ponctuelle
    vec3 lightDirPt = normalize(pointLightPos - FragPos);
    float diffPt = max(dot(norm, lightDirPt), 0.0);
    float dist = length(pointLightPos - FragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * dist + 0.032 * (dist * dist));
    vec3 diffusePt = diffPt * pointLightColor * attenuation;
    
    vec3 halfwayDirPt = normalize(lightDirPt + viewDir);
    float specPt = pow(max(dot(norm, halfwayDirPt), 0.0), 128.0);
    vec3 specularPt = 1.0 * specPt * pointLightColor * attenuation;

    // Eclairage de contour (effet de silhouette)
    float rimFactor = 1.0 - max(dot(viewDir, norm), 0.0);
    rimFactor = smoothstep(0.5, 1.0, rimFactor);
    vec3 rim = rimFactor * pointLightColor * attenuation * 0.5;

    vec3 result = (ambient + diffuse + specular + diffusePt + specularPt + rim) * marbleColor;
    FragColor = vec4(result, 1.0);
}
