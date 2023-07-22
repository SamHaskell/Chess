#include "position.hpp"
#include "piece.hpp"
#include "move.hpp"

#include <sstream>

namespace Chess {
    static const std::string default_game_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Position position_read(const std::string& fen) {
        Position position = {};
        std::vector<std::string> parts;
        std::istringstream iss(fen);
        std::string word;

        while (std::getline(iss, word, ' ')) {
            parts.push_back(word);
        }

        if (parts.size() != 6) { LOG_ERROR("Failure to parse invalid FEN string %s.", fen.c_str()); return position; }

        // Piece Placement
        i32 rank = 7;
        i32 file = 0;
        for (i32 i = 0; i < parts[0].length(); i++) {
            
            if (parts[0][i] == '/') {
                rank --;
                file = 0;
                i++;
            }

            if (!is_location_on_board(file, rank)) { LOG_ERROR("Failure to parse invalid FEN string %s at char %i", fen.c_str(), i); return position; }
            
            if (isdigit(parts[0][i])) {
                file += parts[0][i] - '0';
            } else {
                i32 piece;
                if (piece_from_char(parts[0][i], piece)) {
                    position.Board[get_location(file, rank)] = piece;
                    file ++;
                } else {
                    LOG_ERROR("Failure to parse invalid FEN string %s at char %i", fen.c_str(), i); 
                    return position;
                }
            }
        }

        // Player To Move
        if (parts[1].length() != 1) { LOG_ERROR("Failure to parse invalid FEN string %s. (player)", fen.c_str()); return position; }
        if (parts[1][0] == 'w') { 
            position.Player = PIECE_WHITE;
        } else if (parts[1][0] == 'b') {
            position.Player = PIECE_BLACK;
        } else {
            LOG_ERROR("Failure to parse invalid FEN string %s. (player)", fen.c_str()); return position; 
            return position;
        }

        // Castling Rights
        if ((parts[2][0] == '-') && (parts[2].length() == 1)) {
            position.EnPassantTarget = BOARD_INVALID_CELL;
        } else {
            for (i32 i = 0; i < parts[2].length(); i++) {
                if (i > 3) { LOG_ERROR("Failure to parse invalid FEN string %s. (castling)", fen.c_str()); return position; }
                switch (parts[2][i]) {
                    case 'K':
                        position.CastlingFlags |= CASTLE_WHITE_KING;
                        break;
                    case 'Q':
                        position.CastlingFlags |= CASTLE_WHITE_QUEEN;
                        break;
                    case 'k':
                        position.CastlingFlags |= CASTLE_BLACK_KING;
                        break;
                    case 'q':
                        position.CastlingFlags |= CASTLE_BLACK_QUEEN;
                        break;
                    default:
                        LOG_ERROR("Failure to parse invalid FEN string %s.", fen.c_str());
                        return position;
                }
            }
        }

        // En Passant Target
        if ((parts[3][0] == '-') && (parts[3].length() == 1)) {
            position.EnPassantTarget = BOARD_INVALID_CELL;
        } else if (parts[3].length() == 2) {
            i32 ep_file = parts[3][0] - 'a';
            i32 ep_rank = parts[3][1] - '0';
            if (is_location_on_board(ep_file, ep_rank)) {
                position.EnPassantTarget = get_location(ep_file, ep_rank);
            }
        } else {
            position.EnPassantTarget = BOARD_INVALID_CELL;
            LOG_ERROR("Failure to parse invalid FEN string %s. (en passant)", fen.c_str());
            return position;
        }

        // Halfmove Clock
        position.HalfmoveClock = 0;
        for (u8 c : parts[4]) {
            if (!isdigit(c)) {
                position.HalfmoveClock = 0;
                LOG_ERROR("Failure to parse invalid FEN string %s. (halfmove)", fen.c_str());
                return position;
            }
            position.HalfmoveClock *= 10; 
            position.HalfmoveClock += c - '0';
        }

        // Fullmove Counter
        position.FullmoveCounter = 0;
        for (u8 c : parts[5]) {
            if (!isdigit(c)) {
                position.FullmoveCounter = 0;
                LOG_ERROR("Failure to parse invalid FEN string %s. (fullmove)", fen.c_str());
                return position;
            }
            position.FullmoveCounter *= 10; 
            position.FullmoveCounter += c - '0';
        }

        LOG_INFO("Number of available moves to depth 3: %i", position_perft(position, 3));

        return position;
    }

    std::string position_write(Position position) {
        return "Writing positions to FEN strings not yet implemented.";
    }

    Position position_default() {
        return position_read(default_game_fen);
    }

    Position position_apply_move(Position position, i32 move) {
        i32 piece = position.Board[move_get_target(move)];
        position.Board[move_get_target(move)] = position.Board[move_get_origin(move)];
        position.Board[move_get_origin(move)] = PIECE_NONE;
        if (move_is_en_passant(move)) {
            auto capture_location = get_location(get_file(position.EnPassantTarget), get_rank(move_get_origin(move)));
            position.Board[capture_location] = PIECE_NONE;
        }
        position.EnPassantTarget = BOARD_INVALID_CELL;
        if ((move & MOVE_FLAG_MASK) == MOVE_PAWN_DOUBLE_STEP) {
            position.EnPassantTarget = (move_get_target(move) + move_get_origin(move)) / 2;
        }
        if (move_is_castling(move)) {
            
        }
        position.Player = (position.Player & PIECE_WHITE) ? PIECE_BLACK : PIECE_WHITE;
        return position;
    }

