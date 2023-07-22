#include "board.hpp"

namespace Chess {
    static Wigner::Color COLOR_DARKGREEN = {0.3f, 0.4f, 0.3f, 1.0f};
    static Wigner::Color COLOR_OFFWHITE = {0.9f, 0.9f, 0.9f, 1.0f};
    static Wigner::Color COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
    static Wigner::Color COLOR_BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    static Wigner::Color COLOR_ORANGE = {0.8f, 0.6f, 0.1f, 1.0f};
    static Wigner::Color COLOR_RED_TINT = {1.0f, 0.2f, 0.2f, 0.2f};
    static Wigner::Color COLOR_BLUE_TINT = {0.2f, 0.2f, 1.0f, 0.3f};

    void enumerate_moves(const std::unique_ptr<GameData>& state) {
        state->LegalMoves.clear(); //SLOW BECAUSE WILL HAVE TO HEAP-REALLOCATE AS MOVELIST GROWS, TEMPORARY
        for (i32 origin = 0; origin < 64; origin++) {
            i32 piece = state->PositionHistory.back().Board[origin];
            if (piece & state->PositionHistory.back().Player) {
                i32 piece_type = (piece & PIECE_TYPE_MASK);
                state->LegalMoves = position_generate_moves(state->PositionHistory.back());
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
        state->HighlightedCells.clear();
        for (i32 move : state->LegalMoves) {
            if (move_get_origin(move) == state->SelectedCell) {
                state->HighlightedMoves.push_back(move);
                state->HighlightedCells.insert(move_get_target(move));
            }
        }
    }

    std::unique_ptr<GameData> game_create_from_fen(const std::string& fen) {
        auto state = std::make_unique<GameData>();
        game_load_textures(state);

        state->PositionHistory.push_back(position_read(fen));
        state->SelectedCell = BOARD_INVALID_CELL;

        enumerate_moves(state);

        return state;
    }

    std::unique_ptr<GameData> game_create_default() {
        auto state = std::make_unique<GameData>();
        game_load_textures(state);

        state->PositionHistory.push_back(position_default());
        state->SelectedCell = BOARD_INVALID_CELL;

        enumerate_moves(state);

        return state;
    }

    void game_render(const std::unique_ptr<GameData>& state, Wigner::SceneData scene) {
        f32 dx = state->BoardRect.Width / 8.0f;
        f32 dy = state->BoardRect.Height / 8.0f;

        for (i32 j = 0; j < 8; j++) {
            for (i32 i = 0; i < 8; i++) {
                Wigner::Color col = ((i + j) % 2) ? COLOR_OFFWHITE : COLOR_DARKGREEN;
                Wigner::draw_quad(scene, state->BoardRect.X + dx * i, state->BoardRect.Y + dy * j, dx, dy, col);
            }
        }
        
        Wigner::Color selected_piece_color = Wigner::Color({0.8f, 0.6f, 0.1f, 1.0f});
        if (state->SelectedCell != BOARD_INVALID_CELL) {
            Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(state->SelectedCell), state->BoardRect.Y + dy * get_rank(state->SelectedCell), dx, dy, selected_piece_color);
        }

        // for (i32 target : state->AttackedCells) {
        //     Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(target), state->BoardRect.Y + dy * get_rank(target), dx, dy, COLOR_RED_TINT);
        // }

        for (i32 target : state->HighlightedCells) {
            Wigner::draw_quad(scene, state->BoardRect.X + dx * get_file(target), state->BoardRect.Y + dy * get_rank(target), dx, dy, COLOR_BLUE_TINT);
        }

        for (i32 i = 0; i < 8; i++) {
            for (i32 j = 0; j < 8; j++) {
                i32 piece = state->PositionHistory.back().Board[j*8 + i];
                if ((piece & PIECE_COLOR_MASK) && (piece & PIECE_TYPE_MASK)) {
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

    void game_load_textures(const std::unique_ptr<GameData>& state) {
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

    void game_on_cell_deselect(const std::unique_ptr<GameData>& state) {
        state->SelectedCell = BOARD_INVALID_CELL;
        state->HighlightedMoves.clear();
        state->HighlightedCells.clear();
    }

    void game_on_cell_select(const std::unique_ptr<GameData>& state, i32 file, i32 rank) {
        if ((file == get_file(state->SelectedCell)) && (rank == get_rank(state->SelectedCell))) {
            game_on_cell_deselect(state);
            return;
        } 
        if (state->PositionHistory.back().Board[get_location(file, rank)] & state->PositionHistory.back().Player) {
            state->SelectedCell = get_location(file, rank);
            update_highlighted_moves(state);
            return;
        }
        for (i32 move : state->HighlightedMoves) {
            if (get_location(file, rank) == move_get_target(move)) {
                game_on_move(state, move);
                return;
            }
        }
        if (!state->PositionHistory.back().Board[get_location(file, rank)]) {
            game_on_cell_deselect(state);
            return;
        }
    }

    void game_move(const std::unique_ptr<GameData>& state, i32 move) {
        i32 piece = state->PositionHistory.back().Board[move_get_target(move)];
        state->PositionHistory.back().Board[move_get_target(move)] = state->PositionHistory.back().Board[move_get_origin(move)];
        state->PositionHistory.back().Board[move_get_origin(move)] = PIECE_NONE;
        if (move_is_en_passant(move)) {
            auto capture_location = get_location(get_file(state->PositionHistory.back().EnPassantTarget), get_rank(move_get_origin(move)));
            state->PositionHistory.back().Board[capture_location] = PIECE_NONE;
        }
        state->PositionHistory.back().EnPassantTarget = BOARD_INVALID_CELL;
        if ((move & MOVE_FLAG_MASK) == MOVE_PAWN_DOUBLE_STEP) {
            state->PositionHistory.back().EnPassantTarget = (move_get_target(move) + move_get_origin(move)) / 2;
        }
    }

    void game_on_move(const std::unique_ptr<GameData>& state, i32 move) {
        game_move(state, move);
        game_on_cell_deselect(state);
        game_on_turn_end(state);
    }

    void game_on_turn_end(const std::unique_ptr<GameData>& state) {
        enumerate_moves(state);
        update_attacked_cells(state);
        state->PositionHistory.back().Player = (state->PositionHistory.back().Player & PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
        enumerate_moves(state);
    }
}