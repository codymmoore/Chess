#include "bitboardUtil.h"

#include <boost/json.hpp>
#include <iostream>
#include <bit>

#include "../../constants.h"
#include "../position.h"
#include "bitboardSet.h"

using namespace boost;

namespace util
{
	namespace bitboard
	{
		int popLsb(Bitboard& bitboard)
		{
			if (!bitboard)
			{
				return -1;
			}

			const int lsbIndex = std::countr_zero(bitboard);
			bitboard &= bitboard - 1;
			return lsbIndex;
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

		void print(const Bitboard bitboard, const std::string& indent)
		{
			for (int y = 0; y < RANK_COUNT; y++)
			{
				std::cout << indent;
				for (int x = 0; x < FILE_COUNT; x++)
				{
					const uint64_t position = positionToBitboard(x, y);
					std::cout << "  " << (bitboard & position ? '1' : '0');
				}
				std::cout << std::endl;
			}
		}

		Bitboard shiftBitboard(Bitboard bitboard, const Shift& shift)
		{
			if (shift.horizontal > FILE_COUNT) {
				throw std::exception("Horizontal shift exceeds the number of files");
			}
			else if (shift.vertical > RANK_COUNT) {
				throw std::exception("Vertical shift exceeds the number of ranks");
			}

			if (shift.horizontal < 0) // left
			{
				Bitboard leftShiftMask = 0xFEFEFEFEFEFEFEFE;
				for (int i = shift.horizontal + 1; i < 0; i++)
				{
					leftShiftMask &= (leftShiftMask << 1);
				}

				bitboard = (bitboard & leftShiftMask) >> -shift.horizontal;
			}
			else if (shift.horizontal != 0) // right
			{
				Bitboard rightShiftMask = 0x7F7F7F7F7F7F7F7F;
				for (int i = 1; i < shift.horizontal; i++)
				{
					rightShiftMask &= rightShiftMask >> 1;
				}

				bitboard = (bitboard & rightShiftMask) << shift.horizontal;
			}

			if (shift.vertical < 0) // up
			{
				bitboard >>= (-shift.vertical * FILE_COUNT);
			}
			else if (shift.vertical != 0) // down
			{
				bitboard <<= (shift.vertical * FILE_COUNT);
			}

			return bitboard;
		}
	}
}
