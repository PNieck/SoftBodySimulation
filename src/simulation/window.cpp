#include <simulation/window.hpp>

#include <glad/glad.h>


unsigned int Window::instances_cnt = 0;


Window::Window(int width, int height, const std::string & name):
    window(CreateGFLWwindow(width, height, name)), controller(window)
{
    // Set window pointer to this class for all callback
    glfwSetWindowUserPointer(window, this);

    // Enable "vsync"
    glfwSwapInterval(1);
}


Window::~Window()
{
    glfwDestroyWindow(window);

    if (--instances_cnt == 0) {
        DeinitializeGLFW();
    }
}


GLFWwindow * Window::CreateGFLWwindow(int width, int height, const std::string &name)
{
    if (instances_cnt == 0) {
        InitializeGLFW();
    }

    GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        throw WindowCreationError("Cannot create window");
    }

    glfwMakeContextCurrent(window);
    InitializeGlad();

    // Set callbacks
    glfwSetCursorPosCallback(window, MouseMoveCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    instances_cnt++;

    return window;
}


void Window::InitializeGLFW()
{
    if (glfwInit() != GLFW_TRUE) {
        throw WindowCreationError("Cannot initialize window module");
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


void Window::DeinitializeGLFW()
{
    glfwTerminate();
}


void Window::InitializeGlad()
{
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwTerminate();
        throw WindowCreationError("Cannot initialize Glad");
    }
}


void Window::RunMessageLoop()
{
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        controller.Render();

        glfwSwapBuffers(window);
    }
}


void Window::MouseMoveCallback(GLFWwindow *glfwWindow, const double xpos, const double ypos)
{
    auto* window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    window->controller.MouseMoved(static_cast<int>(xpos), static_cast<int>(ypos));
}


void Window::MouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    (void)mods;

    auto* window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));

    MouseButton mouseButton;

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButton = MouseButton::Left;
        break;

        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButton = MouseButton::Middle;
        break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            mouseButton = MouseButton::Right;
        break;

        default:
            return;
    }

    switch (action)
    {
        case GLFW_PRESS:
            window->controller.MouseClicked(mouseButton);
        break;

        case GLFW_RELEASE:
            window->controller.MouseReleased(mouseButton);
        break;

        default:
            break;
    }
}


void Window::ScrollCallback(GLFWwindow *glfwWindow, const double xoffset, const double yoffset)
{
    (void)xoffset;

    auto* window = static_cast<Window *>(glfwGetWindowUserPointer(glfwWindow));
    window->controller.ScrollMoved(static_cast<int>(yoffset));
}
