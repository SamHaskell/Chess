#include "game.hpp"

namespace Chess
{
    Game::Game()
     : m_MainCamera({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, 16, 16) // Hard-coded until I figure out how I want viewport stuff to work
    {
        Wigner::renderer_init();
    }

    Game::~Game() {
        Wigner::renderer_shutdown();
    }

    void Game::Update(f64 dt)
    {

    }

    void Game::Render()
    {
        auto scene = Wigner::scene_begin(m_MainCamera);
        Wigner::draw_quad(scene, 0.0f, 0.0f, 1.0f, 1.0f, {1.0f, 0.0f, 0.0f, 1.0f});
    }

    bool Game::OnEvent(Wigner::Event &e)
    {
        switch (e.Tag)
        {
        case Wigner::EventTag::KeyEvent:
            LOG_INFO(e.KeyEvent.ToString().c_str());
            break;
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