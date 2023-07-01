#include "board.hpp"

namespace Chess {
    static Wigner::Color COLOR_DARKGREEN = {0.3f, 0.4f, 0.3f, 1.0f};
    static Wigner::Color COLOR_OFFWHITE = {0.9f, 0.9f, 0.9f, 1.0f};
    static Wigner::Color COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
    static Wigner::Color COLOR_BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    static Wigner::Color COLOR_ORANGE = {0.8f, 0.6f, 0.1f, 1.0f};
    static Wigner::Color COLOR_RED_TINT = {1.0f, 0.2f, 0.2f, 0.2f};
    static Wigner::Color COLOR_BLUE_TINT = {0.2f, 0.2f, 1.0f, 0.3f};

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

    bool is_location_on_board(i32 file, i32 rank) {
        return (rank < 8 && rank >= 0 && file < 8 && file >= 0);
    }

    bool are_opponents(i32 piece, i32 other) {
        return ((piece | other) & PIECE_COLOR_MASK) == PIECE_COLOR_MASK;
    }

    bool pawn_has_moved(i32 piece, i32 location) {
        return (((piece & PIECE_WHITE) && (get_rank(location) != 1)) || ((piece & PIECE_BLACK) && (get_rank(location) != 6)));
    }

    void enumerate_moves(const std::unique_ptr<Board>& board) {
        board->LegalMoves.clear(); //SLOW BECAUSE WILL HAVE TO HEAP-REALLOCATE AS MOVELIST GROWS, TEMPORARY
        for (i32 origin = 0; origin < 64; origin++) {
            i32 piece = board->Cells[origin];
            if (piece & board->CurrentColour) {
                i32 piece_type = (piece & PIECE_TYPE_MASK);
                switch (piece_type) {
                    case PIECE_PAWN:
                        emplace_pawn_moves(board, board->LegalMoves, origin, piece);
                        break;
                    case PIECE_KNIGHT:
                        emplace_knight_moves(board, board->LegalMoves, origin, piece);
                        break;
                    case PIECE_ROOK:
                        emplace_orthogonal_sliding_moves(board, board->LegalMoves, origin, piece);
                        break;
                    case PIECE_BISHOP:
                        emplace_diagonal_sliding_moves(board, board->LegalMoves, origin, piece);
                        break;
                    case PIECE_QUEEN:
                        emplace_orthogonal_sliding_moves(board, board->LegalMoves, origin, piece);
                        emplace_diagonal_sliding_moves(board, board->LegalMoves, origin, piece);
                        break;
                    case PIECE_KING:
                        emplace_king_moves(board, board->LegalMoves, origin, piece);
                }
            }
        }
    }

    void update_attacked_cells(const std::unique_ptr<Board>& board) {
        board->AttackedCells.clear();
        for (i32 move : board->LegalMoves) {
            board->AttackedCells.insert(move_get_target(move));
        }
    }

    void update_highlighted_moves(const std::unique_ptr<Board>& board) {
        board->HighlightedMoves.clear();
        for (i32 move : board->LegalMoves) {
            if (move_get_origin(move) == board->SelectedCell) {
                board->HighlightedMoves.insert(move);
            }
        }
    }

    void emplace_pawn_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 forward = (piece & PIECE_WHITE) ? 8 : -8;
        i32 target = origin + forward;
        i32 file = get_file(origin);
        i32 rank = get_rank(origin);

        //SINGLE STEP
        if (!board->Cells[target]) {
            if (get_rank(target) % 7 == 0) {
                moves.push_back(move_create(origin, target, MOVE_PROMOTE_BISHOP));
                moves.push_back(move_create(origin, target, MOVE_PROMOTE_KNIGHT));
                moves.push_back(move_create(origin, target, MOVE_PROMOTE_ROOK)); 
                moves.push_back(move_create(origin, target, MOVE_PROMOTE_QUEEN));                
            } else {
                moves.push_back(move_create(origin, target));
            }
            //DOUBLE STEP ONLY POSSIBLE IF SINGLE WAS
            if (!pawn_has_moved(piece, origin)) {
                target = origin + (2*forward);
                if (!board->Cells[target]) {
                    moves.push_back(move_create(origin, target, MOVE_PAWN_DOUBLE_STEP));
                }
            }
        }

