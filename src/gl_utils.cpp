#include "gl_utils.hpp"
#include <vector>

GLuint compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
        std::vector<char> infoLog(logSize);
        glGetShaderInfoLog(shader, logSize, nullptr, infoLog.data());
        std::cerr << "Erreur de compilation du shader:\n" << infoLog.data() << std::endl;
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

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
        std::cerr << "Erreur de linkage du programme:\n" << infoLog.data() << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
