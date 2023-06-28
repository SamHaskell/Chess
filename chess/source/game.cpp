#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f),
          m_ScreenRect({0.0f, 0.0f, 1280.0f, 720.0f}),
          m_TestPiece("assets/user/textures/w_rook_2x_ns.png")
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

        Wigner::Color dark = {0.25f, 0.4f, 0.25f, 1.0f};
        Wigner::Color light = {0.9f, 0.9f, 0.9f, 1.0f};
        Wigner::Color test = {1.0f, 1.0f, 1.0f, 1.0f};
        Wigner::Point2D center = rect_get_center(m_ScreenRect);
        
        for (i32 j = 0; j < 8; j++)
        {
            for (i32 i = 0; i < 8; i++)
            {
                Wigner::Color col = ((i + 7 * j) % 2 == 0) ? dark : light;
                Wigner::draw_quad(scene, center.X - 320.0f + 80.0f * i, center.Y - 320.0f + 80.0f * j, 80.0f, 80.0f, col);
            }
        }
        Wigner::draw_textured_quad(scene, m_TestPiece, center.X - 320.0f + 10.0f, center.Y - 320.0f + 10.0f, 60.0f, 60.0f, test);
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