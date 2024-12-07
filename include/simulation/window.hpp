#pragma once

#include <string>
#include <stdexcept>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "controllers/mainController.hpp"


class Window {
public:
    Window(int width, int height, const std::string &name);
    ~Window();

    void RunMessageLoop();

private:
    static unsigned int instances_cnt;

    GLFWwindow* window;
    MainController controller;

    static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static GLFWwindow* CreateGFLWwindow(int width, int height, const std::string &name);

    static void InitializeGLFW();
    static void DeinitializeGLFW();

    static void InitializeGlad();
};


class WindowCreationError final : public std::runtime_error {
public:
    explicit WindowCreationError(const std::string &msg): runtime_error(msg) {};
};
