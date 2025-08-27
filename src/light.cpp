#include "light.h"
#include <glm/gtc/constants.hpp>

Light::Light()
    : type(POINT)
    , position(0.0f)
    , direction(-1.0f, -1.0f, -1.0f)
    , ambient(0.1f)
    , diffuse(1.0f)
    , specular(1.0f)
    , constant(1.0f)
    , linear(0.09f)
    , quadratic(0.032f)
    , cutoff(glm::cos(glm::radians(12.5f)))
    , outerCutoff(glm::cos(glm::radians(17.5f)))
{
}

Light Light::Directional(const glm::vec3& dir) {
    Light l;
    l.type = DIRECTIONAL;
    l.direction = glm::normalize(dir);
    return l;
}

Light Light::PointLight(const glm::vec3& pos) {
    Light l;
    l.type = POINT;
    l.position = pos;
    return l;
}

Light Light::SpotLight(const glm::vec3& pos, const glm::vec3& dir) {
    Light l;
    l.type = SPOT;
    l.position = pos;
    l.direction = glm::normalize(dir);
    return l;
}