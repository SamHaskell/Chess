#pragma once

#include "Wigner/Base/Base.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Wigner {
    struct WindowState {
        i32 WindowWidth;
        i32 WindowHeight;
    };

    class Window {
        public:
            Window();
            Window(const Window& ) = delete;
            Window& operator=(const Window& ) = delete;
            ~Window();

            void Update();
        private:
            GLFWwindow* m_WindowHandle;
            
    };
}