#include "bitboardUtil.h"

#include <boost/json.hpp>
#include <iostream>

#include "../../constants.h"
#include "../position.h"
#include "bitboardSet.h"

using namespace boost;

namespace util
{
	namespace bitboard
	{
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

		void print(const Bitboard bitboard)
		{
			for (int y = 0; y < RANK_COUNT; y++)
			{
				for (int x = 0; x < FILE_COUNT; x++)
				{
					const uint64_t position = positionToBitboard(x, y);
					std::cout << (bitboard & position ? '1' : '0');
				}
				std::cout << std::endl;
			}
		}

		Shift::Shift() :
			horizontal(0),
			vertical(0)
		{
		}

		Shift::Shift(const int horizontal, const int vertical) :
			horizontal(horizontal),
			vertical(vertical)
		{
		}

		Shift::Shift(const std::initializer_list<Shift> shifts) :
			horizontal(0),
			vertical(0)
		{
			for (const Shift& shift : shifts)
			{
				horizontal += shift.horizontal;
				vertical += shift.vertical;
			}
		}

		Shift& Shift::operator=(const std::initializer_list<Shift> shifts)
		{
			for (const Shift& shift : shifts)
			{
				horizontal += shift.horizontal;
				vertical += shift.vertical;
			}

			return *this;
		}


		Bitboard shiftBitboard(Bitboard bitboard, const std::initializer_list<Shift> shifts)
		{
			int totalHorizontalShift = 0;
			int totalVerticalShift = 0;

			for (const Shift& shift : shifts) {
				totalHorizontalShift += shift.horizontal;
				totalVerticalShift += shift.vertical;
			}

			if (totalHorizontalShift > FILE_COUNT) {
				throw std::exception("Horizontal shift exceeds the number of files");
			}
			else if (totalVerticalShift > RANK_COUNT) {
				throw std::exception("Vertical shift exceeds the number of ranks");
			}

			if (totalHorizontalShift < 0) // left
			{
				Bitboard leftShiftMask = 0xFEFEFEFEFEFEFEFE;
				for (int i = totalHorizontalShift + 1; i < 0; i++)
				{
					leftShiftMask &= (leftShiftMask << 1);
				}

				bitboard = (bitboard & leftShiftMask) >> -totalHorizontalShift;
			}
			else if (totalHorizontalShift != 0) // right
			{
				Bitboard rightShiftMask = 0x7F7F7F7F7F7F7F7F;
				for (int i = 1; i < totalHorizontalShift; i++)
				{
					rightShiftMask &= rightShiftMask >> 1;
				}

				bitboard = (bitboard & rightShiftMask) << totalHorizontalShift;
			}

			if (totalVerticalShift < 0) // up
			{
				bitboard >>= (-totalVerticalShift * FILE_COUNT);
			}
			else if (totalVerticalShift != 0) // down
			{
				bitboard <<= (totalVerticalShift * FILE_COUNT);
			}

			return bitboard;
		}

		Bitboard shiftBitboard(Bitboard bitboard, const Shift& shift)
		{
			return shiftBitboard(bitboard, { shift });
		}

		Shift up(const int magnitude)
		{
			return Shift(0, -magnitude);
		}

		Shift down(const int magnitude)
		{
			return Shift(0, magnitude);
		}

		Shift left(const int magnitude)
		{
			return Shift(-magnitude, 0);
		}

		Shift right(const int magnitude)
		{
			return Shift(magnitude, 0);
		}
	}
}
