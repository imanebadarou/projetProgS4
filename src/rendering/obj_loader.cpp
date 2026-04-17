#include "rendering/obj_loader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::optional<ModelData> loadOBJ(const std::string &path) {
  ModelData result;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec3> temp_normals;

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Could not open OBJ file: " << path << std::endl;
    return std::nullopt;
  }

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
      glm::vec3 v;
      iss >> v.x >> v.y >> v.z;
      temp_vertices.push_back(v);
    } else if (type == "vn") {
      glm::vec3 vn;
      iss >> vn.x >> vn.y >> vn.z;
      temp_normals.push_back(vn);
    } else if (type == "f") {
      std::string vertexData;
      std::vector<std::string> faceVertices;
      while (iss >> vertexData) {
        faceVertices.push_back(vertexData);
      }

      // Triangulation si necessaire (triangulation en eventail)
      for (size_t i = 1; i < faceVertices.size() - 1; ++i) {
        std::string tri[3] = {faceVertices[0], faceVertices[i],
                              faceVertices[i + 1]};
        for (int j = 0; j < 3; ++j) {
          std::stringstream ss(tri[j]);
          std::string vIdxStr, vtIdxStr, vnIdxStr;
          std::getline(ss, vIdxStr, '/');
          std::getline(ss, vtIdxStr, '/');
          std::getline(ss, vnIdxStr, '/');

          int vIdx = 0;
          try {
            vIdx = std::stoi(vIdxStr) - 1;
          } catch (const std::exception &) {
            std::cerr << "Invalid OBJ vertex index in file: " << path
                      << std::endl;
            return std::nullopt;
          }
          int vnIdx = -1;
          if (!vnIdxStr.empty()) {
            try {
              vnIdx = std::stoi(vnIdxStr) - 1;
            } catch (const std::exception &) {
              std::cerr << "Invalid OBJ normal index in file: " << path
                        << std::endl;
              return std::nullopt;
            }
          }

          glm::vec3 pos = temp_vertices[vIdx];
          glm::vec3 norm =
              (vnIdx != -1) ? temp_normals[vnIdx] : glm::vec3(0, 1, 0);

          result.vertices.push_back(pos.x);
          result.vertices.push_back(pos.y);
          result.vertices.push_back(pos.z);
          result.vertices.push_back(norm.x);
          result.vertices.push_back(norm.y);
          result.vertices.push_back(norm.z);
          result.vertexCount++;
        }
      }
    }
  }
  return result;
}
