#include "piece.hpp"

namespace Chess {
    bool are_opponents(i32 piece, i32 other) {
        return ((piece | other) & PIECE_COLOR_MASK) == PIECE_COLOR_MASK;
    }

    bool can_slide_orthogonal(i32 piece) {
        i32 type = piece & PIECE_TYPE_MASK;
        return (type == PIECE_ROOK) || (type == PIECE_QUEEN);
    }

    bool can_slide_diagonal(i32 piece) {
        i32 type = piece & PIECE_TYPE_MASK;
        return (type == PIECE_BISHOP) || (type == PIECE_QUEEN);
    }
}