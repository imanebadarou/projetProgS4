#include "texture_manager.hpp"
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint TextureManager::loadTexture(const char *filepath) {
  int width, height, channels;
  unsigned char *data = stbi_load(filepath, &width, &height, &channels, 4);
  if (!data) {
    std::cerr << "Failed to load texture: " << filepath << std::endl;
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
  return texture;
}

void TextureManager::loadAllPieceTextures() {
  std::vector<std::string> colors = {"white", "black"};
  std::vector<std::string> names = {"pawn",   "rook",  "knight",
                                    "bishop", "queen", "king"};

  for (const auto &color : colors) {
    for (const auto &name : names) {
      std::string key = color + "-" + name;
      std::string path = "../../assets/pieces/" + key + ".png";
      textures[key] = loadTexture(path.c_str());
    }
  }
}

GLuint TextureManager::getTexture(const std::string &key) const {
  auto it = textures.find(key);
  if (it != textures.end()) {
    return it->second;
  }
  return 0;
}
