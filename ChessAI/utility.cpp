#include "utility.h"
#include <sstream>

// Used for hashing Position objects
const int X_SHIFT = static_cast<int>(log2(NUM_FILES) + 1);

/*
============================================================
-------------------- Position Functions --------------------
============================================================ */
Position& Position::operator=(const Position& rightOperand)
{
    m_x = rightOperand.m_x;
    m_y = rightOperand.m_y;

    return *this;
}

Position& Position::operator+=(const Position& rightOperand)
{
    m_x += rightOperand.m_x;
    m_y += rightOperand.m_y;

    return *this;
}

Position Position::operator+(const Position& rightOperand) const
{
    Position result(*this);
    result += rightOperand;

    return result;
}

Position& Position::operator-=(const Position& rightOperand)
{
    m_x -= rightOperand.m_x;
    m_y -= rightOperand.m_y;

    return *this;
}

Position Position::operator-(const Position& rightOperand) const
{
    Position result(*this);

    result -= rightOperand;
    
    return result;
}

Position& Position::operator*=(const int multiplier)
{
    m_x *= multiplier;
    m_y *= multiplier;

    return *this;
}

Position Position::operator*(const int multiplier) const
{
    Position result(*this);

    result *= multiplier;

    return result;
}

bool Position::operator==(const Position& rightOperand) const
{
    return m_x == rightOperand.m_x && m_y == rightOperand.m_y;
}

bool Position::operator!=(const Position& rightOperand) const
{
    return !(*this == rightOperand);
}

// Used to output Position objects
std::ostream& operator<<(std::ostream& out, const Position& pos)
{
    out << "(" << pos.m_x << ", " << pos.m_y << ")";

    return out;
} /*
============================================================ */

// Create hash value from Position object
std::size_t PositionHasher::operator()(const Position& position) const
{
    return std::hash<int>()((position.m_x << X_SHIFT) + position.m_y);
}

// Return opposite color
Color operator~(const Color color)
{
    return (color == WHITE ? BLACK : WHITE);
}

// Converts Color to string
std::string toString(const Color& color)
{
    switch(color)
    {
        case WHITE:
            return "WHITE";
        case BLACK:
            return "BLACK";
        case NUM_COLORS:
            return "NUM_COLORS";
        default:
            return "NEUTRAL";
    }
}

// Convert PieceType to string
std::string toString(const PieceType& pieceType)
{
    switch(pieceType)
    {
        case PAWN:
            return "PAWN";
        case KNIGHT:
            return "KNIGHT";
        case BISHOP:
            return "BISHOP";
        case ROOK:
            return "ROOK";
        case QUEEN:
            return "QUEEN";
        case KING:
            return "KING";
        case NUM_PIECE_TYPES:
            return "NUM_PIECE_TYPES";
        default:
            return "NONE";
    }
} 

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  vector<string> stringSplit(const string& originalString, const char delim)
///
/// \brief:  Split string into substrings using delimeter
///
/// \param [in]:  originalString : string to be split
/// \param [in]:  delim          : delimeter used to split strings
///
/// \return:  vector<string> : vector containing substrings
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> stringSplit(const std::string& originalString, char delim)
{
    std::vector<std::string> substrings;
    std::stringstream sStream(originalString);
    std::string substring;

    while(std::getline(sStream, substring, delim))
        substrings.push_back(substring);

    return substrings;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t coordToBit(const int x, const int y)
///
/// \brief:  Convert cartesian coordinates to uint64_t used for bitboards
///
/// \param [in]:  x : x-coordinate
/// \param [in]:  y : y-coordinate
///
/// \return:  uint64_t : converted coordinates
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t coordToBit(const int x, const int y)
{
    return uint64_t(1) << (y * NUM_FILES + x);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t coordToBit(const Position& pos)
///
/// \brief:  Convert cartesian coordinates to uint64_t used for bitboards
///
/// \param [in]:  pos : contains x and y coordinates
///
/// \return:  uint64_t : converted coordinates
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t coordToBit(const Position& pos)
{
    return coordToBit(pos.m_x, pos.m_y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string toRankAndFile(const int x, const int y)
///
/// \brief:  Convert cartesian coordinates to string containing rank and file
///
/// \param [in]:  x : x-coordinate
/// \param [in]:  y : y-coordinate
///
/// \return:  std::string : contains rank and file (<file><rank>)
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::string toFileAndRank(const int x, const int y)
{
    std::string result = "";

    // Convert x-coordinate to file
    result += (char)(x + 97);

    // Convert y-coordinate to rank
    result += std::to_string(NUM_RANKS - y);

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string toRankAndFile(const Position& pos)
///
/// \brief:  Convert Position object to string containing rank and file
///
/// \param [in]:  pos : contains x and y coordinates
///
/// \return:  std::string : contains rank and file (<file><rank>)
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::string toFileAndRank(const Position& pos)
{
    return toFileAndRank(pos.m_x, pos.m_y);
}