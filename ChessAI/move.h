#ifndef MOVE_H
#define MOVE_H

#include "chess.h"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \class:  Move
///
/// \brief:  Function container used for altering game state and determining valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
// TODO: make Move friends with Agent and ChessState so makeMove won't be called without a check for valid move
class Move
{
    static void getValidMovesPawn(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves);
    static void getValidMovesKnight(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves);
    static void getValidMovesLinear(const Color player, const PieceNode& piece, const ChessState& game, const std::vector<Position>& directions, std::vector<Position>& moves);
    static void getValidMovesKing(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves);
    static bool isUnderAttack(const Color player, const ChessState& game, const Position& position);
    static bool canCastle(const Color player, const PieceNode& piece, const ChessState& game, const bool kingSide);

public:
    static std::vector<Position> getValidMoves(const Color player, const PieceNode& piece, const ChessState& game);
    static bool isValidMove(const Color player, const PieceNode& piece, const Position& destination, const ChessState& game);
    static bool inCheck(const Color player, const ChessState& game);
    static void makeMove(const Color player, PieceNode& piece, const Position& destination, ChessState& game, const PieceType promotion = QUEEN);
};

#endif // MOVE_H