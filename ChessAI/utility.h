#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <vector>
#include <cinttypes>
#include <cmath>

/* ---------- Global Constants ---------- */
const int NUM_RANKS = 8, // number of rows
          NUM_FILES = 8; // number of columns

/////////////////////////////////////////////////////////////////////////////////////////////
/// \struct:    Position
/////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief:     Struct used to contain cartesian coordinates
///
/////////////////////////////////////////////////////////////////////////////////////////////
struct Position
{
    int m_x, m_y;

    Position() = default;
    Position(const int x, const int y) : m_x(x), m_y(y) {}
    Position& operator=(const Position& rightOperand);
    Position& operator+=(const Position& rightOperand);
    Position operator+(const Position& rightOperand) const;
    Position& operator-=(const Position& rightOperand);
    Position operator-(const Position& rightOperand) const;
    Position& operator*=(const int multiplier);
    Position operator*(const int multiplier) const;
    bool operator==(const Position& rightOperand) const;
    bool operator!=(const Position& rightOperand) const;
};

// Used to output Position objects
std::ostream& operator<<(std::ostream& out, const Position& pos);

// Used for using Position as key in unordered_maps
struct PositionHasher
{
    std::size_t operator()(const Position& position) const;
};

// Directions
const Position UP(0, -1),
               DOWN(0, 1),
               LEFT(-1, 0),
               RIGHT(1, 0);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \enum:     Color
/////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief:    Enumeration used to represent player/piece color
///
/////////////////////////////////////////////////////////////////////////////////////////////
enum Color
{
    WHITE      = 0,
    BLACK      = 1,
    NUM_COLORS = 2,
    NEUTRAL    = 3
};

// Returns opposite color
Color operator~(const Color color);

// Converts Color to string
std::string toString(const Color& color);

/////////////////////////////////////////////////////////////////////////////////////////////
/// \enum:     PieceType
/////////////////////////////////////////////////////////////////////////////////////////////
///
/// \brief:    Enumeration used to represent piece type
///
/////////////////////////////////////////////////////////////////////////////////////////////
enum PieceType
{
    PAWN   = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK   = 3,
    QUEEN  = 4,
    KING   = 5,
    NUM_PIECE_TYPES = 6,
    NONE = 7
};

// Converts PieceType to string
std::string toString(const PieceType& pieceType);

// Split string into substrings using delimeter
std::vector<std::string> stringSplit(const std::string& originalString, const char delim);

// Convert cartesian coordinates to uint64_t used for bitboards
uint64_t coordToBit(const int x, const int y);

// Convert cartesian coordinates to uint64_t used for bitboards
uint64_t coordToBit(const Position& pos);

// Convert coordinates to string containing rank and file
std::string toFileAndRank(const int x, const int y);

// Convert Position object to string containing rank and file
std::string toFileAndRank(const Position& pos);

#endif // UTILITY_H