#pragma once

#include <Wigner.hpp>
#include <iostream>

#include "board.hpp"

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
            Wigner::Point2D m_MousePosition;
            std::unique_ptr<GameData> m_GameData;
    };
}