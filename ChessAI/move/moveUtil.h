#pragma once

#include "../chess.h"
#include "../util/position.h"

namespace move
{
	struct Move;

	/**
	 * Get all valid moves for the specified player.
	 *
	 * \param chessState game state
	 * \param player player whose moves are being generated
	 * \return vector of valid moves
	 */
	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player);

	/**
	 * Determines if a move is valid.
	 *
	 * \param player owner of the piece being move
	 * \param piece the piece being moved
	 * \param destination the end position of the move
	 * \param chessState game state
	 * \return true if move is valid, false otherwise
	 */
	bool isValidMove(const Color player, const PieceNode& piece, const util::Position& destination, const ChessState& chessState);

	/**
	 * Determines if a player is in chech.
	 *
	 * \param player player being evaluated
	 * \param chessState game state
	 * \return true if player is in check, false otherwise
	 */
	bool inCheck(const Color player, const ChessState& chessState);

	/**
	 * Moves a piece and updates the game state.
	 *
	 * \param player owner of the piece being move
	 * \param move contains move information
	 * \param chessState game state
	 * \param promotion piece type to promote to; default is queen
	 */
	void makeMove(const Color player, const Move move, ChessState& chessState, const PieceType promotion = PieceType::QUEEN);

	/**
	 * Moves a piece and updates the game state.
	 *
	 * \param player owner of the piece being move
	 * \param piece the piece being moved
	 * \param destination the position the piece is being moved to
	 * \param chessState game state
	 * \param promotion piece type to promote to; default is queen
	 */
	void makeMove(const Color player, const util::Position& source, const util::Position& destination, ChessState& chessState, const PieceType promotion = PieceType::QUEEN);
}