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

    mutable glm::mat4 lightSpaceMatrix;

    Scene();
    void update(float deltaTime);
    void render(int width, int height) const;
    void renderShadowMap(int width, int height) const;

private:
    std::unique_ptr<Shader> shadowShader;
};