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

	{
		return !(*this == rightOperand);
	}

namespace move
{
	std::vector<Move> getMovesFromBitboard(Bitboard bitboard, const int deltaX, const int deltaY)
	{
		std::vector<Move> result;

		int i = 0;
		while (bitboard)
		{
			if (bitboard & 1)
			{
				const int x = i % 8;
				const int y = i / 8;
				const Position source(x + deltaX, y + deltaY);
				const Position destination(x, y);
				result.emplace_back(source, destination);
			}
			bitboard >>= 1;
			i++;
		}

		return result;
	}

	std::vector<Move> generatePawnMoves(const ChessState& chessState, const Color player)
	{
		static const Bitboard WHITE_START_ROW = 0x00ff000000000000;
		static const Bitboard BLACK_START_ROW = 0x000000000000ff00;

		std::vector<Move> result;

		const int COLOR_COEFFICIENT = player == Color::WHITE ? 1 : -1;
		const Shift FORWARD = player == Color::WHITE ? Shift::UP : Shift::DOWN;
		const BitboardSet& board = chessState.getBoard();
		const Bitboard pawnBoard = board.getBitboard(player, PieceType::PAWN);
		const Bitboard startRow = player == Color::WHITE ? WHITE_START_ROW : BLACK_START_ROW;

		const Bitboard singlePushes = shift(pawnBoard, FORWARD) & ~board.getOccupancyBoard();
		if (singlePushes != 0) {
			std::vector<Move> moves = getMovesFromBitboard(singlePushes, 0, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		Bitboard doublePushes = singlePushes & shift(startRow, FORWARD);
		doublePushes = shift(doublePushes, FORWARD) & ~board.getOccupancyBoard();
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

		const Bitboard leftCaptures = shift(pawnBoard, FORWARD + Shift::LEFT) & opponentOccupancyBoard;
		if (leftCaptures != 0) {
			std::vector<Move> moves = getMovesFromBitboard(leftCaptures, -1, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		const Bitboard rightCaptures = shift(pawnBoard, FORWARD + Shift::RIGHT) & opponentOccupancyBoard;
		if (rightCaptures != 0) {
			std::vector<Move> moves = getMovesFromBitboard(rightCaptures, 1, 1 * COLOR_COEFFICIENT);
			result.insert(result.end(), std::make_move_iterator(moves.begin()), std::make_move_iterator(moves.end()));
		}

		return result;
	}
}
