#pragma once

#include <glad/glad.h>
#include <string>

std::string readTextFile(const std::string &path);
GLuint compileShader(GLenum type, const std::string &source);
GLuint createProgram(const std::string &vertexSource,
                     const std::string &fragmentSource);
GLuint createProgramFromFiles(const std::string &vertexPath,
                              const std::string &fragmentPath);
