#include "engine.h"
#include <iostream>

Engine::Engine()
    : window(nullptr)
    , scene(nullptr)
    , lastX(WINDOW_WIDTH / 2.0f)
    , lastY(WINDOW_HEIGHT / 2.0f)
    , firstMouse(true)
    , lastFrame(0.0f) {
}

Engine::~Engine() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Engine::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* w, int width, int height) {
        auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
        engine->framebufferResizeCallback(width, height);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
        engine->handleMouseMovement(x, y);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* w, double x, double y) {
        auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
        engine->handleMouseScroll(x, y);
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

void Engine::setScene(std::unique_ptr<Scene> scene) {
    this->scene = std::move(scene);
}

void Engine::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(deltaTime);
        if (scene) scene->update(deltaTime);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (scene) scene->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Engine::processInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!scene) return;

    float velocity = 2.5f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        scene->camera.processKeyboard(FORWARD, velocity);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        scene->camera.processKeyboard(BACKWARD, velocity);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        scene->camera.processKeyboard(LEFT, velocity);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        scene->camera.processKeyboard(RIGHT, velocity);
}

void Engine::handleMouseMovement(double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
        return;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    if (scene) {
        scene->camera.processMouseMovement(xoffset, yoffset);
    }
}

void Engine::handleMouseScroll(double xoffset, double yoffset) {
    if (scene) {
        scene->camera.processMouseScroll(static_cast<float>(yoffset));
    }
}

void Engine::framebufferResizeCallback(int width, int height) {
    glViewport(0, 0, width, height);
}