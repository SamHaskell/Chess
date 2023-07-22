#pragma once

#include <Wigner.hpp>

#include <vector>
#include <set>

namespace Chess {
    enum CastlingFlags {
        CASTLE_WHITE_KING = (1 << 0),
        CASTLE_BLACK_KING = (1 << 1),
        CASTLE_WHITE_QUEEN = (1 << 2),
        CASTLE_BLACK_QUEEN = (1 << 3)
    };

    constexpr i32 BOARD_INVALID_CELL = 64;
    constexpr i32 INIT_MOVES_PER_POSITION = 64;

    struct Position {
        i32 Board[64];
        i32 CastlingFlags;
        i32 EnPassantTarget;
        i32 Player;
        i32 HalfmoveClock;
        i32 FullmoveCounter;
    };

    bool move_is_legal(Position position, i32 move);

    Position position_read(const std::string& fen);
    std::string position_write(Position position);
    Position position_default();

    Position position_apply_move(Position position);
    std::vector<i32> position_generate_moves(Position position);
    i32 position_perft(Position position, i32 depth);

    void position_generate_pawn_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece);
    void position_generate_knight_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece);
    void position_generate_orthogonal_sliding_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece);
    void position_generate_diagonal_sliding_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece);
    void position_generate_king_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece);
}