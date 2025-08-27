#include "scene.h"
#include <iostream>
#include <glad/glad.h>
#include "shader.h"
#include "glm/gtc/type_ptr.hpp"

Scene::Scene() {
    shadowShader = std::make_unique<Shader>("shaders/shadow.vert", "shaders/shadow.frag");
    if (!shadowShader->isCompiled()) {
        std::cerr << "Failed to compile shadow shader!" << std::endl;
    }
}

void Scene::update(float deltaTime) {}

void Scene::renderShadowMap(int width, int height) const {
    if (!shader || !shadowShader->isCompiled()) return;

    glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 20.0f);
    glm::mat4 lightView = glm::lookAt(-2.0f * lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    shadowShader->use();
    glUniformMatrix4fv(glGetUniformLocation(shadowShader->getID(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    for (const auto& obj : gameObjects) {
        glm::mat4 model = obj.transform.getModelMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shadowShader->getID(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        obj.model->Draw(*shadowShader, {}, glm::vec3(0.0f), nullptr);
    }

    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

void Scene::render(int width, int height) const {
    if (!shader || gameObjects.empty()) return;

    renderShadowMap(width, height);

    float aspect = static_cast<float>(width) / static_cast<float>(height);
    glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), aspect, 0.1f, 100.0f);
    glm::mat4 view = camera.getViewMatrix();

    shader->use();
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(glGetUniformLocation(shader->getID(), "viewPos"), camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    glUniform1i(glGetUniformLocation(shader->getID(), "numLights"), static_cast<int>(lights.size()));
    glUniformMatrix4fv(glGetUniformLocation(shader->getID(), "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shader->getID(), "shadowMap"), 1);

    for (size_t i = 0; i < lights.size() && i < MAX_LIGHTS; ++i) {
        const Light& light = lights[i];
        std::string base = "lights[" + std::to_string(i) + "]";
        glUniform1i(glGetUniformLocation(shader->getID(), (base + ".type").c_str()), static_cast<int>(light.type));
        glUniform3f(glGetUniformLocation(shader->getID(), (base + ".position").c_str()), light.position.x, light.position.y, light.position.z);
        glUniform3f(glGetUniformLocation(shader->getID(), (base + ".direction").c_str()), light.direction.x, light.direction.y, light.direction.z);
        glUniform3f(glGetUniformLocation(shader->getID(), (base + ".ambient").c_str()), light.ambient.x, light.ambient.y, light.ambient.z);
        glUniform3f(glGetUniformLocation(shader->getID(), (base + ".diffuse").c_str()), light.diffuse.x, light.diffuse.y, light.diffuse.z);
        glUniform3f(glGetUniformLocation(shader->getID(), (base + ".specular").c_str()), light.specular.x, light.specular.y, light.specular.z);
        glUniform1f(glGetUniformLocation(shader->getID(), (base + ".constant").c_str()), light.constant);
        glUniform1f(glGetUniformLocation(shader->getID(), (base + ".linear").c_str()), light.linear);
        glUniform1f(glGetUniformLocation(shader->getID(), (base + ".quadratic").c_str()), light.quadratic);
        glUniform1f(glGetUniformLocation(shader->getID(), (base + ".cutoff").c_str()), light.cutoff);
        glUniform1f(glGetUniformLocation(shader->getID(), (base + ".outerCutoff").c_str()), light.outerCutoff);
    }

    for (const auto& obj : gameObjects) {
        obj.Draw(*shader, lights, camera.getPosition());
    }
}