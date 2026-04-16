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
  static GLuint loadTexture(const char *filepath);
  static GLuint loadCubemap(const std::vector<std::string> &faces);
  static GLuint loadCubemapFromSingleImage(const std::string &path);

private:
  std::map<std::string, GLuint> textures;
};
