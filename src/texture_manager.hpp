#pragma once

#include <glad/glad.h>
#include <map>
#include <string>


class TextureManager {
public:
  TextureManager() = default;
  ~TextureManager() = default;

  void loadAllPieceTextures();
  GLuint getTexture(const std::string &key) const;

private:
  std::map<std::string, GLuint> textures;

  GLuint loadTexture(const char *filepath);
};
