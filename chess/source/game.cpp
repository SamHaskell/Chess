#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f) // Hard-coded until I figure out how I want viewport stuff to work
    {
        Wigner::renderer_init();
    }

    Game::~Game()
    {
        Wigner::renderer_shutdown();
    }

    void Game::Update(f64 dt)
    {
    }

    void Game::Render()
    {
        Wigner::Color black = {0.1f, 0.1f, 0.1f, 1.0f};
        Wigner::Color white = {0.9f, 0.9f, 0.9f, 1.0f};
        auto scene = Wigner::scene_begin(m_MainCamera);
        for (i32 j = 0; j < 8; j++)
        {
            for (i32 i = 0; i < 8; i++)
            {
                Wigner::Color col = ((i + 7 * j) % 2 == 0) ? white : black;
                Wigner::draw_quad(scene, 80.0f * i, 80.0f * j, 80.0f, 80.0f, col);
            }
        }
    }

    bool Game::OnEvent(Wigner::Event &e)
    {
        switch (e.Tag)
        {
        case Wigner::EventTag::KeyEvent:
            LOG_INFO(e.KeyEvent.ToString().c_str());
            break;
        case Wigner::EventTag::WindowSizeEvent:
            m_MainCamera.SetFrustumRect(e.WindowSizeEvent.Width, e.WindowSizeEvent.Height);
        default:
            break;
        }
        return true;
    };

    void Game::OnEnable()
    {
        // Does nothing for now.
    }

    void Game::OnDisable()
    {
        // Does nothing for now.
    }
}