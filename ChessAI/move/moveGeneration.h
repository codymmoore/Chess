#pragma once

#include <vector>

#include "../chess.h"
#include "../enum.h"

class ChessState;

namespace move
{
	struct Move;

	/**
	 * Generate moves for all of the specified player's pawns.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the players pawns
	 */
	std::vector<Move> generatePawnMoves(const ChessState& chessState, const Color player);

	/**
	 * Generate moves for all of the specified player's knights.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the players knights
	 */
	std::vector<Move> generateKnightMoves(const ChessState& chessState, const Color player);
}
