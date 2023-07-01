#include "game.hpp"

namespace Chess
{
    Game::Game()
        : m_MainCamera({0.0f, 0.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, 1280.0f, 720.0f),
          m_ScreenRect({0.0f, 0.0f, 1280.0f, 720.0f})
    {
        Wigner::renderer_init();
        m_Board = board_create_default();
    }

    Game::~Game()
    {
        Wigner::renderer_shutdown();
    }

    void Game::Update(f64 dt)
    {
        Wigner::Point2D center = rect_get_center(m_ScreenRect);
        m_Board->DrawRect = {center.X - 320.0f, center.Y - 320.0f, 640.0f, 640.0f};
    }

    void Game::Render()
    {
        auto scene = Wigner::scene_begin(m_MainCamera);
        board_render(m_Board, scene);
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
                    i32 x = (i32)(8.0f * (m_MousePosition.X - m_Board->DrawRect.X) / m_Board->DrawRect.Width);
                    i32 y = (i32)(8.0f * (m_MousePosition.Y - m_Board->DrawRect.Y) / m_Board->DrawRect.Height);
                    if (!point_in_rect(m_MousePosition, m_Board->DrawRect)) {
                        m_Board->SelectedCell = BOARD_INVALID_CELL;
                        m_Board->HighlightedCells.clear();
                    } else if (x == get_file(m_Board->SelectedCell) && y == get_rank(m_Board->SelectedCell)) {
                        m_Board->SelectedCell = BOARD_INVALID_CELL;
                        m_Board->HighlightedCells.clear();
                    } else {
                        m_Board->SelectedCell = get_location(x, y);
                        update_highlighted_cells(m_Board);
                    }
                }
            case Wigner::EventTag::KeyEvent:
                // FOR TESTING
                if (e.KeyEvent.KeyCode == GLFW_KEY_SPACE && e.KeyEvent.Action == Wigner::KEY_PRESS) {
                    enumerate_moves(m_Board);
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