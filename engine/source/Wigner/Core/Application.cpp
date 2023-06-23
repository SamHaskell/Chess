#include "Wigner/Core/Application.hpp"
#include "Wigner/Graphics/Renderer.hpp"

#include <chrono>

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
        auto frame_start = std::chrono::high_resolution_clock::now();
        auto frame_end = frame_start;
        f64 dt = 0.0;

        int count = 0;
        f64 accum = 0.0;

        while (m_Running)
        {
            count ++;
            if (count == 60) {
                count = 0;
                LOG_INFO("Frame Time: %f", accum/60.0);
                accum = 0.0;
            }
            frame_start = frame_end;

            m_Window->Update(dt);
            m_Layer->Update(dt);
            m_Layer->Render();
            
            frame_end = std::chrono::high_resolution_clock::now();
            dt = std::chrono::duration<double>(frame_end - frame_start).count();
            accum += dt;
        }

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