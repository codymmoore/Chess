#pragma once

#include "../enum.h"
#include "../util/position.h"

namespace move
{
	/**
	 * Contains information for a move in a game of chess.
	 */
	struct Move
	{
		util::Position source;
		util::Position destination;
		PieceType promotion;

		Move() = default;

		/**
		 * Creates a new Move instance.
		 *
		 * \param source the position of the piece before the move
		 * \param destination the position of the piece after the move
		 */
		Move(const util::Position& source, const util::Position& destination);

		/**
		 * Creates a new Move instance.
		 *
		 * \param source the position of the piece before the move
		 * \param destination the position of the piece after the move
		 * \param promotion the piece a pawn will be promoted to
		 */
		Move(const util::Position& source, const util::Position& destination, const PieceType promotion);

		/**
		 * Creates a new Move instance copied from another.
		 *
		 * \param source the Move the new instance is copying from
		 */
		Move(const Move& source);

		/**
		 * Sets the calling object equal to the right operand.
		 *
		 * \param rightOperand the Move the calling object is being set equal to
		 * \return a reference to the updated calling object
		 */
		Move& operator=(const Move& rightOperand);

		/**
		 * Determines if two Moves are equivalent.
		 *
		 * \param rightOperand the Move being compared to the calling object
		 * \return true if the Moves are equivalent, false otherwise
		 */
		bool operator==(const Move& rightOperand) const;

		/**
		 * Determines if two Moves are not equivalent.
		 *
		 * \param rightOperand the Move being compared to the calling object
		 * \return true if the Moves are not equivalent, false otherwise
		 */
		bool operator!=(const Move& rightOperand) const;

		struct MoveHasher
		{
			/**
			 * Generates a hash value for a Move.
			 *
			 * \param move the Move whose hash is being generated
			 * \return hash value for the specified Move
			 */
			std::size_t operator()(const Move& move) const;
		};
	};
}
