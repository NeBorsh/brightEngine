#include "resourceManager.h"
#include <iostream>

#include "texture.h"

std::map<std::string, std::shared_ptr<Shader>> ResourceManager::shaders;
std::map<std::string, std::shared_ptr<Model>> ResourceManager::models;
std::map<std::string, unsigned int> ResourceManager::textures;
std::map<std::string, std::shared_ptr<AudioClip>> ResourceManager::audioClips;
std::mutex ResourceManager::resourceMutex;

std::string ResourceManager::getShaderKey(const std::string& vertex, const std::string& fragment) {
    return vertex + "|" + fragment;
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::lock_guard<std::mutex> lock(resourceMutex);
    std::string key = getShaderKey(vertexPath, fragmentPath);

    auto it = shaders.find(key);
    if (it != shaders.end()) {
        return it->second;
    }

    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    if (shader->isCompiled()) {
        shaders[key] = shader;
        std::cout << "Shader loaded and cached: " << key << std::endl;
        return shader;
    } else {
        std::cerr << "Failed to load shader: " << key << std::endl;
        return nullptr;
    }
}

std::shared_ptr<Model> ResourceManager::getModel(const std::string& path) {
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto it = models.find(path);
    if (it != models.end()) {
        return it->second;
    }

    auto model = std::make_shared<Model>(path);
    if (model && !model->getMeshes().empty()) {
        models[path] = model;
        std::cout << "Model loaded and cached: " << path << std::endl;
        return model;
    } else {
        std::cerr << "Failed to load model: " << path << std::endl;
        return nullptr;
    }
}

unsigned int ResourceManager::getTexture(const std::string& path) {
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    unsigned int id = loadTexture(path);
    if (id != 0) {
        textures[path] = id;
        std::cout << "Texture loaded and cached: " << path << std::endl;
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }

    return id;
}

std::shared_ptr<AudioClip> ResourceManager::getAudioClip(const std::string& path) {
    std::lock_guard<std::mutex> lock(resourceMutex);

    auto it = audioClips.find(path);
    if (it != audioClips.end()) {
        return it->second;
    }

    auto clip = std::make_shared<AudioClip>(path);
    audioClips[path] = clip;
    std::cout << "Audio clip registered: " << path << std::endl;
    return clip;
}

void ResourceManager::clear() {
    std::lock_guard<std::mutex> lock(resourceMutex);

    shaders.clear();
    models.clear();
    textures.clear();
    audioClips.clear();

    std::cout << "ResourceManager: All resources cleared." << std::endl;
}