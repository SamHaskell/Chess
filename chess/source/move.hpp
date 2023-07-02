#pragma once

#include <Wigner.hpp>

#include <memory> 
#include <array> 
#include <set>

namespace Chess {
    enum Moves {
        MOVE_NONE = 0,
        MOVE_PAWN_DOUBLE_STEP = (1 << 12),
        MOVE_KING_CASTLE = (2 << 12),
        MOVE_QUEEN_CASTLE = (3 << 12),
        MOVE_CAPTURE = (4 << 12),
        MOVE_EN_PASSANT = (5 << 12),
        MOVE_PROMOTE_KNIGHT = (8 << 12),
        MOVE_PROMOTE_BISHOP = (9 << 12),
        MOVE_PROMOTE_ROOK = (10 << 12),
        MOVE_PROMOTE_QUEEN = (11 << 12),
        MOVE_PROMOTE_CAPTURE_KNIGHT = (12 << 12),
        MOVE_PROMOTE_CAPTURE_BISHOP = (13 << 12),
        MOVE_PROMOTE_CAPTURE_ROOK = (14 << 12),
        MOVE_PROMOTE_CAPTURE_QUEEN = (15 << 12),

        MOVE_ORIGIN_MASK = 0b0000000000111111,
        MOVE_TARGET_MASK = 0b0000111111000000,
        MOVE_FLAG_MASK = 0b1111000000000000,
    };

    i32 move_create(i32 origin, i32 target);
    i32 move_create(i32 origin, i32 target, i32 flags);

    i32 move_get_target(i32 move);
    i32 move_get_origin(i32 move);

    i32 get_rank(i32 location);
    i32 get_file(i32 location);
    i32 get_location(i32 file, i32 rank);

    bool move_is_en_passant(i32 move);
    bool move_is_castling(i32 move);
    bool move_is_promotion(i32 move);

    bool is_location_on_board(i32 file, i32 rank);
}