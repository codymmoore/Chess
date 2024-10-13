#pragma once

#include <iostream>

namespace util
{
	/**
	 * Contains cartesian coordinates.
	 */
	struct Position
	{
		int x, y;

		Position() = default;

		/**
		 * Creates a new instance of Position.
		 *
		 * \param x The x-coordinate of the position
		 * \param y The y-coordinate of the position
		 */
		Position(const int x, const int y);

		/**
		 * Sets a Position object equal to another.
		 *
		 * \param rightOperand The Position the calling object is being set equal to
		 * \return A reference to the calling object
		 */
		Position& operator=(const Position& rightOperand);

		/**
		 * Sets the calling object equal to the sum of itself and the right operand.
		 *
		 * \param rightOperand The Position being added to the calling object
		 * \return A reference to the calling object
		 */
		Position& operator+=(const Position& rightOperand);

		/**
		 * Adds two Position objects together.
		 *
		 * \param rightOperand The Position object being added to the calling object
		 * \return The sum of the two Position object
		 */
		Position operator+(const Position& rightOperand) const;

		/**
		 * Sets the calling object equal to the difference of itself and the right operand.
		 *
		 * \param rightOperand The Position being subtracted from the calling object
		 * \return A reference to the calling object
		 */
		Position& operator-=(const Position& rightOperand);

		/**
		 * Subtracts one Position object from another.
		 *
		 * \param rightOperand The Position object being subtracted from the calling object
		 * \return The negated Position
		 */
		Position operator-(const Position& rightOperand) const;

		/**
		 * Sets the calling object equal to the product of itself and the multiplier.
		 *
		 * \param multiplier The integer the Position's coordinates will be multiplied by
		 * \return A reference to the calling object
		 */
		Position& operator*=(const int multiplier);

		/**
		 * Multiplies the coordinates of a Position object.
		 *
		 * \param multiplier The integer the Position's coordinate will be multiplied by
		 * \return The multiplied Position
		 */
		Position operator*(const int multiplier) const;

		/**
		 * Negates the coordiantes.
		 *
		 * \return The negated Position
		 */
		Position operator-() const;

		/**
		 * Determines if two Positions are equivalent.
		 *
		 * \param rightOperand The Position being compared to the calling object
		 * \return True if the Positions are equivalent, false otherwise
		 */
		bool operator==(const Position& rightOperand) const;

		/**
		 * Determines if two Positions are not equivalent.
		 *
		 * \param rightOperand The Position being compared to the calling object
		 * \return True if the Positions are not equivalent, false otherwise
		 */
		bool operator!=(const Position& rightOperand) const;

		struct PositionHasher
		{
			std::size_t operator()(const Position& position) const;
		};

		const static Position UP, DOWN, LEFT, RIGHT;
	};

	/**
	 * Writes a Position to an output stream.
	 *
	 * \param out The output stream
	 * \param position The Position being written to the output stream
	 */
	std::ostream& operator<<(std::ostream& out, const Position& position);

	// Directions
	const static Position UP(0, -1),
		DOWN(0, 1),
		LEFT(-1, 0),
		RIGHT(1, 0);
}
