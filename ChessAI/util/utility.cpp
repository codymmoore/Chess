#include "utility.h"

#include <sstream>
#include <boost/json.hpp>

#include "../constants.h"
#include "position.h"

using namespace boost;

namespace util
{
	std::vector<std::string> stringSplit(const std::string& str, char delimeter)
	{
		std::vector<std::string> substrings;
		std::stringstream sStream(str);
		std::string substring;

		while (std::getline(sStream, substring, delimeter))
			substrings.push_back(substring);

		return substrings;
	}

	std::string toFileAndRank(const int x, const int y)
	{
		std::string result = "";

		// Convert x-coordinate to file
		result += (char)(x + 97);

		// Convert y-coordinate to rank
		result += std::to_string(RANK_COUNT - y);

		return result;
	}

	std::string toFileAndRank(const Position& position)
	{
		return toFileAndRank(position.x, position.y);
	}

	Color operator~(const Color color)
	{
		return (color == Color::WHITE ? Color::BLACK : Color::WHITE);
	}

	Color getColorFromString(const std::string& str)
	{
		if (str == "WHITE")
			return Color::WHITE;
		else if (str == "BLACK")
			return Color::BLACK;
		else
			return Color::NEUTRAL;
	}

	std::string toString(const Color& color)
	{
		switch (color)
		{
		case Color::WHITE:
			return "WHITE";
		case Color::BLACK:
			return "BLACK";
		default:
			return "NEUTRAL";
		}
	}

	PieceType getPieceTypeFromString(const std::string& str)
	{
		if (str == "PAWN")
			return PieceType::PAWN;
		else if (str == "KNIGHT")
			return PieceType::KNIGHT;
		else if (str == "BISHOP")
			return PieceType::BISHOP;
		else if (str == "ROOK")
			return PieceType::ROOK;
		else if (str == "QUEEN")
			return PieceType::QUEEN;
		else if (str == "KING")
			return PieceType::KING;
		else
			return PieceType::NONE;
	}

	std::string toString(const PieceType& pieceType)
	{
		switch (pieceType)
		{
		case PieceType::PAWN:
			return "PAWN";
		case PieceType::KNIGHT:
			return "KNIGHT";
		case PieceType::BISHOP:
			return "BISHOP";
		case PieceType::ROOK:
			return "ROOK";
		case PieceType::QUEEN:
			return "QUEEN";
		case PieceType::KING:
			return "KING";
		default:
			return "NONE";
		}
	}

	Position toPosition(const int positionIndex)
	{
		if (positionIndex < 0 || positionIndex >= (FILE_COUNT * RANK_COUNT))
		{
			std::string errorMessage = "Invalid position index: " + std::to_string(positionIndex) + ". Index must be 0-63";
			throw std::exception(errorMessage.c_str());
		}

		const int x = positionIndex % FILE_COUNT;
		const int y = positionIndex / FILE_COUNT;
		return Position(x, y);
	}
}