#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform(
        const glm::vec3& pos = glm::vec3(0.0f),
        const glm::vec3& rot = glm::vec3(0.0f),
        const glm::vec3& scl = glm::vec3(1.0f)
    );

    glm::mat4 getModelMatrix() const;
};