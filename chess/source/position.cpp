#include "position.hpp"
#include "piece.hpp"
#include "move.hpp"

#include <sstream>

namespace Chess {
    static const std::string default_game_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    Position read_position(const std::string& fen) {
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

        return position;
    }

    std::string write_position(Position position) {
        return "Writing positions to FEN strings not yet implemented.";
    }

    Position create_default_position() {
        return read_position(default_game_fen);
    }
}