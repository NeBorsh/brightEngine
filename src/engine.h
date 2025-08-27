#pragma once

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scene.h"

class Engine {
public:
    Engine();
    ~Engine();

    bool init();
    void setScene(std::unique_ptr<Scene> scene);
    void run();

    void processInput(float deltaTime);
    void handleMouseMovement(double xpos, double ypos);
    void handleMouseScroll(double xoffset, double yoffset);

private:
    GLFWwindow* window;
    std::unique_ptr<Scene> scene;

    float lastX, lastY;
    bool firstMouse;
    float lastFrame;

    static constexpr int WINDOW_WIDTH = 800;
    static constexpr int WINDOW_HEIGHT = 600;
    static constexpr const char* WINDOW_TITLE = "OpenGL Engine";

    void framebufferResizeCallback(int width, int height);
};