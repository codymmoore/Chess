#ifndef CHESS_H
#define CHESS_H

#include <deque>
#include <map>
#include <string>
#include "BitBoard.h"

/* ---------- GLOBAL CONSTANTS ---------- */
const double TOTAL_PLAYER_TURN_TIME = 15.0 * 60.0 * 1000000000.0; // 15 minutes converted to nanoseconds

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct: PieceNode
///
/// \brief:  Used to fille piece vector in ChessState class
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct PieceNode
{
    Position m_position;
    PieceType m_pieceType;

    PieceNode();
    PieceNode(const PieceNode& source) = default;
    PieceNode(const int xCoord, const int yCoord, const PieceType pieceType);
    PieceNode(const Position& position, const PieceType pieceType);

    PieceNode& operator=(const PieceNode& rightOperand);

    bool operator==(const PieceNode& rightOperand) const;
    bool operator!=(const PieceNode& rightOperand) const;
};

// Used to output PieceNode objects
std::ostream& operator<<(std::ostream& out, const PieceNode& piece);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct:  MoveNode
///
/// \brief:   Used to fill move history structure in ChessState class
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct MoveNode
{
    Position m_prevPos;
    Position m_currPos;
    Color m_color;
    PieceType m_pieceType;

    MoveNode() = default;
    MoveNode(const MoveNode& source) = default;
    MoveNode(const Color color, const PieceNode& piece, const Position& currPos);
    MoveNode(const Position& prevPos,
             const Position& currPos,
             const Color color,
             const PieceType pieceType);

    MoveNode& operator=(const MoveNode& rightOperand);

    bool operator==(const MoveNode& rightOperand) const;
    bool operator!=(const MoveNode& rightOperand) const;
};

// Used to output MoveNode objects
std::ostream& operator<<(std::ostream& out, const MoveNode& move);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \class:  ChessState
///
/// \brief:  Representation of chess game state
///
/// \note: board is displayed form White's perspective
///
/////////////////////////////////////////////////////////////////////////////////////////////
class ChessState
{
    #ifdef _DEBUG
        public:
    #endif
    BitBoard m_board;
    std::vector<PieceNode> m_whitePieces; // List of white pieces present on board
    std::vector<PieceNode> m_blackPieces; // List of black pieces present on board
    std::deque<MoveNode>  m_moveHistory;  // Contains previous 8 moves
    Color m_winner, m_nextTurn;
    int m_numHalfTurns, // Number of half turns since last capture or pawn advance
        m_numFullTurns; // Number of full moves (starts at 1; increment after Black's move)
    // double m_wTimeRemaining, m_bTimeRemaining; // Remaining turn time for each player
    // Used to determine whether castling is valid or not
    bool m_wKingSideCastle,
         m_wQueenSideCastle,
         m_bKingSideCastle,
         m_bQueenSideCastle;

public:
    static constexpr int PAWN_START_ROW[NUM_COLORS] = {NUM_RANKS - 2, 1};

    // Constructors
    ChessState();
    ChessState(const std::string& gameState);
    ChessState(const ChessState& source);
    ~ChessState();

    // Getters
    Color getNextTurn() const;
    Color getWinner() const;
    const std::vector<PieceNode>& getWhitePieces() const;
    const std::vector<PieceNode>& getBlackPieces() const;
    std::string getFenString() const;

    // Modifiers
    void setState(const std::string& gameState);
    void clear();

    void print() const;
    void printDebug() const;

    friend class Move;
    friend class Agent;
};

#endif // CHESS_H