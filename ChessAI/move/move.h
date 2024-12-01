#pragma once

#include "../chess.h"
#include "../util/position.h"

namespace move
{
	struct Move;

	std::vector<Move> getValidMoves(const ChessState& game, const Color player);
	bool isValidMove(const Color player, const PieceNode& piece, const util::Position& destination, const ChessState& game);
	bool inCheck(const Color player, const ChessState& game);
	void makeMove(const Color player, const PieceNode& piece, const util::Position& destination, ChessState& game, const PieceType promotion = QUEEN);
}