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
	 * \return valid moves for all the player's pawns
	 */
	std::vector<Move> generatePawnMoves(const ChessState& chessState, const Color player);

	/**
	 * Generate moves for all of the specified player's knights.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the player's knights
	 */
	std::vector<Move> generateKnightMoves(const ChessState& chessState, const Color player);

	/**
	 * Generate moves for all of the specified player's bishops.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the player's bishops
	 */
	std::vector<Move> generateBishopMoves(const ChessState& chessState, const Color player);
}
