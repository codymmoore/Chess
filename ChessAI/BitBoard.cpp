#include "BitBoard.h"

using std::cout;
using std::endl;
using std::string;

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  BitBoard::BitBoard()
///
/// \brief:  Create empty board
///
/////////////////////////////////////////////////////////////////////////////////////////////
BitBoard::BitBoard()
{
    for(int color = WHITE; color < BLACK; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            m_board[color][pieceType] = 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  BitBoard::BitBoard(const BitBoard& source)
///
/// \brief:  Create a copy of passed BitBoard
///
/// \param [in]:  source : BitBoard to be copied
///
/////////////////////////////////////////////////////////////////////////////////////////////
BitBoard::BitBoard(const BitBoard& source)
{
    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            m_board[color][pieceType] = source.m_board[color][pieceType];
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  BitBoard& BitBoard::operator=(const BitBoard& rightOperand)
///
/// \brief:  Set calling object equal to right operand
///
/// \param [in]:  rightOperand : BitBoard to be copied
///
/// \return:  BitBoard& : reference to calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
BitBoard& BitBoard::operator=(const BitBoard& rightOperand)
{
    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            m_board[color][pieceType] = rightOperand.m_board[color][pieceType];
        }
    }

    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::operator==(const BitBoard& rightOperand) const
///
/// \brief:  Determine whether two BitBoard objects are equivalent
///
/// \param [in]:  rightOperand : BitBoard object to be compared to
///
/// \return:  bool : true if calling object and right operand are equivalent; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::operator==(const BitBoard& rightOperand) const
{
    bool result = true;
    int color = WHITE;

    while(color < NUM_COLORS && result)
    {
        int pieceType = PAWN;

        while(pieceType < NUM_PIECE_TYPES && result)
        {
            result = m_board[color][pieceType] == rightOperand.m_board[color][pieceType];
            pieceType++;
        }
        color++;
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::operator!=(const BitBoard& rightOperand) const
///
/// \brief:  Determine whether two BitBoard objects are not equivalent
///
/// \param [in]:  rightOperand : BitBoard object to be compared to
///
/// \return:  bool : true if calling object and right operand are not equivalent; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::operator!=(const BitBoard& rightOperand) const
{
    return !(*this == rightOperand);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupied(const int x, const int y) const
///
/// \brief:  Determine whether a position is occupied
///
/// \param [in]:  x : x-coordinate of position
/// \param [in]:  y : y-coordinate of position
///
/// \return:  bool : true if position is occupied; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupied(const int x, const int y) const
{
    uint64_t binaryPosition = coordToBit(x, y);
    return (getOccupiedSpaces() & binaryPosition) == binaryPosition;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupied(const Position& pos) const
///
/// \brief:  Determine whether a position is occupied
///
/// \param [in]:  pos : object containing x and y coordinate
///
/// \return:  bool : true if position is occupied; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupied(const Position& pos) const
{
    return posIsOccupied(pos.m_x, pos.m_y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const int x, const int y, const Color color) const
///
/// \brief:  Determine whether a position is occupied by a color
///
/// \param [in]:  x : x-coordinate of position
/// \param [in]:  y : y-coordinate of position
/// \param [in]:  color
///
/// \return:  bool : true if position is occupied by color; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByColor(const int x, const int y, const Color color) const
{
    uint64_t binaryPosition = coordToBit(x, y);
    return (getColorBoard(color) & binaryPosition) == binaryPosition;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const Position& pos, const Color color) const
///
/// \brief:  Determine whether a position is occupied by a color
///
/// \param [in]:  pos : object containing x and y coordinate
/// \param [in]:  color
///
/// \return:  bool : true if position is occupied by color; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByColor(const Position& pos, const Color color) const
{
    return posIsOccupiedByColor(pos.m_x, pos.m_y, color);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const int x, const int y, const PieceType pieceType) const
///
/// \brief:  Determine whether a position is occupied by a piece type
///
/// \param [in]:  x : x-coordinate of position
/// \param [in]:  y : y-coordinate of position
/// \param [in]:  pieceType
///
/// \return:  bool : true if position is occupied by piece type; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByPiece(const int x, const int y, const PieceType pieceType) const
{
    uint64_t binaryPosition = coordToBit(x, y);
    return (getPieceBoard(pieceType) & binaryPosition) == binaryPosition;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const Position& pos, const PieceType pieceType) const
///
/// \brief:  Determine whether a position is occupied by a piece type
///
/// \param [in]:  pos : object containing x and y coordinate
/// \param [in]:  pieceType
///
/// \return:  bool : true if position is occupied by piece type; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByPiece(const Position& pos, const PieceType pieceType) const
{
    return posIsOccupiedByPiece(pos.m_x, pos.m_y, pieceType);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const int x, const int y, const Color color, const PieceType pieceType) const
///
/// \brief:  Determine whether a position is occupied by a piece type of a particular color
///
/// \param [in]:  x : x-coordinate of position
/// \param [in]:  y : y-coordinate of position
/// \param [in]:  color
/// \param [in]:  pieceType
///
/// \return:  bool : true if position is occupied by piece type of specified color; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByColorPiece(const int x, const int y, const Color color, const PieceType pieceType) const
{
    uint64_t binaryPosition = coordToBit(x, y);
    return (m_board[color][pieceType] & binaryPosition) == binaryPosition;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool BitBoard::posIsOccupiedByColor(const int x, const int y, const Color color, const PieceType pieceType) const
///
/// \brief:  Determine whether a position is occupied by a piece type of a particular color
///
/// \param [in]:  pos : object containing x and y coordinate
/// \param [in]:  color
/// \param [in]:  pieceType
///
/// \return:  bool : true if position is occupied by piece type of specified color; false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool BitBoard::posIsOccupiedByColorPiece(const Position& pos, const Color color, const PieceType pieceType) const
{
    return posIsOccupiedByColorPiece(pos.m_x, pos.m_y, color, pieceType);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t BitBoard::getOccupiedSpaces() const
///
/// \brief:  Get uint64_t representation of occupied positions
///
/// \return:  uint64_t : 64-bit integer representation of occupied spaces
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t BitBoard::getOccupiedSpaces() const
{
    uint64_t result = 0;

    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            result |= m_board[color][pieceType];
        }
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t BitBoard::getColorBoard(const Color color) const
///
/// \brief:  Get uint64_t representation of positions occupied by color
///
/// \param [in]:  color
///
/// \return:  uint64_t : 64-bit integer representation of spaces occupied by color
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t BitBoard::getColorBoard(const Color color) const
{
    uint64_t result = 0;

    for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
    {
        result |= m_board[color][pieceType];
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t BitBoard::getPieceBoard(const PieceType pieceType) const
///
/// \brief:  Get uint64_t representation of positions occupied by a piece type
///
/// \param [in]:  pieceType
///
/// \return:  uint64_t : 64-bit integer representation of spaces occupied by a piece type
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t BitBoard::getPieceBoard(const PieceType pieceType) const
{
    uint64_t result = 0;

    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        result |= m_board[color][pieceType];
    }

    return result;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  uint64_t BitBoard::getColorPieceBoard(const Color color, const PieceType pieceType) const
///
/// \brief:  Get uint64_t representation of positions occupied by a piece type of specified color
///
/// \param [in]:  color
/// \param [in]:  pieceType
///
/// \return:  uint64_t : 64-bit integer representation of spaces occupied by a piece type of specified color
///
/////////////////////////////////////////////////////////////////////////////////////////////
uint64_t BitBoard::getColorPieceBoard(const Color color, const PieceType pieceType) const
{
    return m_board[color][pieceType];
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void BitBoard::print() const
///
/// \brief:  Print ASCII representation of board to stdout
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::print() const
{
    for(int y = 0; y < NUM_RANKS; y++)
    {
        for(int x = 0; x < NUM_FILES; x++)
        {
            bool occupied = false;
            char outputChar = '-';

            int color = WHITE;
            while(color < NUM_COLORS && !occupied)
            {
                int pieceType = PAWN;
                while(pieceType < NUM_PIECE_TYPES && !occupied)
                {
                    if(posIsOccupiedByColorPiece(x, y, (Color)color, (PieceType)pieceType))
                    {
                        outputChar = PIECE_SYMBOLS[color][pieceType];
                        occupied = true;
                    }
                    pieceType += 1;
                }
                color += 1;
            }
            cout << outputChar;
        }
        cout << endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void BitBoard::populateBoard()
///
/// \brief:  Set board to default starting chess board
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::populateBoard()
{
    /* ----- WHITE PIECES ----- */
    // Pawns
    m_board[WHITE][PAWN]   = 0x00ff000000000000;
    // Knights
    m_board[WHITE][KNIGHT] = 0x4200000000000000;
    // Bishops
    m_board[WHITE][BISHOP] = 0x2400000000000000;
    // Rooks
    m_board[WHITE][ROOK]   = 0x8100000000000000;
    // Queen
    m_board[WHITE][QUEEN]  = 0x0800000000000000;
    // King
    m_board[WHITE][KING]   = 0x1000000000000000;

    /* ----- BLACK PIECES ----- */
    // Pawns
    m_board[BLACK][PAWN] = 0x000000000000ff00;
    // Knights
    m_board[BLACK][KNIGHT] = 0x0000000000000042;
    // Bishops
    m_board[BLACK][BISHOP] = 0x0000000000000024;
    // Rooks
    m_board[BLACK][ROOK]   = 0x0000000000000081;
    // Queen
    m_board[BLACK][QUEEN]  = 0x000000000000008;
    // King
    m_board[BLACK][KING]   = 0x000000000000010;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const int x, const int y)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  x : x-coordinate of position to be cleared
/// \param [in]:  y : y-coordinate of position to be cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const int x, const int y)
{
    uint64_t binaryPosition = coordToBit(x, y);

    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            m_board[color][pieceType] &= ~binaryPosition;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const Position& pos)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  pos : object containing x and y coordinate of position to be cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const Position& pos)
{
    clearPos(pos.m_x, pos.m_y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const int x, const int y, const Color color)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  x     : x-coordinate of position to be cleared
/// \param [in]:  y     : y-coordinate of position to be cleared
/// \param [in]:  color : color of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const int x, const int y, const Color color)
{
    uint64_t binaryPosition = coordToBit(x, y);

    for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
    {
        m_board[color][pieceType] &= ~binaryPosition;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const Position& pos, const Color color)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  pos   : object containing x and y coordinate of position to be cleared
/// \param [in]:  color : color of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const Position& pos, const Color color)
{
    clearPos(pos.m_x, pos.m_y, color);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const int x, const int y, const PieceType pieceType)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  x         : x-coordinate of position to be cleared
/// \param [in]:  y         : y-coordinate of position to be cleared
/// \param [in]:  pieceType : type of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const int x, const int y, const PieceType pieceType)
{
    uint64_t binaryPosition = coordToBit(x, y);

    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        m_board[color][pieceType] &= ~binaryPosition;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const Position& pos, const PieceType pieceType)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  pos       : object containing x and y coordinate of position to be cleared
/// \param [in]:  pieceType : type of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const Position& pos, const PieceType pieceType)
{
    clearPos(pos.m_x, pos.m_y, pieceType);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const int x, const int y, const Color color, const PieceType pieceType)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  x         : x-coordinate of position to be cleared
/// \param [in]:  y         : y-coordinate of position to be cleared
/// \param [in]:  color     : color of piece being cleared
/// \param [in]:  pieceType : type of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const int x, const int y, const Color color, const PieceType pieceType)
{
    m_board[color][pieceType] &= ~coordToBit(x, y);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPos(const Position& pos, const Color color, const PieceType pieceType)
///
/// \brief:  Clear a position on the board
///
/// \param [in]:  pos       : object containing x and y coordinate of position to be cleared
/// \param [in]:  color     : color of piece being cleared
/// \param [in]:  pieceType : type of piece being cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPos(const Position& pos, const Color color, const PieceType pieceType)
{
    clearPos(pos.m_x, pos.m_y, color, pieceType);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearColor(const Color color)
///
/// \brief:  Clear all pieces of a particular color from board
///
/// \param [in]:  color : color to be cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearColor(const Color color)
{
    for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
    {
        m_board[color][pieceType] &= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clearPieceType(const PieceType pieceType)
///
/// \brief:  Clear all pieces of a particular type from board
///
/// \param [in]:  pieceType : piece type to be cleared
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clearPieceType(const PieceType pieceType)
{
    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        m_board[color][pieceType] &= 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void clear()
///
/// \brief:  Clear all positions on board
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::clear()
{
    for(int color = WHITE; color < NUM_COLORS; color++)
    {
        for(int pieceType = PAWN; pieceType < NUM_PIECE_TYPES; pieceType++)
        {
            m_board[color][pieceType] &= 0;
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void addPiece(const int x, const int y, const Color color, const PieceType pieceType)
///
/// \brief:  Add a piece to the board
///
/// \param [in]:  x         : x-coordinate of position to add piece
/// \param [in]:  y         : y-coordinate of position to add piece
/// \param [in]:  color     : color of piece to be added
/// \param [in]:  pieceType : type of piece to be added
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::addPiece(const int x, const int y, const Color color, const PieceType pieceType)
{
    // As of right now, allows to place a piece in a position occupied by a different piece type
    m_board[color][pieceType] |= coordToBit(x, y); 
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void addPiece(const Position& pos, const Color color, const PieceType pieceType)
///
/// \brief:  Add a piece to the board
///
/// \param [in]:  pos       : object containing x and y coordinate of position to add piece
/// \param [in]:  color     : color of piece to be added
/// \param [in]:  pieceType : type of piece to be added
///
/// \warning: This function allows you to add a piece to an occupied position
///
/////////////////////////////////////////////////////////////////////////////////////////////
void BitBoard::addPiece(const Position& pos, const Color color, const PieceType pieceType)
{
    addPiece(pos.m_x, pos.m_y, color, pieceType);
}