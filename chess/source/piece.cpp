#include "piece.hpp"

namespace Chess {
    bool piece_from_char(u8 c, i32& piece) {
        // DON'T LOOK MARION, KEEP YOUR EYES SHUT!
        switch (c) {
            case 'P':
                piece = PIECE_WHITE | PIECE_PAWN;
                break;
            case 'K':
                piece = PIECE_WHITE | PIECE_KING;
                break;
            case 'Q':
                piece = PIECE_WHITE | PIECE_QUEEN;
                break;
            case 'B':
                piece = PIECE_WHITE | PIECE_BISHOP;
                break;
            case 'N':
                piece = PIECE_WHITE | PIECE_KNIGHT;
                break;
            case 'R':
                piece = PIECE_WHITE | PIECE_ROOK;
                break;
            case 'p':
                piece = PIECE_BLACK | PIECE_PAWN;
                break;
            case 'k':
                piece = PIECE_BLACK | PIECE_KING;
                break;
            case 'q':
                piece = PIECE_BLACK | PIECE_QUEEN;
                break;
            case 'b':
                piece = PIECE_BLACK | PIECE_BISHOP;
                break;
            case 'n':
                piece = PIECE_BLACK | PIECE_KNIGHT;
                break;
            case 'r':
                piece = PIECE_BLACK | PIECE_ROOK;
                break;
            default:
                piece = PIECE_NONE;
                return false;
        }
        return true;
    }

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