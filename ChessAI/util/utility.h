#pragma once

#include <vector>
#include <boost/json.hpp>

#include "../enum.h"

using Bitboard = uint64_t;

namespace util
{
	struct Position;
	class BitboardSet;

	/**
	 * Splits a string into substrings based on a delimeter.
	 *
	 * \param str The string being split into substrings
	 * \param delimeter The delimeter between substrings
	 * \return A vector containing the substrings
	 */
	std::vector<std::string> stringSplit(const std::string& originalString, const char delim);

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
}


