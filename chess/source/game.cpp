#include "game.hpp"

namespace Chess {
    bool Game::OnEvent(Wigner::Event &e) {
        switch (e.Tag)
        {
            case Wigner::EventTag::KeyEvent:
                LOG_INFO(e.KeyEvent.ToString().c_str());
                break;
        }
        return true;   
    };
}