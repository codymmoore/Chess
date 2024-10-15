#include "bitboardUtil.h"

#include <boost/json.hpp>

#include "../../constants.h"
#include "../position.h"
#include "bitboard.h"

using namespace boost;

namespace util
{
	namespace bitboard
	{
		Bitboard shift(const Bitboard bitboard, const int direction)
		{
			if (direction < 0)
			{
				return bitboard << -direction;
			}
			else
			{
				return bitboard >> direction;
			}
		}

		Bitboard positionToBitboard(const int x, const int y)
		{
			return Bitboard(1) << (y * FILE_COUNT + x);
		}

		Bitboard positionToBitboard(const Position& position)
		{
			return positionToBitboard(position.x, position.y);
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
	}
}
