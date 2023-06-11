#include "Wigner/Core/Application.hpp"

#include "Wigner/Events/Events.hpp"

#include "glad/glad.h"
#include <iostream>

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

    bool Application::OnEvent(Event e)
    {
        switch(e.Tag) {
            case EventTag::KeyEvent:
                LOG_INFO(e.KeyEvent.ToString().c_str());
                break;
            case EventTag::MouseButtonEvent:
                LOG_INFO(e.MouseButtonEvent.ToString().c_str());
                break;
            case EventTag::MouseMoveEvent:
                LOG_INFO(e.MouseMoveEvent.ToString().c_str());
                break;
            case EventTag::WindowCloseEvent:
                LOG_INFO(e.WindowCloseEvent.ToString().c_str());
                m_Running = false;
                break;
        }
        return true;
    }
}