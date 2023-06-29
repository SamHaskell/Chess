#include "state.hpp"

namespace Chess
{
    static Wigner::Color COLOR_DARKGREEN = {0.25f, 0.4f, 0.25f, 1.0f};
    static Wigner::Color COLOR_OFFWHITE = {0.9f, 0.9f, 0.9f, 1.0f};
    static Wigner::Color COLOR_WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
    static Wigner::Color COLOR_BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    static Wigner::Color COLOR_RED_TINT = {1.0f, 0.6f, 0.1f, 0.5f};
    static Wigner::Color COLOR_BLUE_TINT = {0.2f, 0.2f, 1.0f, 0.3f};

    static int piece_values[5] = {
        1,
        5,
        3,
        3,
        8
    };

    std::unique_ptr<GameState> gamestate_create()
    {
        auto state = std::make_unique<GameState>();
        gamestate_load_textures(state);
        board_init(state);
        return state;
    }

    void gamestate_load_textures(const std::unique_ptr<GameState> &state)
    {
        state->PieceTextures[0] = Wigner::Texture2D::Create("assets/user/textures/w_pawn_2x.png");
        state->PieceTextures[1] = Wigner::Texture2D::Create("assets/user/textures/w_rook_2x.png");
        state->PieceTextures[2] = Wigner::Texture2D::Create("assets/user/textures/w_knight_2x.png");
        state->PieceTextures[3] = Wigner::Texture2D::Create("assets/user/textures/w_bishop_2x.png");
        state->PieceTextures[4] = Wigner::Texture2D::Create("assets/user/textures/w_queen_2x.png");
        state->PieceTextures[5] = Wigner::Texture2D::Create("assets/user/textures/w_king_2x.png");

        state->PieceTextures[6] = Wigner::Texture2D::Create("assets/user/textures/b_pawn_2x.png");
        state->PieceTextures[7] = Wigner::Texture2D::Create("assets/user/textures/b_rook_2x.png");
        state->PieceTextures[8] = Wigner::Texture2D::Create("assets/user/textures/b_knight_2x.png");
        state->PieceTextures[9] = Wigner::Texture2D::Create("assets/user/textures/b_bishop_2x.png");
        state->PieceTextures[10] = Wigner::Texture2D::Create("assets/user/textures/b_queen_2x.png");
        state->PieceTextures[11] = Wigner::Texture2D::Create("assets/user/textures/b_king_2x.png");
    }

    void gamestate_update(const std::unique_ptr<GameState> &state, f64 dt)
    {
    }

    void board_init(const std::unique_ptr<GameState> &state)
    {
        state->SelectedCell = {UNSELECTED_INDEX, UNSELECTED_INDEX};
        state->CurrentTeam = TEAM_WHITE;

        for (i32 i = 0; i < 8; i++)
        {
            for (i32 j = 0; j < 8; j++)
            {
                state->Pieces[i][j] = {PIECE_NONE, TEAM_NONE};
            }
        }

        for (i32 i = 0; i < 8; i++)
        {
            state->Pieces[i][1] = {PIECE_PAWN, TEAM_WHITE};
            state->Pieces[i][6] = {PIECE_PAWN, TEAM_BLACK};
        }

        state->Pieces[0][0] = {PIECE_ROOK, TEAM_WHITE};
        state->Pieces[1][0] = {PIECE_KNIGHT, TEAM_WHITE};
        state->Pieces[2][0] = {PIECE_BISHOP, TEAM_WHITE};
        state->Pieces[3][0] = {PIECE_QUEEN, TEAM_WHITE};
        state->Pieces[4][0] = {PIECE_KING, TEAM_WHITE};
        state->Pieces[5][0] = {PIECE_BISHOP, TEAM_WHITE};
        state->Pieces[6][0] = {PIECE_KNIGHT, TEAM_WHITE};
        state->Pieces[7][0] = {PIECE_ROOK, TEAM_WHITE};

        state->Pieces[0][7] = {PIECE_ROOK, TEAM_BLACK};
        state->Pieces[1][7] = {PIECE_KNIGHT, TEAM_BLACK};
        state->Pieces[2][7] = {PIECE_BISHOP, TEAM_BLACK};
        state->Pieces[3][7] = {PIECE_QUEEN, TEAM_BLACK};
        state->Pieces[4][7] = {PIECE_KING, TEAM_BLACK};
        state->Pieces[5][7] = {PIECE_BISHOP, TEAM_BLACK};
        state->Pieces[6][7] = {PIECE_KNIGHT, TEAM_BLACK};
        state->Pieces[7][7] = {PIECE_ROOK, TEAM_BLACK};
    }

