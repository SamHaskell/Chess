#pragma once

#include <Wigner.hpp>
#include <iostream>

namespace Chess {
    class Game : public Wigner::Layer {
        public:
            Game() {};
            ~Game() override {};
            virtual void Update() override {};
            virtual void Render() override {};
            virtual bool OnEvent(Wigner::Event &e) override;
            virtual void OnEnable() override {};
            virtual void OnDisable() override {};
    };
}