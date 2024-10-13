#pragma once

#include <inttypes.h>

#include "position.h"
#include "../enum.h"

namespace util
{
	/**
	 * Contains the bitboard representations of a chess board.
	 */
	class Bitboard
	{
	public:
		/**
		 * Creates a new BitBoard instance with an empty board.
		 *
		 */
		Bitboard();

		/**
		 * Creates a new BitBoard instance from an existing instance.
		 *
		 * \param source The BitBoard instance being copied
		 */
		Bitboard(const Bitboard& source);

		/* ----- Accessors ----- */
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
		 * Get 64-bit integer representation of occupied positions.
		 *
		 * \return 64-bit integer representation of occupied spaces
		 */
		uint64_t getOccupiedSpaces() const;

		/**
		 * Get 64-bit integer representation of positions occupied by a color.
		 *
		 * \param color The color being checked for
		 * \return 64-bit integer representation of spaces occupied by a color
		 */
		uint64_t getColorBoard(const Color color) const;

		/**
		 * Get 64-bit integer representation of positions occupied by a piece type.
		 *
		 * \param pieceType The piece type being checked for
		 * \return 64-bit integer representation of spaces occupied by a piece type
		 */
		uint64_t getPieceBoard(const PieceType pieceType) const;

		/**
		 * Get 64-bit integer representation of positions occupied by a piece of the specified type and color.
		 *
		 * \param color The color being checked for
		 * \param pieceType The piece type being checked for
		 * \return 64-bit integer representation of spaces occupied by the piece of the specified type and color
		 */
		uint64_t getColorPieceBoard(const Color color, const PieceType pieceType) const;

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
		 * \param x The x-coordinate of the position to be cleared
		 * \param y The y-coordinate of the position to be cleared
		 * \param pieceType The type of piece being cleared
		 */
		void clearPos(const int x, const int y, const PieceType pieceType);

		/**
		 * Clear a position on the board.
		 *
		 * \param pos The position being cleared
		 * \param pieceType The type of piece being cleared
		 */
		void clearPos(const Position& pos, const PieceType pieceType);

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
		 * Clear all pieces of a color off the board.
		 *
		 * \param color The color of pieces being cleared
		 */
		void clearColor(const Color color);

		/**
		 * Clear all pieces of a type off the board.
		 *
		 * \param pieceType The type of pieces being cleared
		 */
		void clearPieceType(const PieceType pieceType);

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
		Bitboard& operator=(const Bitboard& rightOperand);

		/**
		 * Determines if two bitboards are equivalent.
		 *
		 * \param rightOperand The bitboard the calling object is being compared to
		 * \return True if the bitboards are equivalent, false otherwise
		 */
		bool operator==(const Bitboard& rightOperand) const;

		/**
		 * Determines if two bitboards are not equivalent.
		 *
		 * \param rightOperand The bitboard the calling object is being compared to
		 * \return True if the bitboards are equivalent, false otherwise
		 */
		bool operator!=(const Bitboard& rightOperand) const;

	private:
		uint64_t m_board[COLOR_COUNT][PIECE_TYPE_COUNT];
	};
}
