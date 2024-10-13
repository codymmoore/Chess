#include "bitboard.h"

#include "utility.h"
#include "../constants.h"

using std::cout;
using std::endl;
using std::string;

namespace util
{
	Bitboard::Bitboard()
	{
		for (int color = WHITE; color < BLACK; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				m_board[color][pieceType] = 0;
			}
		}
	}

	Bitboard::Bitboard(const Bitboard& source)
	{
		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				m_board[color][pieceType] = source.m_board[color][pieceType];
			}
		}
	}

	bool Bitboard::posIsOccupied(const int x, const int y) const
	{
		uint64_t binaryPosition = positionToBitboard(x, y);
		return (getOccupiedSpaces() & binaryPosition) == binaryPosition;
	}

	bool util::Bitboard::posIsOccupied(const Position& position) const
	{
		return posIsOccupied(position.x, position.y);
	}

	bool Bitboard::posIsOccupied(const int x, const int y, const Color color) const
	{
		uint64_t binaryPosition = positionToBitboard(x, y);
		return (getColorBoard(color) & binaryPosition) == binaryPosition;
	}

	bool Bitboard::posIsOccupied(const Position& pos, const Color color) const
	{
		return posIsOccupied(pos.x, pos.y, color);
	}

	bool Bitboard::posIsOccupied(const int x, const int y, const PieceType pieceType) const
	{
		uint64_t binaryPosition = positionToBitboard(x, y);
		return (getPieceBoard(pieceType) & binaryPosition) == binaryPosition;
	}

	bool Bitboard::posIsOccupied(const Position& pos, const PieceType pieceType) const
	{
		return posIsOccupied(pos.x, pos.y, pieceType);
	}

	bool Bitboard::posIsOccupied(const int x, const int y, const Color color, const PieceType pieceType) const
	{
		uint64_t binaryPosition = positionToBitboard(x, y);
		return (m_board[color][pieceType] & binaryPosition) == binaryPosition;
	}

	bool Bitboard::posIsOccupied(const Position& pos, const Color color, const PieceType pieceType) const
	{
		return posIsOccupied(pos.x, pos.y, color, pieceType);
	}

	uint64_t Bitboard::getOccupiedSpaces() const
	{
		uint64_t result = 0;

		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				result |= m_board[color][pieceType];
			}
		}

		return result;
	}

	uint64_t Bitboard::getColorBoard(const Color color) const
	{
		uint64_t result = 0;

		for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
		{
			result |= m_board[color][pieceType];
		}

		return result;
	}

	uint64_t Bitboard::getPieceBoard(const PieceType pieceType) const
	{
		uint64_t result = 0;

		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			result |= m_board[color][pieceType];
		}

		return result;
	}

	uint64_t Bitboard::getColorPieceBoard(const Color color, const PieceType pieceType) const
	{
		return m_board[color][pieceType];
	}

	void Bitboard::print() const
	{
		for (int y = 0; y < RANK_COUNT; y++)
		{
			for (int x = 0; x < FILE_COUNT; x++)
			{
				bool occupied = false;
				char outputChar = '-';

				int color = WHITE;
				while (color < COLOR_COUNT && !occupied)
				{
					int pieceType = PAWN;
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

	void Bitboard::populateBoard()
	{
		/* ----- WHITE PIECES ----- */
		m_board[WHITE][PAWN] = 0x00ff000000000000;
		m_board[WHITE][KNIGHT] = 0x4200000000000000;
		m_board[WHITE][BISHOP] = 0x2400000000000000;
		m_board[WHITE][ROOK] = 0x8100000000000000;
		m_board[WHITE][QUEEN] = 0x0800000000000000;
		m_board[WHITE][KING] = 0x1000000000000000;

		/* ----- BLACK PIECES ----- */
		m_board[BLACK][PAWN] = 0x000000000000ff00;
		m_board[BLACK][KNIGHT] = 0x0000000000000042;
		m_board[BLACK][BISHOP] = 0x0000000000000024;
		m_board[BLACK][ROOK] = 0x0000000000000081;
		m_board[BLACK][QUEEN] = 0x000000000000008;
		m_board[BLACK][KING] = 0x000000000000010;
	}

	void Bitboard::clearPos(const int x, const int y)
	{
		uint64_t binaryPosition = positionToBitboard(x, y);

		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				m_board[color][pieceType] &= ~binaryPosition;
			}
		}
	}

	void Bitboard::clearPos(const Position& pos)
	{
		clearPos(pos.x, pos.y);
	}

	void Bitboard::clearPos(const int x, const int y, const Color color)
	{
		uint64_t binaryPosition = positionToBitboard(x, y);

		for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
		{
			m_board[color][pieceType] &= ~binaryPosition;
		}
	}

	void Bitboard::clearPos(const Position& pos, const Color color)
	{
		clearPos(pos.x, pos.y, color);
	}

	void Bitboard::clearPos(const int x, const int y, const PieceType pieceType)
	{
		uint64_t binaryPosition = positionToBitboard(x, y);

		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			m_board[color][pieceType] &= ~binaryPosition;
		}
	}

	void Bitboard::clearPos(const Position& pos, const PieceType pieceType)
	{
		clearPos(pos.x, pos.y, pieceType);
	}

	void Bitboard::clearPos(const int x, const int y, const Color color, const PieceType pieceType)
	{
		m_board[color][pieceType] &= ~positionToBitboard(x, y);
	}

	void Bitboard::clearPos(const Position& pos, const Color color, const PieceType pieceType)
	{
		clearPos(pos.x, pos.y, color, pieceType);
	}

	void Bitboard::clearColor(const Color color)
	{
		for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
		{
			m_board[color][pieceType] &= 0;
		}
	}

	void Bitboard::clearPieceType(const PieceType pieceType)
	{
		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			m_board[color][pieceType] &= 0;
		}
	}

	void Bitboard::clear()
	{
		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				m_board[color][pieceType] &= 0;
			}
		}
	}

	void Bitboard::addPiece(const int x, const int y, const Color color, const PieceType pieceType)
	{
		// As of right now, allows to place a piece in a position occupied by a different piece type
		m_board[color][pieceType] |= positionToBitboard(x, y);
	}

	void Bitboard::addPiece(const Position& pos, const Color color, const PieceType pieceType)
	{
		addPiece(pos.x, pos.y, color, pieceType);
	}

	Bitboard& Bitboard::operator=(const Bitboard& rightOperand)
	{
		for (int color = WHITE; color < COLOR_COUNT; color++)
		{
			for (int pieceType = PAWN; pieceType < PIECE_TYPE_COUNT; pieceType++)
			{
				m_board[color][pieceType] = rightOperand.m_board[color][pieceType];
			}
		}

		return *this;
	}

	bool Bitboard::operator==(const Bitboard& rightOperand) const
	{
		bool result = true;
		int color = WHITE;

		while (color < COLOR_COUNT && result)
		{
			int pieceType = PAWN;

			while (pieceType < PIECE_TYPE_COUNT && result)
			{
				result = m_board[color][pieceType] == rightOperand.m_board[color][pieceType];
				pieceType++;
			}
			color++;
		}

		return result;
	}

	bool Bitboard::operator!=(const Bitboard& rightOperand) const
	{
		return !(*this == rightOperand);
	}
}