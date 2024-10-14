#include "utility.h"

#include <sstream>
#include <boost/json.hpp>

#include "../constants.h"
#include "position.h"
#include "bitboard.h"

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

	Bitboard positionToBitboard(const int x, const int y)
	{
		return Bitboard(1) << (y * FILE_COUNT + x);
	}

	Bitboard positionToBitboard(const Position& position)
	{
		return positionToBitboard(position.x, position.y);
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

	json::array getJsonFromBoard(const BitboardSet& board)
	{
		json::array result = json::array(RANK_COUNT, json::array(FILE_COUNT));
		for (int y = 0; y < RANK_COUNT; y++)
		{
			for (int x = 0; x < FILE_COUNT; x++)
			{
				bool occupied = false;
				std::string symbol = " ";

				int color = Color::WHITE;
				while (color < COLOR_COUNT && !occupied)
				{
					int pieceType = PieceType::PAWN;
					while (pieceType < PIECE_TYPE_COUNT && !occupied)
					{
						if (board.posIsOccupied(x, y, (Color)color, (PieceType)pieceType))
						{
							symbol = std::string(1, PIECE_SYMBOLS[color][pieceType]);
							occupied = true;
						}
						pieceType += 1;
					}
					color += 1;
				}
				result.at(y).at(x) = symbol;
			}
		}

		return result;
	}

	BitboardSet getBoardFromJson(const json::array& boardJson)
	{
		BitboardSet result;
		for (int y = 0; y < RANK_COUNT; y++)
		{
			const json::array rowJson = boardJson[y].as_array();
			for (int x = 0; x < FILE_COUNT; x++)
			{
				const char symbol = rowJson[x].as_string().front();
				if (symbol != ' ')
				{
					bool searching = false;
					int color = Color::WHITE;
					while (color < COLOR_COUNT && searching)
					{
						int pieceType = PieceType::PAWN;
						while (pieceType < PIECE_TYPE_COUNT && searching)
						{
							if (PIECE_SYMBOLS[color][pieceType] == symbol)
							{
								result.addPiece(y, x, (Color)color, (PieceType)pieceType);
								searching = false;
							}
							pieceType += 1;
						}
						color += 1;
					}
				}
			}
		}

		return result;
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
}