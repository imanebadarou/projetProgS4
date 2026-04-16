#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D skybox;

const vec2 invAtan = vec2(0.1591, 0.3183); // 1/2pi, 1/pi

vec2 SampleEquirectangular(vec3 v)
{
    // On inverse le signe de v.y pour remettre l'image à l'endroit
    vec2 uv = vec2(atan(v.z, v.x), asin(-v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{    
    vec2 uv = SampleEquirectangular(normalize(TexCoords));
    FragColor = texture(skybox, uv);
}
