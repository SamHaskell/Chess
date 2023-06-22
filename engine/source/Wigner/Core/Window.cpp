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
        glfwSwapInterval(1);

        gladLoadGL();
        glClearColor(0.1, 0.1, 0.1, 1.0);

        glfwSetWindowUserPointer(m_WindowHandle, &m_WindowState);

        glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow *window)
                                   {
            auto state = (WindowState*)glfwGetWindowUserPointer(window);
            Event e = {EventTag::WindowCloseEvent};
            if (!state->Callback(e)) {
                glfwSetWindowShouldClose(window, GLFW_FALSE);
            } });

        glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mods) {
            auto state = (WindowState*)glfwGetWindowUserPointer(window);
            Event e = {EventTag::KeyEvent};
            switch (action) {
                case GLFW_PRESS:
                    e.KeyEvent = {key, KeyAction::KEY_PRESS};
                    break;
                case GLFW_RELEASE:
                    e.KeyEvent = {key, KeyAction::KEY_RELEASE};
                    break;
                case GLFW_REPEAT:
                    e.KeyEvent = {key, KeyAction::KEY_REPEAT};
                    break;
            }
            state->Callback(e); }
        );

        glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow *window, i32 button, i32 action, i32 mods) {
            auto state = (WindowState*)glfwGetWindowUserPointer(window);
            Event e = {EventTag::MouseButtonEvent};
            switch (action) {
                case GLFW_PRESS:
                    e.MouseButtonEvent = {button, MouseButtonAction::MOUSEBUTTON_PRESS};
                break;
                case GLFW_RELEASE:
                    e.MouseButtonEvent = {button, MouseButtonAction::MOUSEBUTTON_RELEASE};
                break;
            }
            state->Callback(e); }
        );

        glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow *window, f64 xpos, f64 ypos) {
            auto state = (WindowState *)glfwGetWindowUserPointer(window);
            Event e = {EventTag::MouseMoveEvent};
            e.MouseMoveEvent = {xpos, ypos};
            state->Callback(e); }
        );
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_WindowHandle);
        glfwTerminate();
    }

    void Window::Update(f64 dt)
    {
        glfwSwapBuffers(m_WindowHandle);

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Window::SetEventCallback(std::function<bool(Event &)> callback)
    {
        m_WindowState.Callback = callback;
    }
}