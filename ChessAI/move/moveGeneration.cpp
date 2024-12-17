#include "moveGeneration.h"

#include <vector>
#include <cmath>
#include <deque>

#include "../enum.h"
#include "../util/bitboard/bitboardSet.h"
#include "../util/bitboard/bitboardUtil.h"
#include "../util/utility.h"
#include "move.h"

using namespace util;
using namespace util::bitboard;

using namespace std;

namespace move
{
	std::vector<Move> getMovesFromBitboard(Bitboard bitboard, const int deltaX, const int deltaY)
	{
		std::vector<Move> result;

		while (bitboard)
		{
			const int lsbIndex = popLsb(bitboard);
			const int x = lsbIndex % FILE_COUNT;
			const int y = lsbIndex / FILE_COUNT;
			const Position source(x + deltaX, y + deltaY);
			const Position destination(x, y);
			result.emplace_back(source, destination);
		}

		return result;
	}

	std::vector<Move> generatePawnMoves(const ChessState& chessState, const Color player)
	{
		static const Bitboard WHITE_START_ROW = 0x00ff000000000000;
		static const Bitboard BLACK_START_ROW = 0x000000000000ff00;

		std::vector<Move> result;
		const BitboardSet& board = chessState.getBoard();
		const Bitboard pawnBoard = board.getBitboard(player, PieceType::PAWN);

		if (pawnBoard == 0)
		{
			return result;
		}

		const int COLOR_COEFFICIENT = player == Color::WHITE ? 1 : -1;
		const auto forward = [player](const int magnitude) {
			return player == Color::WHITE ? up(magnitude) : down(magnitude);
			};
		const Bitboard startRow = player == Color::WHITE ? WHITE_START_ROW : BLACK_START_ROW;

		const Bitboard singlePushes = shiftBitboard(pawnBoard, forward(1)) & ~board.getOccupancyBoard();
		if (singlePushes != 0) {
			std::vector<Move> moves = getMovesFromBitboard(singlePushes, 0, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		Bitboard doublePushes = singlePushes & shiftBitboard(startRow, forward(1));
		doublePushes = shiftBitboard(doublePushes, forward(1)) & ~board.getOccupancyBoard();
		if (doublePushes != 0) {
			std::vector<Move> moves = getMovesFromBitboard(doublePushes, 0, 2 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		Bitboard opponentOccupancyBoard = board.getOccupancyBoard(~player);

		const std::deque<MoveHistoryNode> moveHistory = chessState.getMoveHistory();
		if (!moveHistory.empty()) {
			const MoveHistoryNode& prevMove = moveHistory.back();
			if (prevMove.m_pieceType == PieceType::PAWN)
			{
				const int prevY = prevMove.m_prevPos.y;
				const int currY = prevMove.m_currPos.y;

				if (abs(currY - prevY) == 2)
				{
					const Position enPassantPos = prevMove.m_prevPos + (prevMove.m_color == Color::WHITE ? Position::UP : Position::DOWN);
					opponentOccupancyBoard |= positionToBitboard(enPassantPos);
				}
			}
		}

		const Bitboard leftCaptures = shiftBitboard(pawnBoard, { forward(1), left(1) }) & opponentOccupancyBoard;
		if (leftCaptures != 0) {
			std::vector<Move> moves = getMovesFromBitboard(leftCaptures, 1, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		const Bitboard rightCaptures = shiftBitboard(pawnBoard, { forward(1), right(1) }) & opponentOccupancyBoard;
		if (rightCaptures != 0) {
			std::vector<Move> moves = getMovesFromBitboard(rightCaptures, -1, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		return result;
	}

	std::vector<Move> generateKnightMoves(const ChessState& chessState, const Color player)
	{
		static std::vector<Shift> knightShifts = {
			Shift({ up(2), left(1) }),
			Shift({ up(1), left(2) }),
			Shift({ up(2), right(1) }),
			Shift({ up(1), right(2) }),
			Shift({ down(2), left(1) }),
			Shift({ down(1), left(2) }),
			Shift({ down(2), right(1) }),
			Shift({ down(1), right(2) })
		};

		std::vector<Move> result;
		const BitboardSet& board = chessState.getBoard();
		const Bitboard knightBoard = board.getBitboard(player, PieceType::KNIGHT);

		if (knightBoard == 0)
		{
			return result;
		}

		const Bitboard playerOccupancyBoard = board.getOccupancyBoard(player);

		for (const Shift& shift : knightShifts)
		{
			const Bitboard shiftedKnightBoard = shiftBitboard(knightBoard, shift) & ~playerOccupancyBoard;
			if (shiftedKnightBoard != 0)
			{
				std::vector<Move> moves = getMovesFromBitboard(shiftedKnightBoard, -shift.horizontal, -shift.vertical);
				result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
			}
		}

		return result;
	}
}
