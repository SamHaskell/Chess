#pragma once

#include "Wigner/Base/Base.hpp"
#include "Wigner/Core/Application.hpp"

extern Wigner::Application *Wigner::CreateApplication();

int main(int argc, char **argv) {
    auto app = Wigner::CreateApplication();
    if (!app->Run()) {
        // Do whatever you want buddy
    }
    delete app;
}