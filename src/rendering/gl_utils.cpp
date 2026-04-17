#include "rendering/gl_utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::string readTextFile(const std::string &path) {
  std::ifstream file(path, std::ios::in);
  if (!file.is_open()) {
    std::cerr << "Impossible d'ouvrir le fichier: " << path << std::endl;
    return {};
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint compileShader(GLenum type, const std::string &source) {
  GLuint shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<char> infoLog(logSize);
    glGetShaderInfoLog(shader, logSize, nullptr, infoLog.data());
    std::cerr << "Erreur de compilation du shader:\n"
              << infoLog.data() << std::endl;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

GLuint createProgram(const std::string &vertexSource,
                     const std::string &fragmentSource) {
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

  if (vertexShader == 0 || fragmentShader == 0) {
    if (vertexShader) {
      glDeleteShader(vertexShader);
    }
    if (fragmentShader) {
      glDeleteShader(fragmentShader);
    }
    return 0;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLint logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    std::vector<char> infoLog(logSize);
    glGetProgramInfoLog(program, logSize, nullptr, infoLog.data());
    std::cerr << "Erreur de linkage du programme:\n"
              << infoLog.data() << std::endl;
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
    return 0;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

GLuint createProgramFromFiles(const std::string &vertexPath,
                              const std::string &fragmentPath) {
  const std::string vertexSource = readTextFile(vertexPath);
  const std::string fragmentSource = readTextFile(fragmentPath);

  if (vertexSource.empty() || fragmentSource.empty()) {
    std::cerr << "Shaders non charges depuis: " << vertexPath << " et "
              << fragmentPath << std::endl;
    return 0;
  }

  return createProgram(vertexSource, fragmentSource);
}
