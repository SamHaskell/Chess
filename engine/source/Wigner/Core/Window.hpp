#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Events/Events.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <functional>

namespace Wigner {
    struct WindowState {
        i32 Width;
        i32 Height;
        const char* Title;
        std::function<bool(Event)> Callback;
    };

    class Window {
        public:
            Window();
            Window(const Window& ) = delete;
            Window& operator=(const Window& ) = delete;
            ~Window();

            void Update();
            void SetEventCallback(std::function<bool(Event)> callback);
        private:
            GLFWwindow* m_WindowHandle;
            WindowState m_WindowState;
            
    };
}