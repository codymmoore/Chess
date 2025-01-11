#include "agent.h"

#include "enum.h"
#include "util/utility.h"
#include "move/moveUtil.h"
#include "util/bitboard/bitboardSet.h"

#include <chrono>

using namespace util;
using move::Move;

using util::bitboard::BitboardSet;

typedef std::chrono::high_resolution_clock::time_point chronoTime;
typedef std::chrono::duration<std::chrono::nanoseconds> chronoDuration;

const double PIECE_VALUES[PIECE_TYPE_COUNT] = { 2, 4, 4, 6, 10, 0 }, CHECK_VALUE = 1;
const double FULL_TEAM_VALUE = PIECE_VALUES[PAWN] * FILE_COUNT
+ PIECE_VALUES[KNIGHT] * 2
+ PIECE_VALUES[BISHOP] * 2
+ PIECE_VALUES[ROOK] * 2
+ PIECE_VALUES[QUEEN];

Agent::Agent(ChessState& chessState, const Color player, const int quiescentSearchDepth, const int depthLimit) :
	_chessState(chessState),
	_player(player),
	_quiescentSearchDepth(quiescentSearchDepth),
	_depthLimit(depthLimit)
{
	if (depthLimit < quiescentSearchDepth)
	{
		throw std::exception("depthLimit must be greater than or equal to quiescentSearchDepth");
	}
}

Color Agent::getPlayer() const
{
	return _player;
}

Move Agent::getMove()
{
	Move result;
	double maxValue = INT_MIN;

	const std::vector<Move> moves = move::getValidMoves(_chessState, _player);
	for (const Move& move : moves)
	{
		ChessState newState(_chessState);
		move::makeMove(_player, move.source, move.destination, newState);

		const double newValue = -getNegaMaxValue(~_player, newState, 1, -DBL_MAX, DBL_MAX);

		if (newValue > maxValue)
		{
			maxValue = newValue;
			result = move;
		}
	}

	return result;
}

Move Agent::getMove(const double timeRemaining)
{
	Move result;

	const chronoTime startTime = std::chrono::high_resolution_clock::now();
	double timeElapsed = 0.0;
	int depth = 1;

	do
	{
		result = getMove();
		timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
		depth += 1;
	} while (timeHeuristic(timeElapsed, timeRemaining));

	return result;
}

double Agent::evaluateGameState(const ChessState& chessState, const Color player) const
{
	double result = 0.0;
	const Color enemyPlayer = ~player;

	double capturePoints = FULL_TEAM_VALUE;
	const BitboardSet& board = chessState.getBoard();
	for (int i = PieceType::PAWN; i < PieceType::KING; i++)
	{
		const PieceType pieceType = (PieceType)i;
		const double pieceValue = PIECE_VALUES[pieceType];
		const Bitboard pieceBoard = board.getBitboard(player, pieceType);
		if (pieceBoard)
		{
			result += std::popcount(board.getBitboard(player, pieceType)) * pieceValue;
		}

		const Bitboard enemyPieceBoard = board.getBitboard(enemyPlayer, pieceType);
		if (enemyPieceBoard)
		{
			capturePoints -= std::popcount(board.getBitboard(enemyPlayer, pieceType)) * pieceValue;
		}
	}

	result += capturePoints;
	result += move::inCheck(enemyPlayer, chessState) * CHECK_VALUE;
	result -= move::inCheck(player, chessState) * CHECK_VALUE;

	return result;
}

bool Agent::timeHeuristic(const double timeElapsed, const double timeRemaining) const
{
	double turnTime = 0.01 * timeRemaining;

	return timeElapsed < turnTime;
}

bool Agent::isQuiescent(const ChessState& game, const std::vector<Move>& validMoves) const
{
	for (const Move& move : validMoves)
	{
		if (game.m_board.posIsOccupied(move.destination))
		{
			return false;
		}
	}

	return true;
}

double Agent::getMoveValue(const ChessState& chessState, const Color player, const Move& move) const
{
	static const double BASE_CAPTURE_SCORE = 1000;
	const Color enemyPlayer = ~player;
	const util::bitboard::BitboardSet& board = chessState.getBoard();
	double result = 0.0;

	if (board.posIsOccupied(move.destination))
	{
		const PieceType capturedPieceType = board.getPieceType(move.destination, enemyPlayer);
		result += BASE_CAPTURE_SCORE + PIECE_VALUES[capturedPieceType];
	}
	else
	{
		const int deltaY = move.destination.y - move.source.y;
		// piece is moving backwards
		if (deltaY < 0)
		{
			result -= 0.5;
		}
		// piece is moving forwards
		else
		{
			result += 0.5;
		}
	}

	const auto historyTable = _player == player ? _allyHistoryTable : _enemyHistoryTable;
	const auto it = historyTable.find(move);
	if (it != historyTable.end())
	{
		const double historyTableScore = it->second;
		result += historyTableScore;
	}

	return result;
}

MoveIndexMap Agent::getOrderedMoveIndexMap(const ChessState& chessState, const Color player, const std::vector<Move>& moves) const
{
	MoveIndexMap result;

	for (int i = 0; i < moves.size(); i++)
	{
		const Move& move = moves[i];
		result.insert({ getMoveValue(chessState, player, move), i });
	}

	return result;
}

double Agent::getNegaMaxValue(const Color player, const ChessState& chessState, const int searchDepth, double alpha, double beta)
{
	const Color enemyPlayer = ~player;
	const std::vector<Move> playerMoves = move::getValidMoves(chessState, player);
	double maxValue = -DBL_MAX;

	if (!playerMoves.empty())
	{
		if (searchDepth >= _depthLimit || (searchDepth >= _quiescentSearchDepth && isQuiescent(chessState, playerMoves)))
		{
			return evaluateGameState(chessState, player);
		}
		else
		{
			Move optimalMove;
			MoveIndexMap moveIndexMap = getOrderedMoveIndexMap(chessState, player, playerMoves);
			for (const auto& entry : moveIndexMap)
			{
				const Move& move = playerMoves[entry.second];
				ChessState gameCopy(chessState);

				move::makeMove(player, move, gameCopy);

				const double value = -getNegaMaxValue(enemyPlayer, gameCopy, searchDepth + 1, -beta, -alpha);

				if (value > maxValue)
				{
					maxValue = value;
					optimalMove = move;
				}

				alpha = std::max(value, alpha);
				if (alpha >= beta)
				{
					break;
				}
			}

			if (!chessState.getBoard().posIsOccupied(optimalMove.destination))
			{
				const int depthRemaining = _depthLimit - searchDepth;
				auto& historyTable = _player == player ? _allyHistoryTable : _enemyHistoryTable;
				auto it = historyTable.find(optimalMove);

				if (it == historyTable.end())
				{
					it = historyTable.insert({ optimalMove, 0 }).first;
				}
				it->second += depthRemaining * depthRemaining;
			}
		}
	}

	return maxValue;
}
