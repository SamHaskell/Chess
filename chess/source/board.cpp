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

    void enumerate_moves(const std::unique_ptr<GameData>& state) {
        state->LegalMoves.clear(); //SLOW BECAUSE WILL HAVE TO HEAP-REALLOCATE AS MOVELIST GROWS, TEMPORARY
        for (i32 origin = 0; origin < 64; origin++) {
            i32 piece = state->CurrentBoard[origin];
            if (piece & state->CurrentPlayer) {
                i32 piece_type = (piece & PIECE_TYPE_MASK);
                switch (piece_type) {
                    case PIECE_PAWN:
                        emplace_pawn_moves(state, state->LegalMoves, origin, piece);
                        break;
                    case PIECE_KNIGHT:
                        emplace_knight_moves(state, state->LegalMoves, origin, piece);
                        break;
                    case PIECE_ROOK:
                        emplace_orthogonal_sliding_moves(state, state->LegalMoves, origin, piece);
                        break;
                    case PIECE_BISHOP:
                        emplace_diagonal_sliding_moves(state, state->LegalMoves, origin, piece);
                        break;
                    case PIECE_QUEEN:
                        emplace_orthogonal_sliding_moves(state, state->LegalMoves, origin, piece);
                        emplace_diagonal_sliding_moves(state, state->LegalMoves, origin, piece);
                        break;
                    case PIECE_KING:
                        emplace_king_moves(state, state->LegalMoves, origin, piece);
                }
            }
        }
    }

    void update_attacked_cells(const std::unique_ptr<GameData>& state) {
        state->AttackedCells.clear();
        for (i32 move : state->LegalMoves) {
            state->AttackedCells.insert(move_get_target(move));
        }
    }

    void update_highlighted_moves(const std::unique_ptr<GameData>& state) {
        state->HighlightedMoves.clear();
        for (i32 move : state->LegalMoves) {
            if (move_get_origin(move) == state->SelectedCell) {
                state->HighlightedMoves.insert(move);
            }
        }
    }

    void emplace_pawn_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 forward = (piece & PIECE_WHITE) ? 8 : -8;
        i32 target = origin + forward;
        i32 file = get_file(origin);
        i32 rank = get_rank(origin);

        //SINGLE STEP
        if (!state->CurrentBoard[target]) {
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
                if (!state->CurrentBoard[target]) {
                    moves.push_back(move_create(origin, target, MOVE_PAWN_DOUBLE_STEP));
                }
            }
        }

        //DIAGONAL CAPTURE
        if (file != 0) {
            target = origin + forward - 1;
            if (are_opponents(piece, state->CurrentBoard[target])) {
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
            if (are_opponents(piece, state->CurrentBoard[target])) {
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

    void emplace_knight_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece) {
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
                if (!state->CurrentBoard[target]) {
                    moves.push_back(move_create(origin, target));
                } else if (are_opponents(piece, state->CurrentBoard[target])) {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
        }
    }

    void emplace_orthogonal_sliding_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece) {
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
                    if (are_opponents(piece, state->CurrentBoard[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!state->CurrentBoard[get_location(file, rank)]) {
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

    void emplace_diagonal_sliding_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece) {
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
                    if (are_opponents(piece, state->CurrentBoard[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!state->CurrentBoard[get_location(file, rank)]) {
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

    void emplace_king_moves(const std::unique_ptr<GameData>& state, std::vector<i32>& moves, i32 origin, i32 piece) {
        for (i32 file = get_file(origin) - 1; file < get_file(origin) + 2; file++) {
            for (i32 rank = get_rank(origin) - 1; rank < get_rank(origin) + 2; rank++) {
                if (is_location_on_board(file, rank)) {
                    if (are_opponents(piece, state->CurrentBoard[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                    } else if (!state->CurrentBoard[get_location(file, rank)]) {
                        moves.push_back(move_create(origin, get_location(file, rank)));
                    }
                }
            }
        }
    }

    std::unique_ptr<GameData> board_create_default() {
        auto state = std::make_unique<GameData>();
        board_load_textures(state);

        state->CurrentBoard[0] = PIECE_ROOK | PIECE_WHITE;
        state->CurrentBoard[1] = PIECE_KNIGHT | PIECE_WHITE;
        state->CurrentBoard[2] = PIECE_BISHOP | PIECE_WHITE;
        state->CurrentBoard[3] = PIECE_QUEEN | PIECE_WHITE;
        state->CurrentBoard[4] = PIECE_KING | PIECE_WHITE;
        state->CurrentBoard[5] = PIECE_BISHOP | PIECE_WHITE;
        state->CurrentBoard[6] = PIECE_KNIGHT | PIECE_WHITE;
        state->CurrentBoard[7] = PIECE_ROOK | PIECE_WHITE;

        state->CurrentBoard[56] = PIECE_ROOK | PIECE_BLACK;
        state->CurrentBoard[57] = PIECE_KNIGHT | PIECE_BLACK;
        state->CurrentBoard[58] = PIECE_BISHOP | PIECE_BLACK;
        state->CurrentBoard[59] = PIECE_QUEEN | PIECE_BLACK;
        state->CurrentBoard[60] = PIECE_KING | PIECE_BLACK;
        state->CurrentBoard[61] = PIECE_BISHOP | PIECE_BLACK;
        state->CurrentBoard[62] = PIECE_KNIGHT | PIECE_BLACK;
        state->CurrentBoard[63] = PIECE_ROOK | PIECE_BLACK;

        for (i32 i = 0; i < 8; i++) {
            state->CurrentBoard[i + 8] = PIECE_PAWN | PIECE_WHITE;
            state->CurrentBoard[i + 48] = PIECE_PAWN | PIECE_BLACK;
        }

        state->SelectedCell = BOARD_INVALID_CELL;
        state->CurrentCastlingFlags = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN | CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN;

        //PLAY A FAKE "FIRST MOVE" TO GENERATE INFO
        state->CurrentPlayer = PIECE_BLACK;
        board_on_turn_end(state);

        return state;
    }

    void board_render(const std::unique_ptr<GameData>& state, Wigner::SceneData scene) {
        f32 dx = state->BoardRect.Width / 8.0f;
        f32 dy = state->BoardRect.Height / 8.0f;

        for (i32 j = 0; j < 8; j++) {
            for (i32 i = 0; i < 8; i++) {
                Wigner::Color col = ((i + j) % 2) ? COLOR_OFFWHITE : COLOR_DARKGREEN;
                Wigner::draw_quad(scene, state->BoardRect.X + dx * i, state->BoardRect.Y + dy * j, dx, dy, col);
            }
        }
        
        Wigner::Color selected_piece_color = Wigner::Color({0.8f, 0.6f, 0.1f, 0.6f + 0.4f * glm::pow(glm::sin(2.0*state->ElapsedTime), 2)});
        if (state->SelectedCell != BOARD_INVALID_CELL) {
            Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(state->SelectedCell), state->BoardRect.Y + dy * get_rank(state->SelectedCell), dx, dy, selected_piece_color);
        }

        for (i32 target : state->AttackedCells) {
            Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(target), state->BoardRect.Y + dy * get_rank(target), dx, dy, COLOR_RED_TINT);
        }

        for (i32 move : state->HighlightedMoves) {
            i32 target = move_get_target(move);
            Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(target), state->BoardRect.Y + dy * get_rank(target), dx, dy, COLOR_BLUE_TINT);
        }

        for (i32 i = 0; i < 8; i++) {
            for (i32 j = 0; j < 8; j++) {
                i32 piece = state->CurrentBoard[j*8 + i];
                if (piece) {
                    auto tex = state->TextureArray[(piece & PIECE_TYPE_MASK) - 1 + (6 * (bool)(piece & PIECE_BLACK))];
                    f32 w, h, xoff, yoff;
                    h = (true) ? dy * 0.7f : dy * 0.8f;
                    w = h * tex->GetAspectRatio();
                    xoff = (dx - w) / 2.0f + (0.02 * w);
                    yoff = dy * 0.1f;

                    Wigner::draw_textured_quad(
                        scene,
                        tex,
                        state->BoardRect.X + dx * i + xoff,
                        state->BoardRect.Y + dy * j + yoff,
                        w,
                        h,
                        COLOR_WHITE);
                }
            }
        }
    }

    void board_load_textures(const std::unique_ptr<GameData>& state) {
        state->TextureArray[0] = Wigner::Texture2D::Create("assets/user/textures/w_pawn_2x.png");
        state->TextureArray[1] = Wigner::Texture2D::Create("assets/user/textures/w_rook_2x.png");
        state->TextureArray[2] = Wigner::Texture2D::Create("assets/user/textures/w_knight_2x.png");
        state->TextureArray[3] = Wigner::Texture2D::Create("assets/user/textures/w_bishop_2x.png");
        state->TextureArray[4] = Wigner::Texture2D::Create("assets/user/textures/w_queen_2x.png");
        state->TextureArray[5] = Wigner::Texture2D::Create("assets/user/textures/w_king_2x.png");
        state->TextureArray[6] = Wigner::Texture2D::Create("assets/user/textures/b_pawn_2x.png");
        state->TextureArray[7] = Wigner::Texture2D::Create("assets/user/textures/b_rook_2x.png");
        state->TextureArray[8] = Wigner::Texture2D::Create("assets/user/textures/b_knight_2x.png");
        state->TextureArray[9] = Wigner::Texture2D::Create("assets/user/textures/b_bishop_2x.png");
        state->TextureArray[10] = Wigner::Texture2D::Create("assets/user/textures/b_queen_2x.png");
        state->TextureArray[11] = Wigner::Texture2D::Create("assets/user/textures/b_king_2x.png");
    }

    void board_on_cell_deselect(const std::unique_ptr<GameData>& state) {
        state->SelectedCell = BOARD_INVALID_CELL;
        state->HighlightedMoves.clear();
    }

    void board_on_cell_select(const std::unique_ptr<GameData>& state, i32 file, i32 rank) {
        if ((file == get_file(state->SelectedCell)) && (rank == get_rank(state->SelectedCell))) {
            board_on_cell_deselect(state);
            return;
        } 
        if (state->CurrentBoard[get_location(file, rank)] & state->CurrentPlayer) {
            state->SelectedCell = get_location(file, rank);
            update_highlighted_moves(state);
            return;
        }
        for (i32 move : state->HighlightedMoves) {
            if (get_location(file, rank) == move_get_target(move)) {
                board_on_move(state, move);
                return;
            }
        }
        if (!state->CurrentBoard[get_location(file, rank)]) {
            board_on_cell_deselect(state);
            return;
        }
    }

    void board_on_move(const std::unique_ptr<GameData>& state, i32 move) {
        LOG_INFO("THATS A VALID MOVE: NICE!");
        i32 piece = state->CurrentBoard[move_get_target(move)];
        state->CurrentBoard[move_get_target(move)] = state->CurrentBoard[move_get_origin(move)];
        state->CurrentBoard[move_get_origin(move)] = PIECE_NONE;
        board_on_cell_deselect(state);
        board_on_turn_end(state);
    }

    void board_on_turn_end(const std::unique_ptr<GameData>& state) {
        enumerate_moves(state);
        update_attacked_cells(state);
        state->CurrentPlayer = (state->CurrentPlayer & PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
        enumerate_moves(state);
    }
}