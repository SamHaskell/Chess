#include "move.hpp"

namespace Chess {
    i32 move_create(i32 origin, i32 target) {
        return origin | (target << 6);
    }

    i32 move_create(i32 origin, i32 target, i32 flags) {
        return origin | (target << 6) | flags;
    }

    i32 move_get_target(i32 movecode) {
        return (movecode & MOVE_TARGET_MASK) >> 6;
    }

    i32 move_get_origin(i32 movecode) {
        return (movecode & MOVE_ORIGIN_MASK);
    }

    i32 get_rank(i32 location) {
        return (location & 0b111000) >> 3;
    }

    i32 get_file(i32 location) {
        return (location & 0b000111);
    }

    i32 get_location(i32 file, i32 rank) {
        return (rank*8) + file;
    }

    bool move_is_en_passant(i32 movecode) {
        return (movecode & MOVE_FLAG_MASK) == MOVE_EN_PASSANT;
    }

    bool move_is_castling(i32 movecode) {
        auto flags = (movecode & MOVE_FLAG_MASK);
        return (flags == MOVE_KING_CASTLE) || (flags == MOVE_QUEEN_CASTLE);
    }

    bool move_is_promotion(i32 movecode) {
        return (movecode & (8 << 12));
    }

    bool pawn_has_moved(i32 piece, i32 location) {
        return (((piece & PIECE_WHITE) && (get_rank(location) != 1)) || ((piece & PIECE_BLACK) && (get_rank(location) != 6)));
    }

    bool is_location_on_board(i32 file, i32 rank) {
        return (rank < 8 && rank >= 0 && file < 8 && file >= 0);
    }
}