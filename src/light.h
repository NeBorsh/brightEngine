#pragma once

#include <glm/glm.hpp>

enum LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light {
    LightType type;

    glm::vec3 position;
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutoff;
    float outerCutoff;

    Light();

    static Light Directional(const glm::vec3& dir);
    static Light PointLight(const glm::vec3& pos);
    static Light SpotLight(const glm::vec3& pos, const glm::vec3& dir);
};