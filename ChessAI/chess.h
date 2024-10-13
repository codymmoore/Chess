#ifndef CHESS_H
#define CHESS_H

#include <deque>
#include <string>
#include <vector>

#include "constants.h"
#include "util/position.h"
#include "util/bitboard.h"

/* ---------- GLOBAL CONSTANTS ---------- */
const double TOTAL_PLAYER_TURN_TIME = 15.0 * 60.0 * 1000000000.0; // 15 minutes converted to nanoseconds
const unsigned int MAX_MOVE_HISTORY_SIZE = 8;

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct: PieceNode
///
/// \brief:  Used to fille piece vector in ChessState class
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct PieceNode
{
	util::Position m_position;
	PieceType m_pieceType;

	PieceNode();
	PieceNode(const PieceNode& source) = default;
	PieceNode(const int xCoord, const int yCoord, const PieceType pieceType);
	PieceNode(const util::Position& position, const PieceType pieceType);

	PieceNode& operator=(const PieceNode& rightOperand);

	bool operator==(const PieceNode& rightOperand) const;
	bool operator!=(const PieceNode& rightOperand) const;
};

// Used to output PieceNode objects
std::ostream& operator<<(std::ostream& out, const PieceNode& piece);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct:  MoveHistoryNode
///
/// \brief:   Used to fill move history structure in ChessState class
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct MoveHistoryNode
{
	util::Position m_prevPos;
	util::Position m_currPos;
	Color m_color;
	PieceType m_pieceType;

	MoveHistoryNode() = default;
	MoveHistoryNode(const MoveHistoryNode& source) = default;
	MoveHistoryNode(const Color color, const PieceNode& piece, const util::Position& currPos);
	MoveHistoryNode(const util::Position& prevPos,
		const util::Position& currPos,
		const Color color,
		const PieceType pieceType);

	MoveHistoryNode& operator=(const MoveHistoryNode& rightOperand);

	bool operator==(const MoveHistoryNode& rightOperand) const;
	bool operator!=(const MoveHistoryNode& rightOperand) const;
};

// Used to output MoveHistoryNode objects
std::ostream& operator<<(std::ostream& out, const MoveHistoryNode& move);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct:  MoveNode
///
/// \brief:   Represents a move
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct MoveNode
{
	util::Position m_source;
	util::Position m_destination;
	PieceType m_promotion;

	MoveNode() = default;
	MoveNode(const MoveNode& source) = default;
	MoveNode(const util::Position& source,
		const util::Position& destination,
		const PieceType promotion);

	MoveNode& operator=(const MoveNode& rightOperand);

	bool operator==(const MoveNode& rightOperand) const;
	bool operator!=(const MoveNode& rightOperand) const;
};

// Used to output MoveHistoryNode objects
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
	util::Bitboard m_board;
	std::vector<PieceNode> m_whitePieces; // List of white pieces present on board
	std::vector<PieceNode> m_blackPieces; // List of black pieces present on board
	std::deque<MoveHistoryNode>  m_moveHistory;  // Contains previous 8 moves
	Color m_winner, m_nextTurn;
	int m_numHalfTurns, // Number of half turns since last capture or pawn advance
		m_numFullTurns; // Number of full moves (starts at 1; increment after Black's move)
	// double m_wTimeRemaining, m_bTimeRemaining; // Remaining turn time for each player
	// Used to determine whether castling is valid or not
	bool m_wKingSideCastle,
		m_wQueenSideCastle,
		m_bKingSideCastle,
		m_bQueenSideCastle;

	void initialize();

public:
	static constexpr int PAWN_START_ROW[COLOR_COUNT] = { RANK_COUNT - 2, 1 };

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
	const util::Bitboard& getBoard() const;

	// Modifiers
	void setState(const std::string& gameState);
	void clear();
	void reset();

	void print() const;
	void printDebug() const;

	friend class Move;
	friend class Agent;
};

#endif // CHESS_H