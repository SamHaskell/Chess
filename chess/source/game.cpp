#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f),
          m_ScreenRect({0.0f, 0.0f, 1280.0f, 720.0f}),
          m_TestTexture("assets/user/textures/spyro.png")
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
        auto scene = Wigner::scene_begin(m_MainCamera);

        Wigner::Color black = {0.1f, 0.1f, 0.1f, 1.0f};
        Wigner::Color white = {0.9f, 0.9f, 0.9f, 1.0f};
        Wigner::Color test = {1.0f, 1.0f, 1.0f, 1.0f};
        Wigner::Point2D center = rect_get_center(m_ScreenRect);
        
        for (i32 j = 0; j < 8; j++)
        {
            for (i32 i = 0; i < 8; i++)
            {
                Wigner::Color col = ((i + 7 * j) % 2 == 0) ? black : white;
                Wigner::draw_quad(scene, center.X - 320.0f + 80.0f * i, center.Y - 320.0f + 80.0f * j, 80.0f, 80.0f, col);
            }
        }

        Wigner::draw_textured_quad(scene, m_TestTexture, 0.0f, 0.0f, 100.0f, 100.0f, test);
    }

    bool Game::OnEvent(Wigner::Event &e)
    {
        switch (e.Tag)
        {
        case Wigner::EventTag::WindowSizeEvent:
            m_ScreenRect.Width = e.WindowSizeEvent.Width;
            m_ScreenRect.Height = e.WindowSizeEvent.Height;
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