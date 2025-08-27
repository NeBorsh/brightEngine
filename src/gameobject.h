#pragma once

#include "model.h"
#include "transform.h"
#include "mesh.h"

class GameObject {
public:
    Transform transform;
    Model* model;
    Material materialOverride;
    bool useMaterialOverride;

    GameObject(Model* model, const Transform& transform = Transform());
    void Draw(const Shader& shader, const std::vector<Light>& lights, const glm::vec3& viewPos) const;
};