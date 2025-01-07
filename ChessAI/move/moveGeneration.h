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

	/**
	 * Generate moves for all of the specified player's rooks.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the player's rooks
	 */
	std::vector<Move> generateRookMoves(const ChessState& chessState, const Color player);

	/**
	 * Generate moves for all of the specified player's queens.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the player's queens
	 */
	std::vector<Move> generateQueenMoves(const ChessState& chessState, const Color player);

	/**
	 * Generate moves for the specified player's king.
	 *
	 * Note: does not include castling
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being generated
	 * \return valid moves for all the player's kings
	 */
	std::vector<Move> generateKingMoves(const ChessState& chessState, const Color player);
}
