#include "Wigner/Core/Application.hpp"
#include "Wigner/Graphics/Renderer.hpp"

namespace Wigner
{
    Application::Application()
        : m_Running(true),
          m_Window(std::make_unique<Window>())
    {
        m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
    }

    Application::~Application()
    {
    }

    bool Application::Run()
    {
        renderer_init();
        while (m_Running)
        {
            f64 dt = 0.013333; // TEMPORARY UNTIL WE IMPLEMENT DELTATIME
            m_Window->Update(dt);
            m_Layer->Update(dt);
            m_Layer->Render();
        }
        renderer_shutdown();
        return true;
    }

    void Application::SetLayer(std::unique_ptr<Layer> layer)
    {
        m_Layer = std::move(layer);
    }

    bool Application::OnEvent(Event &e)
    {
        switch (e.Tag)
        {
        case EventTag::WindowCloseEvent:
            m_Running = false;
            break;
        default:
            break;
        }

        m_Layer->OnEvent(e);

        return true;
    }
}