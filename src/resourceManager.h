#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>

#include "shader.h"
#include "model.h"

class AudioClip {
public:
    std::string filePath;
    bool loaded = false;
    AudioClip(const std::string& path) : filePath(path) {}
};

class ResourceManager {
public:
    static std::shared_ptr<Shader> getShader(const std::string& vertexPath, const std::string& fragmentPath);
    static std::shared_ptr<Model> getModel(const std::string& path);
    static unsigned int getTexture(const std::string& path);
    static std::shared_ptr<AudioClip> getAudioClip(const std::string& path);

    static void clear();

private:
    static std::map<std::string, std::shared_ptr<Shader>> shaders;
    static std::map<std::string, std::shared_ptr<Model>> models;
    static std::map<std::string, unsigned int> textures;
    static std::map<std::string, std::shared_ptr<AudioClip>> audioClips;

    static std::mutex resourceMutex;
    
    static std::string getShaderKey(const std::string& vertex, const std::string& fragment);
};