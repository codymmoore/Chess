#pragma once

#include <inttypes.h>
#include <initializer_list>

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
		void print(const Bitboard bitboard);

		/**
		 * Stores horizontal and vertical direction and magnitude used to shift a bitboard.
		 */
		struct Shift
		{
			int horizontal;
			int vertical;

			/**
			 * Create a default Shift instance.
			 *
			 */
			Shift();

			/**
			 * Create a new Shift instance.
			 *
			 * \param horizontal the direction and magnitude of the horizontal shift
			 *		(horizontal < 0 -> left, horizontal > 0 -> right)
			 * \param vertical the direction and magnitude of the vertical shift
			 *		(vertical < 0 -> up, vertical > 0 -> down)
			 */
			Shift(const int horizontal, const int vertical);

			/**
			 * Create a new Shift instance from a list of Shifts.
			 *
			 * \param shifts the list of Shifts to be combined into the new instance
			 */
			Shift(const std::initializer_list<Shift> shifts);

			/**
			 * Sets the calling object equal to a combined list of Shifts.
			 *
			 * \param shifts the list of Shifts to be combined into the current instance
			 * \return a reference to the calling object
			 */
			Shift& operator=(const std::initializer_list<Shift> shifts);
		};

		/**
		 * Shifts a bitboard and applies a bit mask to account for pieces being moved off the board.
		 *
		 * \param bitboard the bitboard being shifted
		 * \param shifts a list of shifts to apply to the bitboard
		 * \return a shifted copy of the bitboad
		 */
		Bitboard shiftBitboard(Bitboard bitboard, const std::initializer_list<Shift> shifts);

		/**
		 * Shifts a bitboard and applies a bit mask to account for pieces being moved off the board.
		 *
		 * \param bitboard the bitboard being shifted
		 * \param shift shift to apply to the bitboard
		 * \return a shifted copy of the bitboad
		 */
		Bitboard shiftBitboard(Bitboard bitboard, const Shift& shift);

		/**
		 * Provides a Shift object to be used to shift a bitboard up.
		 *
		 * \param magnitude the distance the bitboard will be shifted
		 * \return a Shift object to be used to shift a bitboard
		 */
		Shift up(const int magnitude);

		/**
		 * Provides a Shift object to be used to shift a bitboard down.
		 *
		 * \param magnitude the distance the bitboard will be shifted
		 * \return a Shift object to be used to shift a bitboard
		 */
		Shift down(const int magnitude);

		/**
		 * Provides a Shift object to be used to shift a bitboard left.
		 *
		 * \param magnitude the distance the bitboard will be shifted
		 * \return a Shift object to be used to shift a bitboard
		 */
		Shift left(const int magnitude);

		/**
		 * Provides a Shift object to be used to shift a bitboard right.
		 *
		 * \param magnitude the distance the bitboard will be shifted
		 * \return a Shift object to be used to shift a bitboard
		 */
		Shift right(const int magnitude);
	}
}
