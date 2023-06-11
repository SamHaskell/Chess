#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Core/Window.hpp"
#include "Wigner/Core/Layer.hpp"
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
        bool OnEvent(Event &e);
        void SetLayer(std::unique_ptr<Layer> layer);

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
        std::unique_ptr<Layer> m_Layer;
    };

    Application *CreateApplication();
}