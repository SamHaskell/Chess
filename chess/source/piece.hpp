#pragma once

#include <Wigner.hpp>

namespace Chess {
    enum Piece {
        PIECE_NONE = 0,
        PIECE_PAWN = 1,
        PIECE_ROOK = 2,
        PIECE_KNIGHT = 3,
        PIECE_BISHOP = 4,
        PIECE_QUEEN = 5,
        PIECE_KING = 6,
        PIECE_TYPE_MASK = 7,
        PIECE_BLACK = 8,
        PIECE_WHITE = 16,
        PIECE_COLOR_MASK = 24,
    };

    bool piece_from_char(u8 c, i32& piece);

    bool are_opponents(i32 piece, i32 other);
    bool can_slide_orthogonal(i32 piece);
    bool can_slide_diagonal(i32 piece);
}