    void board_render(const std::unique_ptr<GameState> &state, Wigner::SceneData scene)
    {
        f32 dx = state->BoardRect.Width / 8.0f;
        f32 dy = state->BoardRect.Height / 8.0f;

        for (i32 j = 0; j < 8; j++)
        {
            for (i32 i = 0; i < 8; i++)
            {
                Wigner::Color col = ((i + 7 * j) % 2 == 0) ? COLOR_DARKGREEN : COLOR_OFFWHITE;
                Wigner::draw_quad(scene, state->BoardRect.X + dx * i, state->BoardRect.Y + dy * j, dx, dy, col);
            }
        }

        if (state->SelectedCell.X == UNSELECTED_INDEX || state->SelectedCell.Y == UNSELECTED_INDEX)
        {
        }
        else
        {
            Wigner::draw_quad(
                scene,
                state->BoardRect.X + dx * state->SelectedCell.X,
                state->BoardRect.Y + dy * state->SelectedCell.Y,
                dx,
                dy,
                COLOR_RED_TINT);

            for (auto coord : state->HighlightedCells)
            {
                Wigner::draw_quad(
                    scene,
                    state->BoardRect.X + dx * coord.X,
                    state->BoardRect.Y + dy * coord.Y,
                    dx,
                    dy,
                    COLOR_BLUE_TINT);
            }
        }

        for (i32 i = 0; i < 8; i++)
        {
            for (i32 j = 0; j < 8; j++)
            {
                auto piece = state->Pieces[i][j];
                if (piece.Type != PIECE_NONE)
                {
                    f32 w, h, xoff, yoff;
                    h = (piece.Type == PIECE_PAWN) ? dy * 0.7f : dy * 0.8f;
                    w = h * state->PieceTextures[piece.Type + PIECE_NONE * piece.Team]->GetAspectRatio();
                    xoff = (dx - w) / 2.0f + (0.02 * w);
                    yoff = dy * 0.1f;

                    Wigner::draw_textured_quad(
                        scene,
                        state->PieceTextures[piece.Type + PIECE_NONE * piece.Team],
                        state->BoardRect.X + dx * i + xoff,
                        state->BoardRect.Y + dy * j + yoff,
                        w,
                        h,
                        COLOR_WHITE);
                }
            }
        }
    }

    void on_cell_select(const std::unique_ptr<GameState> &state, Coord cell)
    {
        GamePiece current_piece = state->Pieces[state->SelectedCell.X][state->SelectedCell.Y];
        GamePiece selected_piece = state->Pieces[cell.X][cell.Y];
        Team other_team = (Team)((state->CurrentTeam+1) % 2);

        // Check if player is making an existing valid move

        if (check_valid_move(state, cell)) {
            if (selected_piece.Team == other_team) {
                LOG_DEBUG("CAPTURED A PIECE");
            }

            if (current_piece.Type == PIECE_PAWN) {
                if ((current_piece.Team == TEAM_WHITE && cell.Y == 7) || (current_piece.Team == TEAM_BLACK && cell.Y == 0)) {
                    LOG_WARN("Pawn promotion not yet implemented, pawn should not exist at this location.");
                }
            }

            current_piece.FirstMove = false;
            state->Pieces[cell.X][cell.Y] = current_piece;
            state->Pieces[state->SelectedCell.X][state->SelectedCell.Y] = {PIECE_NONE, TEAM_NONE};
            
            state->SelectedCell = {UNSELECTED_INDEX, UNSELECTED_INDEX};
            state->HighlightedCells.clear();

            state->CurrentTeam = other_team;
            return;
        }
        
        // If not a valid move, deselect if clicking on an enemy piece or empty cell

        if (selected_piece.Team != state->CurrentTeam) {
            state->SelectedCell = {UNSELECTED_INDEX, UNSELECTED_INDEX};
            state->HighlightedCells.clear();
            return;
        }

        //

        state->SelectedCell = cell;
        state->HighlightedCells.clear();
        
        i32 y_dir = (selected_piece.Team == TEAM_WHITE) ? 1 : -1;
        GamePiece target;

        switch (selected_piece.Type) {
            case PIECE_NONE:
                LOG_WARN("Selected piece is of type NONE");
                break;
            case PIECE_PAWN:
                // BASIC FORWARD MOVES
                target = state->Pieces[cell.X][cell.Y + y_dir];
                if (target.Type == PIECE_NONE) {
                    state->HighlightedCells.push_back({cell.X, cell.Y + y_dir});
                    target = state->Pieces[cell.X][cell.Y + y_dir * 2];
                    if (target.Type == PIECE_NONE && selected_piece.FirstMove) {
                        state->HighlightedCells.push_back({cell.X, cell.Y + y_dir * 2});
                    }
                }
                // DIAGONAL CAPTURE MOVES
                if (cell.X != 7) {
                    target = state->Pieces[cell.X + 1][cell.Y + y_dir];
                    if (target.Type != PIECE_NONE && target.Team != selected_piece.Team) {
                        state->HighlightedCells.push_back({cell.X + 1, cell.Y + y_dir});
                    }
                }
                if (cell.X != 0) {
                    target = state->Pieces[cell.X - 1][cell.Y + y_dir];
                    if (target.Type != PIECE_NONE && target.Team != selected_piece.Team) {
                        state->HighlightedCells.push_back({cell.X - 1, cell.Y + y_dir});
                    }
                }
                break;
            case PIECE_ROOK:
                break;
            case PIECE_BISHOP:
                break;
            case PIECE_KNIGHT:
                break;
            case PIECE_QUEEN:
                break;
            case PIECE_KING:
                break;
        }

    }

    void on_cell_deselect(const std::unique_ptr<GameState> &state)
    {
        state->SelectedCell = {UNSELECTED_INDEX, UNSELECTED_INDEX};
        state->HighlightedCells.clear();
    }

    bool check_valid_move(const std::unique_ptr<GameState>& state, Coord cell)
    {
        for (Coord move : state->HighlightedCells) {
            if (move.X == cell.X && move.Y == cell.Y) {
                return true;
            }
        }
        return false;
    }
}
