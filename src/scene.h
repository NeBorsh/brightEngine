#pragma once

#include <memory>
#include <vector>
#include "camera.h"
#include "light.h"
#include "gameobject.h"
#include "shader.h"

class Scene {
public:
    std::vector<GameObject> gameObjects;
    Camera camera;
    std::vector<Light> lights;
    std::unique_ptr<Shader> shader;

    unsigned int depthMapFBO = 0;
    unsigned int depthMap = 0;
    static const int SHADOW_WIDTH = 2048;
    static const int SHADOW_HEIGHT = 2048;

    glm::mat4 lightSpaceMatrix;

    Scene() = default;
    void update(float deltaTime);
    void render() const;
    void renderShadowMap();
};