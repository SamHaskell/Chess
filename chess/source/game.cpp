#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f),
          m_ScreenRect({0.0f, 0.0f, 1280.0f, 720.0f})
    {
        Wigner::renderer_init();
        m_GameData = game_create_from_fen("R6R/3Q4/1Q4Q1/4Q3/2Q4Q/Q4Q2/pp1Q4/kBNN1KB1 w - - 0 1");
    }

    Game::~Game()
    {
        Wigner::renderer_shutdown();
    }

    void Game::Update(f64 dt)
    {
        Wigner::Point2D center = rect_get_center(m_ScreenRect);
        m_GameData->ElapsedTime += dt;
        m_GameData->BoardRect = {center.X - 320.0f, center.Y - 320.0f, 640.0f, 640.0f};
    }

    void Game::Render()
    {
        auto scene = Wigner::scene_begin(m_MainCamera);
        game_render(m_GameData, scene);
    }

    bool Game::OnEvent(Wigner::Event &e)
    {
        switch (e.Tag) {
            case Wigner::EventTag::WindowSizeEvent:
                m_ScreenRect.Width = e.WindowSizeEvent.Width;
                m_ScreenRect.Height = e.WindowSizeEvent.Height;
                m_MainCamera.SetFrustumRect(e.WindowSizeEvent.Width, e.WindowSizeEvent.Height);
            case Wigner::EventTag::MouseMoveEvent:
                m_MousePosition = {(f32)e.MouseMoveEvent.X, m_ScreenRect.Height - (f32)e.MouseMoveEvent.Y};
            case Wigner::EventTag::MouseButtonEvent:
                if (e.MouseButtonEvent.ButtonCode == GLFW_MOUSE_BUTTON_LEFT && e.MouseButtonEvent.Action == Wigner::MOUSEBUTTON_PRESS) {
                    i32 x = (i32)(8.0f * (m_MousePosition.X - m_GameData->BoardRect.X) / m_GameData->BoardRect.Width);
                    i32 y = (i32)(8.0f * (m_MousePosition.Y - m_GameData->BoardRect.Y) / m_GameData->BoardRect.Height);
                    if (point_in_rect(m_MousePosition, m_GameData->BoardRect)) {
                        game_on_cell_select(m_GameData, x, y);
                    } else {
                        game_on_cell_deselect(m_GameData);
                    }
                }
            case Wigner::EventTag::KeyEvent:

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