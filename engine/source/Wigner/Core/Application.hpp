#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Core/Window.hpp"
#include "Wigner/Events/Events.hpp"

#include <memory>
#include <functional>

namespace Wigner
{
    class Application
    {
    public:
        Application();
        virtual ~Application();
        bool Run();
        bool OnEvent(Event e);
    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
    };

    Application *CreateApplication();
}