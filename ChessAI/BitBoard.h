#ifndef BITBOARD_H
#define BITBOARD_H

#include "utility.h"

/* ----- Global Constants ----- */
constexpr char PIECE_SYMBOLS[NUM_COLORS][NUM_PIECE_TYPES] = {{'P', 'N', 'B', 'R', 'Q', 'K'},  // White Symbols
                                                             {'p', 'n', 'b', 'r', 'q', 'k'}}; // Black Symbols

constexpr char PRETTY_PIECE_SYMBOLS[NUM_PIECE_TYPES][5] = {"♟ ", "♞ ", "♝ ", "♜ ", "♛ ", "♚ "};

/////////////////////////////////////////////////////////////////////////////////////////////
/// \class:  BitBoard
/////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief:  Bitboard representation of a chess board
///
/////////////////////////////////////////////////////////////////////////////////////////////
class BitBoard
{
//public:
    uint64_t m_board[NUM_COLORS][NUM_PIECE_TYPES];
public:
    BitBoard();
    BitBoard(const BitBoard& source);

    /* ----- Operators ----- */
    BitBoard& operator=(const BitBoard& rightOperand);
    bool operator==(const BitBoard& rightOperand) const;
    bool operator!=(const BitBoard& rightOperand) const;

    /* ----- Non-Modifiers ----- */
    bool posIsOccupied(const int x, const int y) const;
    bool posIsOccupied(const Position& pos) const;
    bool posIsOccupiedByColor(const int x, const int y, const Color color) const;
    bool posIsOccupiedByColor(const Position& pos, const Color color) const;
    bool posIsOccupiedByPiece(const int x, const int y, const PieceType pieceType) const;
    bool posIsOccupiedByPiece(const Position& pos, const PieceType pieceType) const;
    bool posIsOccupiedByColorPiece(const int x, const int y, const Color color, const PieceType pieceType) const;
    bool posIsOccupiedByColorPiece(const Position& pos, const Color color, const PieceType pieceType) const;
    uint64_t getOccupiedSpaces() const;
    uint64_t getColorBoard(const Color color) const;
    uint64_t getPieceBoard(const PieceType pieceType) const;
    uint64_t getColorPieceBoard(const Color color, const PieceType pieceType) const;
    void print() const;

    /* ----- Modifiers ----- */
    void populateBoard();
    void clearPos(const int x, const int y);
    void clearPos(const Position& pos);
    void clearPos(const int x, const int y, const Color color);
    void clearPos(const Position& pos, const Color color);
    void clearPos(const int x, const int y, const PieceType pieceType);
    void clearPos(const Position& pos, const PieceType pieceType);
    void clearPos(const int x, const int y, const Color color, const PieceType pieceType);
    void clearPos(const Position& pos, const Color color, const PieceType pieceType);
    void clearColor(const Color color);
    void clearPieceType(const PieceType pieceType);
    void clear();
    void addPiece(const int x, const int y, const Color color, const PieceType pieceType);
    void addPiece(const Position& pos, const Color color, const PieceType pieceType);
};

#endif // BITBOARD_H