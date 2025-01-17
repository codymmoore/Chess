#pragma once

namespace util
{
	namespace bitboard
	{
		/**
		 * Stores horizontal and vertical direction and magnitude used to shift a bitboard.
		 */
		struct Shift
		{
			int horizontal;
			int vertical;

			Shift(const Shift& shift) = default;

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
			 * Sets the calling object equal to a combined list of Shifts.
			 *
			 * \param shifts the list of Shifts to be combined into the current instance
			 * \return a reference to the calling object
			 */
			Shift& operator=(const Shift& shift) = default;

			/**
			 * Adds the horizontal and vertical shift of the right operand to the calling object.
			 *
			 * \param rightOperand the Shift being added to the calling object
			 * \return a reference to the calling object
			 */
			Shift& operator+=(const Shift& rightOperand);

			/**
			 * Gets the sum of two Shift instances.
			 *
			 * \param rightOperand the Shift being added to the calling object
			 * \return the sum of the two Shift instances
			 */
			Shift operator+(const Shift& rightOperand) const;

			/**
			 * Subtracts the horizontal and vertical shift of the right operand from the calling object.
			 *
			 * \param rightOperand the Shift being subtracted from the calling object
			 * \return a reference to the calling object
			 */
			Shift& operator-=(const Shift& rightOperand);

			/**
			 * Gets the difference between two Shift instances.
			 *
			 * \param rightOperand the Shift being subtracted from the calling object
			 * \return the difference between two Shift instances
			 */
			Shift operator-(const Shift& rightOperand) const;

			/**
			 * Gets a Shift in the opposite direction of the calling object.
			 *
			 * \return a Shift in the opposite direction of the calling object
			 */
			Shift operator-() const;

			/**
			 * Multiplies the horizontal and vertical shift of the calling object by the specified multiplier.
			 *
			 * \param multipier the multiplier for the horizontal and vertical shift
			 * \return a reference to the calling object
			 */
			Shift& operator*=(const int multipier);

			/**
			 * Multiplies a Shift by the specified multiplier.
			 *
			 * \param multiplier the multiplier for the horizontal and vertical shift
			 * \return the multiplied Shift
			 */
			Shift operator*(const int multiplier) const;

			/**
			 * Determines if two Shift instances are equivalent.
			 *
			 * \param rightOperand the Shift being compared to the calling object
			 * \return true if the Shifts are equivalent, false otherwise
			 */
			bool operator==(const Shift& rightOperand) const = default;

			/**
			 * Determines if two Shift instances are not equivalent.
			 *
			 * \param rightOperand the Shift being compared to the calling object
			 * \return true if the Shifts are not equivalent, false otherwise
			 */
			bool operator!=(const Shift& rightOperand) const = default;
		};

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
