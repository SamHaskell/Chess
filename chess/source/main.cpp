#include <Wigner.hpp>
#include <Wigner/Core/Entrypoint.h>
#include "game.hpp"
#include <iostream>

class Program : public Wigner::Application {
    public:
        Program() {
            SetLayer(std::make_unique<Chess::Game>());
        }
        ~Program() {}
    private:
};

Wigner::Application *Wigner::CreateApplication() {
    return new Program();
}