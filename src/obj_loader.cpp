#include "obj_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

ModelData loadOBJ(const std::string& path) {
    ModelData result;
    result.vertexCount = 0;
    
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Impossible to open the file: " << path << std::endl;
        return result;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;
        if (type == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (type == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (type == "f") {
            std::vector<std::string> vStrs;
            std::string vertexStr;
            while (iss >> vertexStr) {
                vStrs.push_back(vertexStr);
            }
            if (vStrs.size() < 3) continue;
            
            // Triangulate
            for (size_t i = 1; i < vStrs.size() - 1; ++i) {
                std::string tri[3] = {vStrs[0], vStrs[i], vStrs[i+1]};
                for(int j=0; j<3; ++j) {
                    std::istringstream viss(tri[j]);
                    std::string v, vt, vn;
                    std::getline(viss, v, '/');
                    std::getline(viss, vt, '/');
                    std::getline(viss, vn, '/');
                    
                    int vIdx = v.empty() ? 0 : std::stoi(v) - 1;
                    int vnIdx = vn.empty() ? -1 : std::stoi(vn) - 1;
                    
                    glm::vec3 vertex = (vIdx >= 0 && vIdx < temp_vertices.size()) ? temp_vertices[vIdx] : glm::vec3(0.0f);
                    glm::vec3 normal = (vnIdx >= 0 && vnIdx < temp_normals.size()) ? temp_normals[vnIdx] : glm::vec3(0.0f, 1.0f, 0.0f);
                    
                    result.vertices.push_back(vertex.x);
                    result.vertices.push_back(vertex.y);
                    result.vertices.push_back(vertex.z);
                    result.vertices.push_back(normal.x);
                    result.vertices.push_back(normal.y);
                    result.vertices.push_back(normal.z);
                    result.vertexCount++;
                }
            }
        }
    }
    return result;
}
