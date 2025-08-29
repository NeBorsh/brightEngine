#pragma once

#include "model.h"
#include "transform.h"
#include "mesh.h"
#include "audioSource.h"

class GameObject {
public:
    Transform transform;
    Model* model = nullptr;
    Material materialOverride;
    bool useMaterialOverride = false;

    AudioSource audioSource;

    GameObject() = default;
    GameObject(Model* model, const Transform& transform = Transform());

    void Draw(const Shader& shader, const std::vector<Light>& lights, const glm::vec3& viewPos) const;
};