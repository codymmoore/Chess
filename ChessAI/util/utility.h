#pragma once

#include <vector>
#include <boost/json.hpp>

#include "../enum.h"

using Bitboard = uint64_t;

namespace util
{
	struct Position;

	/**
	 * Splits a string into substrings based on a delimeter.
	 *
	 * \param str The string being split into substrings
	 * \param delimeter The delimeter between substrings
	 * \return A vector containing the substrings
	 */
	std::vector<std::string> stringSplit(const std::string& originalString, const char delim);

	/**
	 * Converts a position to a string containing the file and rank for a chess board.
	 *
	 * \param x The x-coordinate of the position
	 * \param y The y-coordinate of the position
	 * \return A string containing the file and rank
	 */
	std::string toFileAndRank(const int x, const int y);

	/**
	 * Converts a Position to a string containing the file and rank for a chess board.
	 *
	 * \param position The Position being converted to file and rank
	 * \return A string containing the file and rank
	 */
	std::string toFileAndRank(const Position& position);

	/**
	* Get the opposite Color.
	*
	* \param color
	* \return BLACK if color is WHITE, WHITE otherwise
	*/
	Color operator~(const Color color);

	/**
	 * Get the Color associated with a string.
	 *
	 * \param str The string being converted to a Color
	 * \return The Color associated with the string
	 */
	Color getColorFromString(const std::string& str);

	/**
	 * Get the string associated with a Color.
	 *
	 * \param color The Color being converted to a string
	 * \return The string associated with the Color
	 */
	std::string toString(const Color& color);

	/**
	 * Get the PieceType associated with a string.
	 *
	 * \param str The string being converted to a PieceType
	 * \return The PieceType associated with the string
	 */
	PieceType getPieceTypeFromString(const std::string& str);

	/**
	 * Get the string associated with a PieceType.
	 *
	 * \param pieceType The PieceType being converted to a string
	 * \return The string associated with the PieceType
	 */
	std::string toString(const PieceType& pieceType);

	/**
	 * Convert an index to a Position object.
	 *
	 * \param positionIndex an integer (0-63) corresponding with a position on a board
	 * \return index's correpsonding Position object
	 */
	Position toPosition(const int positionIndex);
}


