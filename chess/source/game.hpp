#pragma once

#include <Wigner.hpp>
#include <iostream>

#include "state.hpp"

namespace Chess {
    class Game : public Wigner::Layer {
        public:
            Game();
            ~Game() override;
            void Update(f64 dt) override;
            void Render() override;
            bool OnEvent(Wigner::Event &e) override;
            void OnEnable() override;
            void OnDisable() override;

        private:
            Wigner::OrthographicCamera m_MainCamera;
            Wigner::Rect2D m_ScreenRect;
            std::unique_ptr<GameState> m_GameState;
    };
}