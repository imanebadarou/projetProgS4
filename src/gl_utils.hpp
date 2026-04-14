#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

GLuint compileShader(GLenum type, const std::string& source);
GLuint createProgram(const std::string& vertexSource, const std::string& fragmentSource);
