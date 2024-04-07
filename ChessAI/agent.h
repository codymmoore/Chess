#ifndef AGENT_H
#define AGENT_H

#include "move.h"
#include <unordered_map>
#include <climits>
#include <chrono>
#include <map>

// Typedefs
typedef std::chrono::high_resolution_clock::time_point chronoTime;
typedef std::chrono::duration<std::chrono::nanoseconds> chronoDuration;

/* ----- GLOBAL CONSTANTS ----- */
// Point values used for relative value heuristic
const int PIECE_VALUES[NUM_PIECE_TYPES] = {2, 4, 4, 6, 10, 0}, CHECK_VALUE  = 1;
const int FULL_TEAM_VALUE = PIECE_VALUES[PAWN] * NUM_FILES
                          + PIECE_VALUES[KNIGHT] * 2
                          + PIECE_VALUES[BISHOP] * 2
                          + PIECE_VALUES[ROOK] * 2
                          + PIECE_VALUES[QUEEN];
const int MAX_DEPTH = 4, MAX_QUIESCENCE_SEARCH_DEPTH = 4;
const double SQUASH_COEFFICIENT = 1.05;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:     squash_function(int)
///
/// \brief:    Normalizes positive integers between 0 and 1
///
/// \param [in]:    value : positive integer to be normalized
///
/// \return:        double
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double squash_function(const int value);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class:   Agent
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief:    Class used to contain chess-playing algorithms
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Agent
{
    Color m_player;                    // Player color (Black/White)
    ChessState& m_game;                // Reference to chess game
    std::vector<PieceNode>& m_allies;  // Reference to vector of player-controlled pieces
    std::vector<PieceNode>& m_enemies; // Reference to vector of opposing player's pieces
    // History tables used for move ordering; m_historyTable[<previous position>][<new position>] = <move rating>
    std::unordered_map<Position, std::unordered_map<Position, int, PositionHasher>, PositionHasher> m_allyHistoryTable, m_enemyHistoryTable;

#ifdef TESTING
    public:
#endif
    int pieceValueHeuristic(const ChessState& game) const;
    bool timeHeuristic(const double timeElapsed, const double timeRemaining) const;
    bool isQuiescent(const ChessState& game, const std::unordered_map<Position, std::vector<Position>, PositionHasher>& validMoves) const;
    double getMoveValue(const Color& player, const Position& prevPos, const Position& newPos, const ChessState& game) const;
    std::vector<std::pair<Position, Position>> orderMoves(const Color& player, 
        const std::unordered_map<Position, std::vector<Position>, PositionHasher>& validMoves, const ChessState& game) const;
    int getMaxValue(const ChessState& game, const int depthLimit);
    int getMaxValue(const ChessState& game, const int depthLimit, int alpha, int beta);
    int getMinValue(const ChessState& game, const int depthLimit);
    int getMinValue(const ChessState& game, const int depthLimit, int alpha, int beta);
    std::string depthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit);
    std::string prunedDepthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit);

public:
    Agent() = delete;
    Agent(const Agent& source) = delete;

    Agent(const Color player, ChessState& game);
    ~Agent();
    void clear();
    std::string makeRandomMove();
    std::string iterDepthLimitedMinimax();
    std::string prunedIterDepthLimitedMinimax();
    std::string iterTimeLimitedMinimax(const double timeRemaining);
};

#endif // AGENT_H