    std::vector<i32> position_generate_moves(Position position) {
        std::vector<i32> moves;
        std::set<i32> attacked_cells;
        for (i32 origin = 0; origin < 64; origin++) {
            i32 piece = position.Board[origin];
            if (piece & position.Player) {
                i32 piece_type = (piece & PIECE_TYPE_MASK);
                switch (piece_type) {
                    case PIECE_PAWN:
                        position_generate_pawn_moves(position, moves, origin, piece);
                        break;
                    case PIECE_KNIGHT:
                        position_generate_knight_moves(position, moves, origin, piece);
                        break;
                    case PIECE_ROOK:
                        position_generate_orthogonal_sliding_moves(position, moves, origin, piece);
                        break;
                    case PIECE_BISHOP:
                        position_generate_diagonal_sliding_moves(position, moves, origin, piece);
                        break;
                    case PIECE_QUEEN:
                        position_generate_orthogonal_sliding_moves(position, moves, origin, piece);
                        position_generate_diagonal_sliding_moves(position, moves, origin, piece);
                        break;
                    case PIECE_KING:
                        position_generate_king_moves(position, moves, origin, piece);
                }
            }
        }
        return moves;
    }

    void position_generate_pawn_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 forward = (piece & PIECE_WHITE) ? 8 : -8;
        i32 target = origin + forward;
        i32 file = get_file(origin);
        i32 rank = get_rank(origin);

        //SINGLE STEP
        if (!position.Board[target]) {
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
                target = origin + 2*forward;
                if (!position.Board[target]) {
                    moves.push_back(move_create(origin, target, MOVE_PAWN_DOUBLE_STEP));
                }
            }
        }

        //DIAGONAL CAPTURE
        if (file != 0) {
            target = origin + forward - 1;
            if (are_opponents(piece, position.Board[target])) {
                if (get_rank(target) % 7 == 0) {
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_BISHOP));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_KNIGHT));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_ROOK)); 
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_QUEEN));                
                } else {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
            if (target == position.EnPassantTarget) {
                moves.push_back(move_create(origin, target, MOVE_EN_PASSANT));
            }
        }  
        if (file != 7) {
            target = origin + forward + 1;
            if (are_opponents(piece, position.Board[target])) {
                if (get_rank(target) % 7 == 0) {
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_BISHOP));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_KNIGHT));
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_ROOK)); 
                    moves.push_back(move_create(origin, target, MOVE_PROMOTE_CAPTURE_QUEEN));                
                } else {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
            if (target == position.EnPassantTarget) {
                moves.push_back(move_create(origin, target, MOVE_EN_PASSANT));
            }
        }
    }

    void position_generate_knight_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece) {
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
                if (!position.Board[target]) {
                    moves.push_back(move_create(origin, target));
                } else if (are_opponents(piece, position.Board[target])) {
                    moves.push_back(move_create(origin, target, MOVE_CAPTURE));
                }
            }
        }
    }

    void position_generate_orthogonal_sliding_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece) {
        i32 fileshifts[8] = {1, -1, 0, 0, 1, -1, 1, -1};
        i32 rankshifts[8] = {0, 0, 1, -1, 1, -1, -1, 1};
        
        i32 start = (!can_slide_orthogonal(piece)) ? 4 : 0;
        i32 end = (!can_slide_diagonal(piece)) ? 4 : 8;
        
        for (i32 i = start; i < end; i++) {
            bool obstructed = false;
            i32 file = get_file(origin);
            i32 rank = get_rank(origin);
            i32 target = origin;
            while (!obstructed) {
                file += fileshifts[i];
                rank += rankshifts[i];
                if (is_location_on_board(file, rank)) {
                    if (are_opponents(piece, position.Board[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!position.Board[get_location(file, rank)]) {
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

    void position_generate_diagonal_sliding_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece) {
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
                    if (are_opponents(piece, position.Board[get_location(file, rank)])) {
                        moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                        obstructed = true;
                    } else if (!position.Board[get_location(file, rank)]) {
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

    void position_generate_king_moves(const Position& position, std::vector<i32>& moves, i32 origin, i32 piece) {
        for (i32 file = get_file(origin) - 1; file < get_file(origin) + 2; file++) {
            for (i32 rank = get_rank(origin) - 1; rank < get_rank(origin) + 2; rank++) {
                if (!is_location_on_board(file, rank)) { continue; }
                if (are_opponents(piece, position.Board[get_location(file, rank)])) {
                    moves.push_back(move_create(origin, get_location(file, rank), MOVE_CAPTURE));
                } else if (!position.Board[get_location(file, rank)]) {
                    moves.push_back(move_create(origin, get_location(file, rank)));
                }
            }
        }

        // CASTLING
        if (position.Player & PIECE_WHITE) {
            if (position.CastlingFlags & CASTLE_WHITE_KING) {
                // WHITE KING SIDE
            } else if (position.CastlingFlags & CASTLE_WHITE_QUEEN) {
                // WHITE QUEEN SIDE
            }
        } else {
            if (position.CastlingFlags & CASTLE_BLACK_KING) {
                // BLACK KING SIDE
            } else if (position.CastlingFlags & CASTLE_BLACK_QUEEN) {
                // BLACK QUEEN SIDE
            }
        }
    }

    i32 position_perft(Position position, i32 depth) {
        i32 count = 0;
        if (depth == 1) {
            return position_generate_moves(position).size();
        }

        for (i32 move : position_generate_moves(position)) {
            count += position_perft(position_apply_move(position, move), depth - 1);
        }
        return count;
    }
}