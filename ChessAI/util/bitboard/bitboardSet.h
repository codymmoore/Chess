#pragma once

#include <inttypes.h>

#include "../position.h"
#include "../../enum.h"

using Bitboard = uint64_t;

namespace util
{
	namespace bitboard
	{
		/**
		 * Contains the bitboard representations of a chess board.
		 */
		class BitboardSet
		{
		public:
			/**
			 * Creates a new BitBoard instance with an empty board.
			 *
			 */
			BitboardSet();

			/**
			 * Creates a new BitBoard instance from an existing instance.
			 *
			 * \param source The BitBoard instance being copied
			 */
			BitboardSet(const BitboardSet& source);

			/* ----- Accessors ----- */
			/**
			 * Get bitboard for specified piece type and color.
			 *
			 * \param color
			 * \param pieceType
			 * \return Bitboard containing pieces of specified type and color
			 */
			Bitboard getBitboard(const Color color, const PieceType pieceType) const;

			/**
			 * Get bitboard containing all pieces.
			 *
			 * \return Bitboard containing all pieces
			 */
			Bitboard getOccupancyBoard() const;

			/**
			 * Get bitboard containing all pieces of the specified color.
			 *
			 * \param color
			 * \return Bitboard containing pieces of the specified color
			 */
			Bitboard getOccupancyBoard(const Color color) const;

			/**
			 * Determines if a position is occupied.
			 *
			 * \param x The x-coordinate of the position
			 * \param y The y-coordinate of the position
			 * \return True if the position is occupied, false otherwise
			 */
			bool posIsOccupied(const int x, const int y) const;

			/**
			 * Determines if a Position is occupied.
			 *
			 * \param position The Position being checked
			 * \return True if the Position is occupied, false otherwise
			 */
			bool posIsOccupied(const Position& position) const;

			/**
			 * Determines if a position is occupied by a Color.
			 *
			 * \param x The x-coordinate of the position
			 * \param y The y-coordinate of the position
			 * \param color The Color being checked for
			 * \return True if a piece of the specified color is occupying the position, false otherwise
			 */
			bool posIsOccupied(const int x, const int y, const Color color) const;

			/**
			 * Determines if a Position is occupied by a Color.
			 *
			 * \param position The Position being checked
			 * \param color The Color being checked for
			 * \return True if a piece of the specified color is occupying the position, false otherwise
			 */
			bool posIsOccupied(const Position& position, const Color color) const;

			/**
			 * Determines if a position is occupied by a piece type.
			 *
			 * \param x The x-coordinate of the position
			 * \param y The y-coordinate of the position
			 * \param pieceType The type of piece being checked for
			 * \return True if the position is occupied by the specified piece type, false otherwise
			 */
			bool posIsOccupied(const int x, const int y, const PieceType pieceType) const;

			/**
			 * Determines if a Position is occupied by a piece type.
			 *
			 * \param pos The Position being checked
			 * \param pieceType The type of piece being checked for
			 * \return True if the Position is occupied by the specified piece type, false otherwise
			 */
			bool posIsOccupied(const Position& pos, const PieceType pieceType) const;

			/**
			 * Determines if a position is occupied by a piece of the specified type and color.
			 *
			 * \param x The x-coordinate of the position
			 * \param y The y-coordinate of the position
			 * \param color The color being checked for
			 * \param pieceType The type of piece being checked for
			 * \return True if the position is occupied by a piece of the specified type and color, false otherwise
			 */
			bool posIsOccupied(const int x, const int y, const Color color, const PieceType pieceType) const;

			/**
			 * Determines if a Position is occupied by a piece of the specified type and color.
			 *
			 * \param pos The Position being checked
			 * \param color The color being checked for
			 * \param pieceType The type of piece being checked for
			 * \return True if the Position is occupied by a piece of the specified type and color, false otherwise
			 */
			bool posIsOccupied(const Position& pos, const Color color, const PieceType pieceType) const;

			/**
			 * Print ASCII representation of board to stdout.
			 *
			 */
			void print() const;

			/* ----- Modifiers ----- */
			/**
			 * Set board to default starting chess board.
			 *
			 */
			void populateBoard();

			/**
			 * Clear a position on the board.
			 *
			 * \param x The x-coordinate of the position to be cleared
			 * \param y The y-coordinate of the position to be cleared
			 */
			void clearPos(const int x, const int y);

			/**
			 * Clear a position on the board.
			 *
			 * \param pos The position to be cleared
			 */
			void clearPos(const Position& pos);

			/**
			 * Clear a position on the board.
			 *
			 * \param x The x-coordinate of the position to be cleared
			 * \param y The y-coordinate of the position to be cleared
			 * \param color The color of the piece to be cleared
			 */
			void clearPos(const int x, const int y, const Color color);

			/**
			 * Clear a position on the board.
			 *
			 * \param pos The position to be cleared
			 * \param color The color of the piece to be cleared
			 */
			void clearPos(const Position& pos, const Color color);

			/**
			 * Clear a position on the board.
			 *
			 * \param x The x-coordinate of the position being cleared
			 * \param y The y-coordinate of the position being cleared
			 * \param color The color of the piece being cleared
			 * \param pieceType The type of piece being cleared
			 */
			void clearPos(const int x, const int y, const Color color, const PieceType pieceType);

			/**
			 * Clear a position on the board.
			 *
			 * \param pos The position being cleared
			 * \param color The color of the piece being cleared
			 * \param pieceType The type of piece being cleared
			 */
			void clearPos(const Position& pos, const Color color, const PieceType pieceType);

			/**
			 * Clear all positions on the board.
			 *
			 */
			void clear();

			/**
			 * Add a piece to the board.
			 *
			 * \param x The x-coordinate of the position of the piece
			 * \param y The y-coordinate of the position of the piece
			 * \param color The color of the piece being added
			 * \param pieceType The type of piece being added
			 */
			void addPiece(const int x, const int y, const Color color, const PieceType pieceType);

			/**
			 * Add a piece to the board.
			 *
			 * \param pos The position of the piece
			 * \param color The color of the piece being added
			 * \param pieceType The type of piece being added
			 */
			void addPiece(const Position& pos, const Color color, const PieceType pieceType);

			/* ----- Operators ----- */
			/**
			 * Assigns the calling object to the right operand.
			 *
			 * \param rightOperand The BitBoard instance the calling object is being assigned to
			 * \return A reference to the calling object
			 */
			BitboardSet& operator=(const BitboardSet& rightOperand);

			/**
			 * Determines if two bitboards are equivalent.
			 *
			 * \param rightOperand The bitboard the calling object is being compared to
			 * \return True if the bitboards are equivalent, false otherwise
			 */
			bool operator==(const BitboardSet& rightOperand) const;

			/**
			 * Determines if two bitboards are not equivalent.
			 *
			 * \param rightOperand The bitboard the calling object is being compared to
			 * \return True if the bitboards are equivalent, false otherwise
			 */
			bool operator!=(const BitboardSet& rightOperand) const;

		private:
			/**
			 * Update all occupancy boards.
			 *
			 */
			void updateOccupancyBoards();

			/**
			 * Update occupancy board for a color.
			 *
			 * \param color the color of the occupancy board being updated
			 */
			void updateOccupancyBoards(const Color color);

			Bitboard _bitboards[COLOR_COUNT][PIECE_TYPE_COUNT];
			Bitboard _allOccupancyBoard;
			Bitboard _colorOccupancyBoards[COLOR_COUNT];
		};
	}
}
