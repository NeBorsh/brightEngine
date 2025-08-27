#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : programID(0), compiled(false) {
    
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);

    if (vertexCode.empty() || fragmentCode.empty()) {
        std::cerr << "Failed to read shader files!" << std::endl;
        return;
    }

    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    if (!vertexShader || !fragmentShader) return;

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
        glDeleteProgram(programID);
        programID = 0;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    compiled = true;
}

Shader::~Shader() {
    if (programID) {
        glDeleteProgram(programID);
    }
}

std::string Shader::readShaderFile(const std::string& filePath) {
    std::ifstream file;
    std::stringstream buffer;

    try {
        file.open(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filePath);
        }
        buffer << file.rdbuf();
        file.close();
    } catch (const std::exception& e) {
        std::cerr << "Error reading shader file " << filePath << ": " << e.what() << std::endl;
        return "";
    }

    return buffer.str();
}

GLuint Shader::compileShader(const std::string& source, GLenum type) {
    const char* src = source.c_str();
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed (" 
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                  << "):\n" << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::use() const {
    if (compiled && programID) {
        glUseProgram(programID);
    }
}