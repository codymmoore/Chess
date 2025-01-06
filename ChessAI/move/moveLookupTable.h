#pragma once

#include <inttypes.h>
#include <array>

#include "../constants.h"

using Bitboard = uint64_t;

namespace move
{
	/**
	 * Get the bitboard representation of moves for a knight at the specified position.
	 *
	 * \param positionIndex the index of the position of the knight whose moves are being retrieved
	 * \return bitboard representation of moves
	 */
	Bitboard getKnightMoveBoard(const int positionIndex);

	/**
	 * Get the bitboard representation of moves for a bishop at the specified position.
	 *
	 * \param positionIndex the index of the position of the bishop whose moves are being retrieved
	 * \param occupancyBoard the occupancy board for the game state in which moves are being retrieved
	 * \return bitboard representation of moves
	 */
	Bitboard getBishopMoveBoard(const int positionIndex, const Bitboard occupancyBoard);

	/**
	 * Get the bitboard representation of moves for a rook at the specified position.
	 *
	 * \param positionIndex the index of the position of the rook whose moves are being retrieved
	 * \param occupancyBoard the occupancy board for the game state in which moves are being retrieved
	 * \return bitboard representation of moves
	 */
	Bitboard getRookMoveBoard(const int positionIndex, const Bitboard occupancyBoard);

	/**
	 * Get the bitboard representation of moves for a king at the specified position.
	 *
	 * \param positionIndex the index of the position of the king whose moves are being retrieved
	 * \return bitboard representation of moves
	 */
	Bitboard getKingMoveBoard(const int positionIndex);
}
