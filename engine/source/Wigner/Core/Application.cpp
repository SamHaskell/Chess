#include "Wigner/Core/Application.hpp"

#include "glad/glad.h"
#include <iostream>

namespace Wigner
{
    Application::Application()
        : m_Running(true), m_Window(std::make_unique<Window>())
    {
    }
    Application::~Application() {}

    bool Application::Run()
    {
        while (m_Running)
        {
            m_Window->Update();
        }
        return true;
    }
}