#include <Wigner.hpp>

#include "game.hpp"
#include <iostream>

class Testbed : public Wigner::Application {
    public:
        Testbed() {}
        ~Testbed() {}
    private:
};

Wigner::Application *Wigner::CreateApplication() {
    return new Testbed();
}