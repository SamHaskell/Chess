#include "Wigner/Core/Window.hpp"

namespace Wigner
{
    Window::Window()
    {
        m_WindowState.Width = 1080;
        m_WindowState.Height = 720;
        m_WindowState.Title = "Application";

        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if PLATFORM_MACOS
        LOG_INFO("Platform Detected: MacOS, enabling OpenGL forward compat");
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

        m_WindowHandle = glfwCreateWindow(1080, 720, "Application", nullptr, nullptr);
        glfwMakeContextCurrent(m_WindowHandle);

        gladLoadGL();
        glClearColor(0.9, 0.9, 0.9, 1.0);

        glfwSetWindowUserPointer(m_WindowHandle, &m_WindowState);

        glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow *window)
                                   {
            WindowState state = *(WindowState*)glfwGetWindowUserPointer(window);
            Event e = {EventTag::WindowCloseEvent};
            if (!state.Callback(e)) {
                glfwSetWindowShouldClose(window, GLFW_FALSE);
            } });
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_WindowHandle);
        glfwTerminate();
    }

    void Window::Update()
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(m_WindowHandle);
    }

    void Window::SetEventCallback(std::function<bool(Event)> callback) {
        m_WindowState.Callback = callback;
    }
}