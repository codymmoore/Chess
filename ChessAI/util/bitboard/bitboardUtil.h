#pragma once

#include <inttypes.h>

#include "../../constants.h"

namespace boost
{
	namespace json
	{
		class array;
	}
}

using Bitboard = uint64_t;

namespace util
{
	struct Position;

	namespace bitboard
	{
		class BitboardSet;

		/**
		 * Enum used to shift bitboards.
		 */
		enum Shift
		{
			UP = FILE_COUNT,
			DOWN = -FILE_COUNT,
			LEFT = -1,
			RIGHT = 1
		};

		/**
		 * Shift a bitboard using the specified direction.
		 *
		 * @see ShiftDirection enum.
		 *
		 * \param bitboard bitboard being shifted
		 * \param direction direction to shift the bitboard
		 * \return shifted bitboard
		 */
		Bitboard shift(const Bitboard bitboard, const int direction);

		/**
		 * Converts a position to its bitboard representation.
		 *
		 * \param x The x-coordinate of the position
		 * \param y The y-coordinate of the position
		 * \return The bitboard representation of the position
		 */
		Bitboard positionToBitboard(const int x, const int y);

		/**
		 * Converts a Position to its bitboard representation.
		 *
		 * \param position The Position to be converted
		 * \return The bitboard representation of the Position
		 */
		Bitboard positionToBitboard(const Position& position);

		/**
		 * Converts a BitBoard to a JSON array.
		 *
		 * \param board The BitBoard to be converted
		 * \return The JSON representation of the board
		 */
		boost::json::array getJsonFromBoard(const BitboardSet& board);

		/**
		 * Converts a JSON array to a BitBoard.
		 *
		 * \param boardJson The JSON representation of a board
		 * \return A new BitBoard instance
		 */
		BitboardSet getBoardFromJson(const boost::json::array& boardJson);
	}
}
