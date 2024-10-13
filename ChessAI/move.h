#ifndef MOVE_H
#define MOVE_H

#include "chess.h"
#include "util/position.h"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \class:  Move
///
/// \brief:  Function container used for altering game state and determining valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
// TODO: make Move friends with Agent and ChessState so makeMove won't be called without a check for valid move
class Move
{
	static void getValidMovesPawn(const Color player, const PieceNode& piece, const ChessState& game, std::vector<util::Position>& moves);
	static void getValidMovesKnight(const Color player, const PieceNode& piece, const ChessState& game, std::vector<util::Position>& moves);
	static void getValidMovesLinear(const Color player, const PieceNode& piece, const ChessState& game, const std::vector<util::Position>& directions, std::vector<util::Position>& moves);
	static void getValidMovesKing(const Color player, const PieceNode& piece, const ChessState& game, std::vector<util::Position>& moves);
	static bool isUnderAttack(const Color player, const ChessState& game, const util::Position& position);
	static bool canCastle(const Color player, const PieceNode& piece, const ChessState& game, const bool kingSide);

public:
	static std::vector<util::Position> getValidMoves(const Color player, const PieceNode& piece, const ChessState& game);
	static bool isValidMove(const Color player, const PieceNode& piece, const util::Position& destination, const ChessState& game);
	static bool inCheck(const Color player, const ChessState& game);
	static void makeMove(const Color player, const PieceNode& piece, const util::Position& destination, ChessState& game, const PieceType promotion = QUEEN);
};

#endif // MOVE_H