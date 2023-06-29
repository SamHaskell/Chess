#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f),
          m_ScreenRect({0.0f, 0.0f, 1280.0f, 720.0f})
    {
        Wigner::renderer_init();
        m_GameState = gamestate_create();
    }

    Game::~Game()
    {
        Wigner::renderer_shutdown();
    }

    void Game::Update(f64 dt)
    {
        Wigner::Point2D center = rect_get_center(m_ScreenRect);
        m_GameState->BoardRect = {center.X - 320.0f, center.Y - 320.0f, 640.0f, 640.0f};
        gamestate_update(m_GameState, dt);
    }

    void Game::Render()
    {
        auto scene = Wigner::scene_begin(m_MainCamera);
        board_render(m_GameState, scene);
    }

    bool Game::OnEvent(Wigner::Event &e)
    {
        switch (e.Tag)
        {
        case Wigner::EventTag::WindowSizeEvent:
            m_ScreenRect.Width = e.WindowSizeEvent.Width;
            m_ScreenRect.Height = e.WindowSizeEvent.Height;
            m_MainCamera.SetFrustumRect(e.WindowSizeEvent.Width, e.WindowSizeEvent.Height);
        case Wigner::EventTag::MouseMoveEvent:
            m_MousePosition = {(f32)e.MouseMoveEvent.X, m_ScreenRect.Height - (f32)e.MouseMoveEvent.Y};
        case Wigner::EventTag::MouseButtonEvent:
            if (e.MouseButtonEvent.ButtonCode == GLFW_MOUSE_BUTTON_LEFT && e.MouseButtonEvent.Action == GLFW_PRESS) {
                i32 y = (i32)(8.0f*(m_MousePosition.Y - m_GameState->BoardRect.Y)/m_GameState->BoardRect.Height);
                i32 x = (i32)(8.0f*(m_MousePosition.X - m_GameState->BoardRect.X)/m_GameState->BoardRect.Width);
                Coord temp = {y, x};
                if (!point_in_rect(m_MousePosition, m_GameState->BoardRect)) {
                    on_cell_deselect(m_GameState);
                } else if (temp.Col == m_GameState->SelectedCell.Col && temp.Row == m_GameState->SelectedCell.Row) {
                    on_cell_deselect(m_GameState);
                } else {
                    if (m_GameState->Pieces[x][y].Team == m_GameState->CurrentTeam) {
                        on_cell_select(m_GameState, {y, x});
                    } else {
                        on_cell_deselect(m_GameState);
                    }
                }
            }
        case Wigner::EventTag::KeyEvent:
            if (e.KeyEvent.KeyCode == GLFW_KEY_SPACE && e.KeyEvent.Action == GLFW_PRESS) {
                m_GameState->CurrentTeam = (Team)(((int)m_GameState->CurrentTeam + 1) % 2);
                LOG_INFO("TEAM SWITCHED!");
            }
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