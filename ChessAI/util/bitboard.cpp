#include "bitboard.h"

#include "utility.h"
#include "../constants.h"

using std::cout;
using std::endl;
using std::string;

namespace util
{
	BitboardSet::BitboardSet()
	{
		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_bitboards[color][pieceType] = 0;
			}
			_colorOccupancyBoards[color] = 0;
		}
		_allOccupancyBoard = 0;
	}

	BitboardSet::BitboardSet(const BitboardSet& source)
	{
		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_bitboards[color][pieceType] = source._bitboards[color][pieceType];
			}
			_colorOccupancyBoards[color] = source._colorOccupancyBoards[color];
		}
		_allOccupancyBoard = source._allOccupancyBoard;
	}

	bool BitboardSet::posIsOccupied(const int x, const int y) const
	{
		const Bitboard binaryPosition = positionToBitboard(x, y);
		return _allOccupancyBoard & binaryPosition;
	}

	bool util::BitboardSet::posIsOccupied(const Position& position) const
	{
		return posIsOccupied(position.x, position.y);
	}

	bool BitboardSet::posIsOccupied(const int x, const int y, const Color color) const
	{
		const Bitboard binaryPosition = positionToBitboard(x, y);
		return _colorOccupancyBoards[color] & binaryPosition;
	}

	bool BitboardSet::posIsOccupied(const Position& pos, const Color color) const
	{
		return posIsOccupied(pos.x, pos.y, color);
	}

	bool BitboardSet::posIsOccupied(const int x, const int y, const Color color, const PieceType pieceType) const
	{
		const Bitboard binaryPosition = positionToBitboard(x, y);
		return _bitboards[color][pieceType] & binaryPosition;
	}

	bool BitboardSet::posIsOccupied(const Position& pos, const Color color, const PieceType pieceType) const
	{
		return posIsOccupied(pos.x, pos.y, color, pieceType);
	}

	void BitboardSet::print() const
	{
		for (int y = 0; y < RANK_COUNT; y++)
		{
			for (int x = 0; x < FILE_COUNT; x++)
			{
				bool occupied = false;
				char outputChar = '-';

				int color = Color::WHITE;
				while (color < COLOR_COUNT && !occupied)
				{
					int pieceType = PieceType::PAWN;
					while (pieceType < PIECE_TYPE_COUNT && !occupied)
					{
						if (posIsOccupied(x, y, (Color)color, (PieceType)pieceType))
						{
							outputChar = PIECE_SYMBOLS[color][pieceType];
							occupied = true;
						}
						pieceType += 1;
					}
					color += 1;
				}
				cout << outputChar;
			}
			cout << endl;
		}
	}

	void BitboardSet::populateBoard()
	{
		/* ----- WHITE PIECES ----- */
		_bitboards[Color::WHITE][PieceType::PAWN] = 0x00ff000000000000;
		_bitboards[Color::WHITE][PieceType::KNIGHT] = 0x4200000000000000;
		_bitboards[Color::WHITE][PieceType::BISHOP] = 0x2400000000000000;
		_bitboards[Color::WHITE][PieceType::ROOK] = 0x8100000000000000;
		_bitboards[Color::WHITE][PieceType::QUEEN] = 0x0800000000000000;
		_bitboards[Color::WHITE][PieceType::KING] = 0x1000000000000000;

		/* ----- BLACK PIECES ----- */
		_bitboards[Color::BLACK][PieceType::PAWN] = 0x000000000000ff00;
		_bitboards[Color::BLACK][PieceType::KNIGHT] = 0x0000000000000042;
		_bitboards[Color::BLACK][PieceType::BISHOP] = 0x0000000000000024;
		_bitboards[Color::BLACK][PieceType::ROOK] = 0x0000000000000081;
		_bitboards[Color::BLACK][PieceType::QUEEN] = 0x000000000000008;
		_bitboards[Color::BLACK][PieceType::KING] = 0x000000000000010;

		updateOccupancyBoards();
	}

	void BitboardSet::clearPos(const int x, const int y)
	{
		const Bitboard binaryPosition = ~positionToBitboard(x, y);

		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_bitboards[color][pieceType] &= binaryPosition;
			}
		}
		updateOccupancyBoards();
	}

	void BitboardSet::clearPos(const Position& pos)
	{
		clearPos(pos.x, pos.y);
	}

	void BitboardSet::clearPos(const int x, const int y, const Color color)
	{
		const Bitboard binaryPosition = ~positionToBitboard(x, y);

		for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
		{
			_bitboards[color][pieceType] &= binaryPosition;
		}
		updateOccupancyBoards(color);
	}

	void BitboardSet::clearPos(const Position& pos, const Color color)
	{
		clearPos(pos.x, pos.y, color);
	}

	void BitboardSet::clearPos(const int x, const int y, const Color color, const PieceType pieceType)
	{
		_bitboards[color][pieceType] &= ~positionToBitboard(x, y);
		updateOccupancyBoards(color);
	}

	void BitboardSet::clearPos(const Position& pos, const Color color, const PieceType pieceType)
	{
		clearPos(pos.x, pos.y, color, pieceType);
	}

	void BitboardSet::clear()
	{
		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_bitboards[color][pieceType] &= 0;
			}
			_colorOccupancyBoards[color] = 0;
		}
		_allOccupancyBoard = 0;
	}

	void BitboardSet::addPiece(const int x, const int y, const Color color, const PieceType pieceType)
	{
		_bitboards[color][pieceType] |= positionToBitboard(x, y);
		updateOccupancyBoards(color);
	}

	void BitboardSet::addPiece(const Position& pos, const Color color, const PieceType pieceType)
	{
		addPiece(pos.x, pos.y, color, pieceType);
	}

	BitboardSet& BitboardSet::operator=(const BitboardSet& rightOperand)
	{
		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_bitboards[color][pieceType] = rightOperand._bitboards[color][pieceType];
			}
			_colorOccupancyBoards[color] = rightOperand._colorOccupancyBoards[color];
		}
		_allOccupancyBoard = rightOperand._allOccupancyBoard;

		return *this;
	}

	bool BitboardSet::operator==(const BitboardSet& rightOperand) const
	{
		bool result = true;
		int color = Color::WHITE;

		while (color < COLOR_COUNT && result)
		{
			int pieceType = PieceType::PAWN;

			while (pieceType < PIECE_TYPE_COUNT && result)
			{
				result = _bitboards[color][pieceType] == rightOperand._bitboards[color][pieceType];
				pieceType++;
			}
			color++;
		}

		return result;
	}

	bool BitboardSet::operator!=(const BitboardSet& rightOperand) const
	{
		return !(*this == rightOperand);
	}

	void BitboardSet::updateOccupancyBoards()
	{
		_allOccupancyBoard = 0;
		for (int color = Color::WHITE; color < COLOR_COUNT; color++)
		{
			_colorOccupancyBoards[color] = 0;
			for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				_colorOccupancyBoards[color] |= _bitboards[color][pieceType];
			}
			_allOccupancyBoard |= _colorOccupancyBoards[color];
		}
	}

	void BitboardSet::updateOccupancyBoards(const Color color)
	{
		_colorOccupancyBoards[color] = 0;
		for (int pieceType = PieceType::PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
		{
			_colorOccupancyBoards[color] |= _bitboards[color][pieceType];
		}
		_allOccupancyBoard = _colorOccupancyBoards[Color::WHITE] | _colorOccupancyBoards[Color::BLACK];
	}
}