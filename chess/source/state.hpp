#pragma once

#include <Wigner.hpp>
#include <iostream>
#include <array>
#include <vector>

namespace Chess {
    enum PieceType {
        PIECE_PAWN,
        PIECE_ROOK,
        PIECE_KNIGHT,
        PIECE_BISHOP,
        PIECE_QUEEN,
        PIECE_KING,
        PIECE_NONE
    };

    enum Team {
        TEAM_WHITE,
        TEAM_BLACK,
        TEAM_NONE
    };

    struct GamePiece {
        i32 Row;
        i32 Col;
        PieceType Type;
        Team Team;
    };

    struct GameState {
        Wigner::Rect2D BoardRect;
        std::array<std::shared_ptr<Wigner::Texture2D>, 12> PieceTextures;
        GamePiece Pieces[8][8];
        
    };

    std::unique_ptr<GameState> gamestate_create();
    void gamestate_load_textures(const std::unique_ptr<GameState>& state);
    void gamestate_update(const std::unique_ptr<GameState>& state, f64 dt);
    void board_init(const std::unique_ptr<GameState> &state);
    void board_render(const std::unique_ptr<GameState>& state, Wigner::SceneData scene);
    void pieces_render(const std::unique_ptr<GameState>& state, Wigner::SceneData scene);
}