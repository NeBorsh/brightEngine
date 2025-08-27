#pragma once

#include <string>
#include <vector>

#include "mesh.h"
#include "shader.h"
#include "light.h"

#include <tiny_obj_loader.h>

class Model {
public:
    Model(const std::string& path);
    void Draw(const Shader& shader, const std::vector<Light>& lights, const glm::vec3& viewPos, const Material* overrideMaterial = nullptr);

    const std::vector<Mesh>& getMeshes() const {
        return meshes;
    }

private:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;

    void loadModel(const std::string& path);
    void processNode(
        const tinyobj::attrib_t& attrib,
        const std::vector<tinyobj::shape_t>& shapes,
        const std::vector<tinyobj::material_t>& materials);
    Mesh processMesh(
        const tinyobj::attrib_t& attrib,
        const tinyobj::mesh_t& mesh,
        const std::vector<tinyobj::material_t>& materials);
    std::vector<Texture> loadTextures(const tinyobj::material_t& mat);
};