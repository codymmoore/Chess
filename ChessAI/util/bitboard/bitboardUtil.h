#pragma once

#include <inttypes.h>
#include <string>

#include "../../constants.h"
#include "shift.h"

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
		struct Shift;

		/**
		 * Pop the least significant bit (LSB) and return its index.
		 *
		 * \param bitboard the bitboard whose LSB is being popped
		 * \return index of LSB; -1 if no LSB exists
		 */
		int popLsb(Bitboard& bitboard);

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

		/**
		 * Prints a bitboard to stdout.
		 *
		 * \param bitboard the bitboard to be printed
		 */
		void print(const Bitboard bitboard, const std::string& indent = "");

		/**
		 * Shifts a bitboard and applies a bit mask to account for pieces being moved off the board.
		 *
		 * \param bitboard the bitboard being shifted
		 * \param shift shift to apply to the bitboard
		 * \return a shifted copy of the bitboad
		 */
		Bitboard shiftBitboard(Bitboard bitboard, const Shift& shift);
	}
}
