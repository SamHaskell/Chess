#pragma once

#include <Wigner.hpp>

namespace Chess {
    enum CastlingFlags {
        CASTLE_WHITE_KING = (1 << 0),
        CASTLE_BLACK_KING = (1 << 1),
        CASTLE_WHITE_QUEEN = (1 << 2),
        CASTLE_BLACK_QUEEN = (1 << 3)
    };

    constexpr i32 BOARD_INVALID_CELL = 64;

    struct Position {
        i32 Board[64];
        i32 CastlingFlags;
        i32 EnPassantTarget;
        i32 Player;
        i32 HalfmoveClock;
        i32 FullmoveCounter;
    };

    Position read_position(const std::string& fen);
    std::string write_position(Position position);
    Position create_default_position();
}