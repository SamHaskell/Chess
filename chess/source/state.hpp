#pragma once

#include <Wigner.hpp>
#include <iostream>
#include <array>
#include <vector>

namespace Chess {
    #define UNSELECTED_INDEX -1

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

    struct Coord {
        i32 X;
        i32 Y;
    };

    struct GamePiece {
        PieceType Type;
        Team Team;
        bool FirstMove = true;
    };

    struct GameState {
        Wigner::Rect2D BoardRect;
        std::array<std::shared_ptr<Wigner::Texture2D>, 12> PieceTextures;
        GamePiece Pieces[8][8];
        Coord SelectedCell;
        std::vector<Coord> HighlightedCells;
        Team CurrentTeam;
        bool IsBlackInCheck;
        bool IsWhiteInCheck;
        bool CanBlackCastle;
        bool CanWhiteCastle;
    };

    std::unique_ptr<GameState> gamestate_create();
    void gamestate_load_textures(const std::unique_ptr<GameState>& state);
    void gamestate_update(const std::unique_ptr<GameState>& state, f64 dt);
    void board_init(const std::unique_ptr<GameState> &state);
    void board_render(const std::unique_ptr<GameState>& state, Wigner::SceneData scene);
    void on_cell_select(const std::unique_ptr<GameState>& state, Coord cell);
    void on_cell_deselect(const std::unique_ptr<GameState>& state);
    bool check_valid_move(const std::unique_ptr<GameState>& state, Coord cell);
}