#pragma once
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <vector>

struct ModelData {
  std::vector<float>
      vertices; // Entrelace : pos.x, pos.y, pos.z, norm.x, norm.y, norm.z
  int vertexCount{0};
};

std::optional<ModelData> loadOBJ(const std::string &path);
