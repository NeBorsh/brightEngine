#include <iostream>

#include "engine.h"
#include "model.h"
#include "shader.h"
#include "scene.h"
#include "light.h"
#include "gameobject.h"
#include "camera.h"

int main() {
    Engine engine;
    if (!engine.init()) return -1;

    auto scene = std::make_unique<Scene>();
    scene->camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    auto shader = std::make_unique<Shader>("shaders/model.vert", "shaders/model.frag");
    if (!shader->isCompiled()) {
        std::cerr << "Shader failed to compile!" << std::endl;
        return -1;
    }

    Model* model = new Model("models/model.obj");
    if (!model) {
        std::cerr << "Failed to load model!" << std::endl;
        return -1;
    }

    Transform transform(glm::vec3(0.0f, 0.0f, -5.0f),
                        glm::vec3(-90.0f, 0.0f, 0.0f),
                        glm::vec3(0.5f));
    GameObject go(model, transform);
    scene->gameObjects.push_back(go);

    Light pointLight = Light::PointLight(glm::vec3(2.0f, 4.0f, 2.0f));
    pointLight.ambient = glm::vec3(0.05f);
    pointLight.diffuse = glm::vec3(0.8f);
    pointLight.specular = glm::vec3(1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
    scene->lights.push_back(pointLight);

    Light dirLight = Light::Directional(glm::vec3(-0.2f, -1.0f, -0.3f));
    dirLight.ambient = glm::vec3(0.05f);
    dirLight.diffuse = glm::vec3(0.4f);
    dirLight.specular = glm::vec3(0.5f);
    scene->lights.push_back(dirLight);

    scene->shader = std::move(shader);

    glGenFramebuffers(1, &scene->depthMapFBO);
    glGenTextures(1, &scene->depthMap);
    glBindTexture(GL_TEXTURE_2D, scene->depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Scene::SHADOW_WIDTH, Scene::SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, scene->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, scene->depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    engine.setScene(std::move(scene));
    engine.run();

    return 0;
}