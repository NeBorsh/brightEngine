#include "model.h"
#include "texture.h"

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::loadModel(const std::string& path) {
    size_t found = path.find_last_of("/\\");
    directory = path.substr(0, found);

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), directory.c_str())) {
        return;
    }

    processNode(attrib, shapes, materials);
}

void Model::processNode(
    const tinyobj::attrib_t& attrib,
    const std::vector<tinyobj::shape_t>& shapes,
    const std::vector<tinyobj::material_t>& materials)
{
    for (const auto& shape : shapes) {
        Mesh mesh = processMesh(attrib, shape.mesh, materials);
        meshes.push_back(mesh);
    }
}

Mesh Model::processMesh(
    const tinyobj::attrib_t& attrib,
    const tinyobj::mesh_t& mesh,
    const std::vector<tinyobj::material_t>& materials)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh.indices.size(); ++i) {
        tinyobj::index_t idx = mesh.indices[i];
        Vertex vertex;

        vertex.Position = {
            attrib.vertices[3 * idx.vertex_index + 0],
            attrib.vertices[3 * idx.vertex_index + 1],
            attrib.vertices[3 * idx.vertex_index + 2]
        };

        if (idx.normal_index >= 0) {
            vertex.Normal = {
                attrib.normals[3 * idx.normal_index + 0],
                attrib.normals[3 * idx.normal_index + 1],
                attrib.normals[3 * idx.normal_index + 2]
            };
        }

        if (idx.texcoord_index >= 0) {
            vertex.TexCoords = {
                attrib.texcoords[2 * idx.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * idx.texcoord_index + 1]
            };
        }

        vertices.push_back(vertex);
        indices.push_back(static_cast<unsigned int>(i));
    }

    Material material;
    std::vector<Texture> textures;

    int materialID = -1;
    if (!mesh.material_ids.empty()) {
        materialID = mesh.material_ids[0];
    }

    if (materialID >= 0 && materialID < static_cast<int>(materials.size())) {
        auto& mat = materials[materialID];
        material.ambient  = glm::vec3(mat.ambient[0],  mat.ambient[1],  mat.ambient[2]);
        material.diffuse  = glm::vec3(mat.diffuse[0],  mat.diffuse[1],  mat.diffuse[2]);
        material.specular = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
        material.shininess = mat.shininess;
        textures = loadTextures(mat);
    } else {
        material.ambient  = glm::vec3(0.1f);
        material.diffuse  = glm::vec3(0.8f);
        material.specular = glm::vec3(0.2f);
        material.shininess = 32.0f;
    }

    return Mesh(vertices, indices, textures, material);
}

std::vector<Texture> Model::loadTextures(const tinyobj::material_t& mat) {
    std::vector<Texture> textures;

    auto loadTex = [&](const std::string& texName, const std::string& type) {
        if (texName.empty()) return;
        std::string texPath = directory + "/" + texName;
        for (const auto& t : textures_loaded) {
            if (t.path == texPath) {
                textures.push_back(t);
                return;
            }
        }
        unsigned int id = loadTexture(texPath);
        if (id != 0) {
            Texture texture;
            texture.id = id;
            texture.type = type;
            texture.path = texPath;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    };

    loadTex(mat.diffuse_texname, "texture_diffuse");
    loadTex(mat.specular_texname, "texture_specular");

    return textures;
}

void Model::Draw(const Shader& shader, const std::vector<Light>& lights, const glm::vec3& viewPos, const Material* overrideMaterial) {
    shader.use();

    glUniform3f(glGetUniformLocation(shader.getID(), "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform1i(glGetUniformLocation(shader.getID(), "numLights"), static_cast<int>(lights.size()));

    for (size_t i = 0; i < lights.size() && i < 5; ++i) {
        const Light& light = lights[i];
        std::string base = "lights[" + std::to_string(i) + "]";

        glUniform1i(glGetUniformLocation(shader.getID(), (base + ".type").c_str()), static_cast<int>(light.type));
        glUniform3f(glGetUniformLocation(shader.getID(), (base + ".position").c_str()), light.position.x, light.position.y, light.position.z);
        glUniform3f(glGetUniformLocation(shader.getID(), (base + ".direction").c_str()), light.direction.x, light.direction.y, light.direction.z);
        glUniform3f(glGetUniformLocation(shader.getID(), (base + ".ambient").c_str()), light.ambient.x, light.ambient.y, light.ambient.z);
        glUniform3f(glGetUniformLocation(shader.getID(), (base + ".diffuse").c_str()), light.diffuse.x, light.diffuse.y, light.diffuse.z);
        glUniform3f(glGetUniformLocation(shader.getID(), (base + ".specular").c_str()), light.specular.x, light.specular.y, light.specular.z);
        glUniform1f(glGetUniformLocation(shader.getID(), (base + ".constant").c_str()), light.constant);
        glUniform1f(glGetUniformLocation(shader.getID(), (base + ".linear").c_str()), light.linear);
        glUniform1f(glGetUniformLocation(shader.getID(), (base + ".quadratic").c_str()), light.quadratic);
        glUniform1f(glGetUniformLocation(shader.getID(), (base + ".cutoff").c_str()), light.cutoff);
        glUniform1f(glGetUniformLocation(shader.getID(), (base + ".outerCutoff").c_str()), light.outerCutoff);
    }

    for (Mesh& mesh : meshes) {
        const Material& mat = overrideMaterial ? *overrideMaterial : mesh.material;

        glUniform3f(glGetUniformLocation(shader.getID(), "material.ambient"),  mat.ambient.x,  mat.ambient.y,  mat.ambient.z);
        glUniform3f(glGetUniformLocation(shader.getID(), "material.diffuse"),  mat.diffuse.x,  mat.diffuse.y,  mat.diffuse.z);
        glUniform3f(glGetUniformLocation(shader.getID(), "material.specular"), mat.specular.x, mat.specular.y, mat.specular.z);
        glUniform1f(glGetUniformLocation(shader.getID(), "material.shininess"), mat.shininess);

        for (size_t i = 0; i < mesh.textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
            glUniform1i(glGetUniformLocation(shader.getID(), mesh.textures[i].type.c_str()), static_cast<int>(i));
        }

        mesh.Draw();
    }
}