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

    GLFWwindow* window;

private:
    std::unique_ptr<Scene> scene;
    float lastX, lastY;
    bool firstMouse;
    float lastFrame;

    static constexpr int WINDOW_WIDTH = 1280;
    static constexpr int WINDOW_HEIGHT = 720;
    static constexpr const char* WINDOW_TITLE = "OpenGL Engine";

    void framebufferResizeCallback(int width, int height);
};