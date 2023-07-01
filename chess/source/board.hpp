#pragma once

#include <Wigner.hpp>

#include <memory> 
#include <array> 

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

    enum MoveFlags {
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
        MOVE_FLAG_MASK = 0b111100000000000,
    };

    struct Board {
        i32 Cells[64];
        i32 CurrentColour;
        i32 SelectedCell;
        Wigner::Rect2D DrawRect;
        std::array<std::shared_ptr<Wigner::Texture2D>, 12> TextureArray;
    };

    constexpr i32 BOARD_INVALID_CELL = 64;

    i32 move_create(i32 origin, i32 target);
    i32 move_create(i32 origin, i32 target, i32 flags);

    i32 move_get_target(i32 movecode);
    i32 move_get_origin(i32 movecode);

    i32 get_rank(i32 location);
    i32 get_file(i32 location);

    bool move_is_en_passant(i32 movecode);
    bool move_is_castling(i32 movecode);
    bool move_is_promotion(i32 movecode);

    bool are_opponents(i32 piece, i32 other);
    bool pawn_has_moved(i32 piece, i32 location);

    std::vector<i32> enumerate_moves(const std::unique_ptr<Board>& board);
    void emplace_pawn_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_knight_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_orthogonal_sliding_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_diagonal_sliding_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece);
    void emplace_king_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece);

    std::unique_ptr<Board> board_create_default();
    void board_load_textures(const std::unique_ptr<Board>& board);
    void board_render(const std::unique_ptr<Board>& board, Wigner::SceneData scene);
}