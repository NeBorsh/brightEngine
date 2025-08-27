#include "gameobject.h"

#include "glm/gtc/type_ptr.hpp"

GameObject::GameObject(Model* model, const Transform& transform)
    : model(model)
    , transform(transform)
    , useMaterialOverride(false) {
}

void GameObject::Draw(const Shader& shader, const std::vector<Light>& lights, const glm::vec3& viewPos) const {
    if (!model) return;

    glm::mat4 modelMatrix = transform.getModelMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader.getID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    const Material* matPtr = useMaterialOverride ? &materialOverride : nullptr;
    model->Draw(shader, lights, viewPos, matPtr);
}