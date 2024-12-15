#pragma once

#include "move/move.h"
#include "chess.h"
#include "constants.h"
#include <unordered_map>
#include <map>

typedef std::multimap<double, int, std::greater<double>> MoveIndexMap;

/**
 * Class used to determine optimal moves in a game of Chess.
 */
class Agent
{
public:
	Agent() = delete;
	Agent(const Agent& source) = delete;

	/**
	 * Creates a new Agent.
	 *
	 * \param chessState game state
	 * \param player the player the agent will be playing as
	 * \param quiescentSearchDepth the search depth for "quiet" states
	 * \param depthLimit the absolute limit for seach depth
	 */
	Agent(ChessState& chessState, const Color player, const int quiescentSearchDepth, const int depthLimit);

	/**
	 * Retrieves the player.
	 *
	 * \return the color the agent is playing for
	 */
	Color getPlayer() const;

	/**
	 * Determine the best move for the current game state.
	 *
	 * \return optimal move
	 */
	move::Move getMove();

	/**
	 * Determine the best move for the current game state.
	 *
	 * \param timeRemaining the time remaining for the player
	 * \return optimal move
	 */
	move::Move getMove(const double timeRemaining);

private:
	/**
	 * Calculates a score for the given game state based on how desirable it is for the given player.
	 *
	 * \param chessState the game state being scored
	 * \param player the player the score is being calculated for
	 * \return score of the game state
	 */
	double evaluateGameState(const ChessState& chessState, const Color player) const;

	/**
	 * Determines when to stop evaluating a game state for the best move.
	 *
	 * \param timeElapsed the time spent determining the current move
	 * \param timeRemaining the total remaining time the player has to finish the game
	 * \return true to continue evaluation, false otherwise
	 */
	bool timeHeuristic(const double timeElapsed, const double timeRemaining) const;

	/**
	 * Determines if a game state is "quiet" or unlikely to result in meaningful impacts to the player.
	 *
	 * \param game game state
	 * \param validMoves all valid moves for the player in the given game state
	 * \return true if game state is "quiet", false otherwise
	 */
	bool isQuiescent(const ChessState& game, const std::vector<move::Move>& validMoves) const;

	/**
	 * Scores a move based on how desireable it is for the given player.
	 *
	 * \param chessState game state
	 * \param player the player the score is being calculated for
	 * \param move the move being scored
	 * \return score of the move
	 */
	double getMoveValue(const ChessState& chessState, const Color player, const move::Move& move) const;

	/**
	 * Scores each given move and places it in a map with its index to be sorted.
	 *
	 * \param chessState game state
	 * \param player the player whose moves are being scored and sorted
	 * \param moves the moves being scored and sorted
	 * \return ordered map containing score and index for each move
	 */
	MoveIndexMap getOrderedMoveIndexMap(const ChessState& chessState, const Color player, const std::vector<move::Move>& moves) const;

	/**
	 * Calculates the score of a game state by recursively exploring possible moves.
	 *
	 * \param player the current turn's player
	 * \param chessState game state
	 * \param searchDepth te current search depth
	 * \param alpha the greatest value that can be guaranteed by the player; used for pruning
	 * \param beta the greatest value that can be guaranteed by the enemy; used for pruning
	 * \return the score for the given game state
	 */
	double getNegaMaxValue(const Color player, const ChessState& chessState, const int searchDepth, double alpha, double beta);

	const Color _player;
	const ChessState& _chessState;
	int _quiescentSearchDepth;
	int _depthLimit;
	std::unordered_map<move::Move, double, move::Move::MoveHasher> _allyHistoryTable, _enemyHistoryTable;
};