        //DIAGONAL CAPTURE
        if (file != 0) {
            target = origin + forward - 1;
            if (are_opponents(piece, board->Cells[target])) {
                if (get_rank(target) % 7 == 0) {
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_BISHOP));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_KNIGHT));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_ROOK)); 
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_QUEEN));                
                } else {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
        }  
        if (file != 7) {
            target = origin + forward + 1;
            if (are_opponents(piece, board->Cells[target])) {
                if (get_rank(target) % 7 == 0) {
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_BISHOP));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_KNIGHT));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_ROOK)); 
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_QUEEN));                
                } else {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
        }
        // TODO: EN PASSANT
    }

    void emplace_knight_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 file = get_file(origin);
        i32 rank = get_rank(origin);

        i32 ranks[8] = { // DEFINITELY A BETTER WAY BUT I DONT CARE
            rank + 1, rank + 1,
            rank - 1, rank - 1,
            rank + 2, rank + 2,
            rank - 2, rank - 2
        };

        i32 files[8] = {
            file + 2, file - 2,
            file + 2, file - 2,
            file + 1, file - 1,
            file + 1, file - 1
        };   

        for (i32 i = 0; i < 8; i++) {
            if (is_location_on_board(files[i], ranks[i])) {
                i32 target = get_location(files[i], ranks[i]);
                if (!board->Cells[target]) {
                    moves.push_back(move_create(origin, target));
                } else if (are_opponents(piece, board->Cells[target])) {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
        }
    }

    void emplace_orthogonal_sliding_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 fileshifts[4] = {1, -1, 0, 0};
        i32 rankshifts[4] = {0, 0, 1, -1};
        
        for (i32 i = 0; i < 4; i++) {
            bool obstructed = false;
            i32 file = get_file(origin);
            i32 rank = get_rank(origin);
            i32 target = origin;
            while (!obstructed) {
                file += fileshifts[i];
                rank += rankshifts[i];
                if (is_location_on_board(file, rank)) {
                    if (are_opponents(piece, board->Cells[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!board->Cells[get_location(file, rank)]) {
                        moves.push_back(move_create(origin, get_location(file, rank)));
                    } else {
                        obstructed = true;
                    }
                } else {
                    obstructed = true;
                }
            }
        }
    }

    void emplace_diagonal_sliding_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 fileshifts[4] = {1, -1, 1, -1};
        i32 rankshifts[4] = {1, -1, -1, 1};
        
        for (i32 i = 0; i < 4; i++) {
            bool obstructed = false;
            i32 file = get_file(origin);
            i32 rank = get_rank(origin);
            i32 target = origin;
            while (!obstructed) {
                file += fileshifts[i];
                rank += rankshifts[i];
                if (is_location_on_board(file, rank)) {
                    if (are_opponents(piece, board->Cells[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!board->Cells[get_location(file, rank)]) {
                        moves.push_back(move_create(origin, get_location(file, rank)));
                    } else {
                        obstructed = true;
                    }
                } else {
                    obstructed = true;
                }
            }
        }
    }

    void emplace_king_moves(const std::unique_ptr<Board>& board, std::vector<i32>& moves, i32 origin, i32 piece) {
        for (i32 file = get_file(origin) - 1; file < get_file(origin) + 2; file++) {
            for (i32 rank = get_rank(origin) - 1; rank < get_rank(origin) + 2; rank++) {
                if (is_location_on_board(file, rank)) {
                    if (are_opponents(piece, board->Cells[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                    } else if (!board->Cells[get_location(file, rank)]) {
                        moves.push_back(move_create(origin, get_location(file, rank)));
                    }
                }
            }
        }
    }

    std::unique_ptr<Board> board_create_default() {
        auto board = std::make_unique<Board>();
        board_load_textures(board);

        board->Cells[0] = PIECE_ROOK | PIECE_WHITE;
        board->Cells[1] = PIECE_KNIGHT | PIECE_WHITE;
        board->Cells[2] = PIECE_BISHOP | PIECE_WHITE;
        board->Cells[3] = PIECE_QUEEN | PIECE_WHITE;
        board->Cells[4] = PIECE_KING | PIECE_WHITE;
        board->Cells[5] = PIECE_BISHOP | PIECE_WHITE;
        board->Cells[6] = PIECE_KNIGHT | PIECE_WHITE;
        board->Cells[7] = PIECE_ROOK | PIECE_WHITE;

        board->Cells[56] = PIECE_ROOK | PIECE_BLACK;
        board->Cells[57] = PIECE_KNIGHT | PIECE_BLACK;
        board->Cells[58] = PIECE_BISHOP | PIECE_BLACK;
        board->Cells[59] = PIECE_QUEEN | PIECE_BLACK;
        board->Cells[60] = PIECE_KING | PIECE_BLACK;
        board->Cells[61] = PIECE_BISHOP | PIECE_BLACK;
        board->Cells[62] = PIECE_KNIGHT | PIECE_BLACK;
        board->Cells[63] = PIECE_ROOK | PIECE_BLACK;

        for (i32 i = 0; i < 8; i++) {
            board->Cells[i + 8] = PIECE_PAWN | PIECE_WHITE;
            board->Cells[i + 48] = PIECE_PAWN | PIECE_BLACK;
        }

        board->SelectedCell = BOARD_INVALID_CELL;

        //PLAY A FAKE "FIRST MOVE" TO GENERATE INFO
        board->CurrentColour = PIECE_BLACK;
        board_on_turn_end(board);

        return board;
    }

    void board_render(const std::unique_ptr<Board>& board, Wigner::SceneData scene) {
        f32 dx = board->DrawRect.Width / 8.0f;
        f32 dy = board->DrawRect.Height / 8.0f;

        for (i32 j = 0; j < 8; j++) {
            for (i32 i = 0; i < 8; i++) {
                Wigner::Color col = ((i + j) % 2) ? COLOR_OFFWHITE : COLOR_DARKGREEN;
                Wigner::draw_quad(scene, board->DrawRect.X + dx * i, board->DrawRect.Y + dy * j, dx, dy, col);
            }
        }
        
        if (board->SelectedCell != BOARD_INVALID_CELL) {
            Wigner::draw_quad(scene, board->DrawRect.X + dx * get_file(board->SelectedCell), board->DrawRect.Y + dy * get_rank(board->SelectedCell), dx, dy, COLOR_ORANGE);
        }

        for (i32 target : board->AttackedCells) {
            Wigner::draw_quad(scene, board->DrawRect.X + dx * get_file(target), board->DrawRect.Y + dy * get_rank(target), dx, dy, COLOR_RED_TINT);
        }

        for (i32 move : board->HighlightedMoves) {
            i32 target = move_get_target(move);
            Wigner::draw_quad(scene, board->DrawRect.X + dx * get_file(target), board->DrawRect.Y + dy * get_rank(target), dx, dy, COLOR_BLUE_TINT);
        }

        for (i32 i = 0; i < 8; i++) {
            for (i32 j = 0; j < 8; j++) {
                i32 piece = board->Cells[j*8 + i];
                if (piece) {
                    auto tex = board->TextureArray[(piece & PIECE_TYPE_MASK) - 1 + (6 * (bool)(piece & PIECE_BLACK))];
                    f32 w, h, xoff, yoff;
                    h = (true) ? dy * 0.7f : dy * 0.8f;
                    w = h * tex->GetAspectRatio();
                    xoff = (dx - w) / 2.0f + (0.02 * w);
                    yoff = dy * 0.1f;

                    Wigner::draw_textured_quad(
                        scene,
                        tex,
                        board->DrawRect.X + dx * i + xoff,
                        board->DrawRect.Y + dy * j + yoff,
                        w,
                        h,
                        COLOR_WHITE);
                }
            }
        }
    }

    void board_load_textures(const std::unique_ptr<Board>& board) {
        board->TextureArray[0] = Wigner::Texture2D::Create("assets/user/textures/w_pawn_2x.png");
        board->TextureArray[1] = Wigner::Texture2D::Create("assets/user/textures/w_rook_2x.png");
        board->TextureArray[2] = Wigner::Texture2D::Create("assets/user/textures/w_knight_2x.png");
        board->TextureArray[3] = Wigner::Texture2D::Create("assets/user/textures/w_bishop_2x.png");
        board->TextureArray[4] = Wigner::Texture2D::Create("assets/user/textures/w_queen_2x.png");
        board->TextureArray[5] = Wigner::Texture2D::Create("assets/user/textures/w_king_2x.png");
        board->TextureArray[6] = Wigner::Texture2D::Create("assets/user/textures/b_pawn_2x.png");
        board->TextureArray[7] = Wigner::Texture2D::Create("assets/user/textures/b_rook_2x.png");
        board->TextureArray[8] = Wigner::Texture2D::Create("assets/user/textures/b_knight_2x.png");
        board->TextureArray[9] = Wigner::Texture2D::Create("assets/user/textures/b_bishop_2x.png");
        board->TextureArray[10] = Wigner::Texture2D::Create("assets/user/textures/b_queen_2x.png");
        board->TextureArray[11] = Wigner::Texture2D::Create("assets/user/textures/b_king_2x.png");
    }

    void board_on_cell_deselect(const std::unique_ptr<Board>& board) {
        board->SelectedCell = BOARD_INVALID_CELL;
        board->HighlightedMoves.clear();
    }

    void board_on_cell_select(const std::unique_ptr<Board>& board, i32 file, i32 rank) {
        if ((file == get_file(board->SelectedCell)) && (rank == get_rank(board->SelectedCell))) {
            board_on_cell_deselect(board);
            return;
        } 
        if (board->Cells[get_location(file, rank)] & board->CurrentColour) {
            board->SelectedCell = get_location(file, rank);
            update_highlighted_moves(board);
            return;
        }
        for (i32 move : board->HighlightedMoves) {
            if (get_location(file, rank) == move_get_target(move)) {
                board_on_move(board, move);
                return;
            }
        }
        if (!board->Cells[get_location(file, rank)]) {
            board_on_cell_deselect(board);
            return;
        }
    }

    void board_on_move(const std::unique_ptr<Board>& board, i32 move) {
        LOG_INFO("THATS A VALID MOVE: NICE!");
        i32 piece = board->Cells[move_get_target(move)];
        board->Cells[move_get_target(move)] = board->Cells[move_get_origin(move)];
        board->Cells[move_get_origin(move)] = PIECE_NONE;
        board_on_cell_deselect(board);
        board_on_turn_end(board);
    }

    void board_on_turn_end(const std::unique_ptr<Board>& board) {
        enumerate_moves(board);
        update_attacked_cells(board);
        board->CurrentColour = (board->CurrentColour & PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
        enumerate_moves(board);
    }
}