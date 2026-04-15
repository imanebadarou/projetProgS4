#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

struct ModelData {
    std::vector<float> vertices; // Interleaved: pos.x, pos.y, pos.z, norm.x, norm.y, norm.z
    int vertexCount{0};
};

ModelData loadOBJ(const std::string& path);
