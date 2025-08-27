#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <cstddef>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    bool operator==(const Vertex& other) const {
        return Position == other.Position &&
               Normal == other.Normal &&
               TexCoords == other.TexCoords;
    }
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Vec3Hash {
    std::size_t operator()(const glm::vec3& v) const {
        std::size_t h1 = std::hash<float>()(v.x);
        std::size_t h2 = std::hash<float>()(v.y);
        std::size_t h3 = std::hash<float>()(v.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct Vec2Hash {
    std::size_t operator()(const glm::vec2& v) const {
        std::size_t h1 = std::hash<float>()(v.x);
        std::size_t h2 = std::hash<float>()(v.y);
        return h1 ^ (h2 << 1);
    }
};

struct VertexHash {
    std::size_t operator()(const Vertex& vertex) const {
        Vec3Hash vec3Hash;
        Vec2Hash vec2Hash;
        std::size_t h1 = vec3Hash(vertex.Position);
        std::size_t h2 = vec3Hash(vertex.Normal);
        std::size_t h3 = vec2Hash(vertex.TexCoords);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);
    void Draw();

    std::vector<Texture> textures;
    Material material;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;

    void setupMesh();
};