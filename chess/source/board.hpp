#pragma once

#include <Wigner.hpp>

#include "piece.hpp"
#include "move.hpp"
#include "pin.hpp"

#include <memory> 
#include <array> 
#include <set>

namespace Chess {
    enum CastlingFlags {
        CASTLE_WHITE_KING = (1 << 0),
        CASTLE_BLACK_KING = (1 << 1),
        CASTLE_WHITE_QUEEN = (1 << 2),
        CASTLE_BLACK_QUEEN = (1 << 3)
    };

    struct GameData {
        //POTENTIALLY PUT THIS INTO A BOARD STRUCT SO WE CAN STORE PREVIOUS TURNS
        i32 CurrentBoard[64];
        i32 CurrentCastlingFlags;
        i32 CurrentEnPassantTarget;
        i32 CurrentPlayer;
        std::set<i32> AttackedCells;
        std::vector<i32> LegalMoves;
        
        i32 SelectedCell;
        std::vector<i32> HighlightedMoves;
        std::set<i32> HighlightedCells;
        Wigner::Rect2D BoardRect;
        f64 ElapsedTime;
        std::array<std::shared_ptr<Wigner::Texture2D>, 12> TextureArray;
    };

    constexpr i32 BOARD_INVALID_CELL = 64;

    bool pawn_has_moved(i32 piece, i32 location);

    void enumerate_moves(const std::unique_ptr<GameData>& state);
    void update_attacked_cells(const std::unique_ptr<GameData>& state);
    void update_highlighted_cells(const std::unique_ptr<GameData>& state);

    void emplace_pawn_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_knight_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_orthogonal_sliding_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_diagonal_sliding_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_king_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece);

    std::unique_ptr<GameData> game_create_default();
    void game_load_textures(const std::unique_ptr<GameData>& state);
    void game_render(const std::unique_ptr<GameData>& state, Wigner::SceneData scene);
    void game_on_cell_deselect(const std::unique_ptr<GameData>& state);
    void game_on_cell_select(const std::unique_ptr<GameData>& state, i32 file, i32 rank);
    void game_on_move(const std::unique_ptr<GameData>& state, i32 move);
    void game_on_turn_end(const std::unique_ptr<GameData>& state);
}