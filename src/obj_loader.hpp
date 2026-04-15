#pragma once
#include <vector>
#include <string>

struct ModelData {
    std::vector<float> vertices; // interleaved x, y, z, nx, ny, nz
    int vertexCount;
};

ModelData loadOBJ(const std::string& path);
