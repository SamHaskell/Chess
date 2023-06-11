#include "Wigner/Core/Application.hpp"

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
        while (m_Running)
        {
            m_Window->Update();
        }
        return true;
    }

    void Application::SetLayer(std::unique_ptr<Layer> layer) {
        m_Layer = std::move(layer);
    }

    bool Application::OnEvent(Event &e)
    {
        switch (e.Tag)
        {
        case EventTag::KeyEvent:
            LOG_INFO(e.KeyEvent.ToString().c_str());
            break;
        case EventTag::WindowCloseEvent:
            m_Running = false;
            break;
        }
        m_Layer->OnEvent(e);
        return true;
    }
}