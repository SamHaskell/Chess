#include "game.hpp"

namespace Chess
{
    void Game::Update(f64 dt)
    {

    }

    void Game::Render()
    {

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