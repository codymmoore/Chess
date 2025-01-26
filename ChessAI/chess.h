#pragma once

#include <deque>
#include <string>
#include <vector>
#include <optional>

#include "constants.h"
#include "util/position.h"
#include "util/bitboard/bitboardSet.h"
#include "move/move.h"

const double TOTAL_PLAYER_TURN_TIME = 15.0 * 60.0 * 1000000000.0; // 15 minutes converted to nanoseconds
const unsigned int MAX_MOVE_HISTORY_SIZE = 8;
constexpr int PAWN_START_ROW[COLOR_COUNT] = { RANK_COUNT - 2, 1 };
const util::Position KING_START_POS[COLOR_COUNT] = {
	util::Position(4, RANK_COUNT - 1),
	util::Position(4, 0)
};

/**
 * Struct to contain historic move information.
 */
struct MoveHistoryNode
{
	util::Position source;
	util::Position destination;
	Color player;
	PieceType pieceType;

	MoveHistoryNode() = default;
	MoveHistoryNode(const MoveHistoryNode& source) = default;
	MoveHistoryNode(const util::Position& source,
		const util::Position& destination,
		const Color color,
		const PieceType pieceType);

	MoveHistoryNode& operator=(const MoveHistoryNode& rightOperand);

	bool operator==(const MoveHistoryNode& rightOperand) const;
	bool operator!=(const MoveHistoryNode& rightOperand) const;
};
std::ostream& operator<<(std::ostream& out, const MoveHistoryNode& move);

/**
 * Class representing a state within a game of Chess.
 */
class ChessState
{
public:
	/**
	 * Create a default instance of ChessState.
	 */
	ChessState();

	/**
	 * Create a new Chess state based on a FEN string.
	 *
	 * https://www.chess.com/terms/fen-chess
	 *
	 * \param fenString FEN string describing a game state
	 */
	ChessState(const std::string& fenString);

	/**
	 * Create a copy of a Chess state.
	 *
	 * \param source the Chess state being copied
	 */
	ChessState(const ChessState& source);

	/**
	 * Destruct the current ChessState instance.
	 */
	~ChessState();

	/**
	 * Get the next player's turn.
	 *
	 * \return the player whose turn it is to move
	 */
	Color getNextTurn() const;

	/**
	 * Get the winner.
	 *
	 * \return Color::BLACK or Color::WHITE if a player has one, Color::NEUTRAL if a tie has occurred, std::nullopt if game is in progress
	 */
	std::optional<Color> getWinner() const;

	/**
	 * Get the FEN string representation of the current game state.
	 *
	 * https://www.chess.com/terms/fen-chess
	 *
	 * \return FEN string representation of the current game state
	 */
	std::string getFenString() const;

	/**
	 * Get the BitboardSet representation of the current board state.
	 *
	 * \return BitboardSet representation of the current board state
	 */
	const util::bitboard::BitboardSet& getBoard() const;

	/**
	 * Get the move history for the current game state.
	 *
	 * \return container for the last eight moves
	 */
	const std::deque<MoveHistoryNode>& getMoveHistory() const;

	/**
	 * Get the number of half turns.
	 *
	 * \return the number of halfturns
	 */
	int getHalfTurnCount() const;

	/**
	 * Get the number of full turns.
	 *
	 * \return  the number of full turns
	 */
	int getFullTurnCount() const;

	/**
	 * Determine if the specified player can still perform a king-side castle.
	 *
	 * \param player the Color the player is representing
	 * \return true if player can still king-side castle, false otherwise
	 */
	bool canKingSideCastle(const Color player) const;

	/**
	 * Determine if the specified player can still perform a queen-side castle.
	 *
	 * \param player the Color the player is representing
	 * \return true if player can still queen-side castle, false otherwise
	 */
	bool canQueenSideCastle(const Color player) const;

	/**
	 * Moves a piece and updates the game state.
	 *
	 * \param player owner of the piece being move
	 * \param move contains move information
	 * \param promotion piece type to promote to; default is queen
	 */
	void update(const Color player, const move::Move& move, const PieceType promotion = PieceType::QUEEN);

	/**
	 * Moves a piece and updates the game state.
	 *
	 * \param player owner of the piece being move
	 * \param piece the piece being moved
	 * \param destination the position the piece is being moved to
	 * \param promotion piece type to promote to; default is queen
	 */
	void update(const Color player, const util::Position& source, const util::Position& destination, const PieceType promotion = PieceType::QUEEN);

	/**
	 * Clear the current game state.
	 */
	void clear();

	/**
	 * Reset the current game state to a new game.
	 */
	void reset();

	/**
	 * Print the current game state information.
	 */
	void print() const;

	#ifdef _DEBUG
	/**
	 * Print the current game state information for debugging.
	 */
	void printDebug() const;
	#endif
private:
	/**
	 * Initialize a new ChessState.
	 */
	void initialize();

	/**
	 * Set the current game state based on a FEN string.
	 *
	 * https://www.chess.com/terms/fen-chess
	 *
	 * \param fenString FEN string describing a game state
	 */
	void setState(const std::string& fenString);

	util::bitboard::BitboardSet _board;
	std::deque<MoveHistoryNode> _moveHistory;
	std::optional<Color> _winner;
	Color _nextTurn;
	int _halfTurnCount, // Number of half turns since last capture or pawn advance
		_fullTurnCount; // Number of full moves (starts at 1; increment after Black's move)
	// TODO double m_wTimeRemaining, m_bTimeRemaining;
	bool _wKingSideCastle,
		_wQueenSideCastle,
		_bKingSideCastle,
		_bQueenSideCastle;
};
