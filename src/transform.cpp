#include "transform.h"

Transform::Transform(
    const glm::vec3& pos,
    const glm::vec3& rot,
    const glm::vec3& scl
)
    : position(pos)
    , rotation(rot)
    , scale(scl) {
}

glm::mat4 Transform::getModelMatrix() const {
    glm::mat4 model(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);
    return model;
}