#pragma once

#include <string>
#include <glad/glad.h>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    void use() const;
    bool isCompiled() const { return compiled; }

    GLuint getID() const { return programID; }

private:
    GLuint programID;
    bool compiled;

    std::string readShaderFile(const std::string& filePath);
    GLuint compileShader(const std::string& source, GLenum type);
};