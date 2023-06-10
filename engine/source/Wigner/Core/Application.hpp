#pragma once

#include "Wigner/Core/Window.hpp"
#include <memory>

namespace Wigner
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        bool Run();
    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
    };

    Application *CreateApplication();
}