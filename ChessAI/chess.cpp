#include "chess.h"

#include "util/utility.h"

using namespace util;
using namespace util::bitboard;

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  PieceNode::PieceNode()
///
/// \brief:  Create default PieceNode object
///
/////////////////////////////////////////////////////////////////////////////////////////////
PieceNode::PieceNode() : m_position(0, 0), m_pieceType(NONE) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  PieceNode::PieceNode(const int xCoord, const int yCoord, const PieceType pieceType)
///
/// \brief:  Create PieceNode object
///
/// \param [in]:  xCoord : x-coordinate of piece contained by calling object
/// \param [in]:  yCoord : y-coordinate of piece contained by calling object
/// \param [in]:  pieceType : type of piece contained by calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
PieceNode::PieceNode(const int xCoord, const int yCoord, const PieceType pieceType) :
	m_position(xCoord, yCoord),
	m_pieceType(pieceType) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  PieceNode::PieceNode(const Position& position, const PieceType pieceType)
///
/// \brief:  Create PieceNode object
///
/// \param [in]:  position : current position of piece contained by calling object
/// \param [in]:  pieceType : type of piece contained by calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
PieceNode::PieceNode(const Position& position, const PieceType pieceType) :
	m_position(position),
	m_pieceType(pieceType) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  PieceNode& PieceNode::operator=(const PieceNode& rightOperand)
///
/// \brief:  Set calling object equal to right operand
///
/// \param [in]:  rightOperand : object that calling object will be set equal to
///
/// \return:  PieceNode& : reference to calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
PieceNode& PieceNode::operator=(const PieceNode& rightOperand)
{
	m_position = rightOperand.m_position;
	m_pieceType = rightOperand.m_pieceType;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool PieceNode::operator==(const PieceNode& rightOperand) const
///
/// \brief:  Determine whether two PieceNode objects are equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool PieceNode::operator==(const PieceNode& rightOperand) const
{
	return m_position == rightOperand.m_position && m_pieceType == rightOperand.m_pieceType;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool PieceNode::operator!=(const PieceNode& rightOperand) const
///
/// \brief:  Determine whether two PieceNode objects are not equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are not equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool PieceNode::operator!=(const PieceNode& rightOperand) const
{
	return !(*this == rightOperand);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::ostream& operator<<(std::ostream& out, const PieceNode& piece)
///
/// \brief:  Output PieceNode to ostream
///
/// \param [in, out]:  out : ostream that PieceNode object will be output to
/// \param [in]:  piece : PieceNode object to be output
///
/// \return:  ostream& : reference to updated ostream variable
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& out, const PieceNode& piece)
{
	out << "{" << piece.m_position << ", " << toString(piece.m_pieceType) << "}";

	return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  MoveHistoryNode::MoveHistoryNode(const Color color, const PieceNode& piece, const Position& currPos)
///
/// \brief:  Create MoveHistoryNode object
///
/// \param [in]:  color : color of piece
/// \param [in]:  piece : contains previous position and type of piece
/// \param [in]:  currPos : new position of piece
///
/////////////////////////////////////////////////////////////////////////////////////////////
MoveHistoryNode::MoveHistoryNode(const Color color, const PieceNode& piece, const Position& currPos) :
	m_prevPos(piece.m_position),
	m_currPos(currPos),
	m_color(color),
	m_pieceType(piece.m_pieceType) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  MoveHistoryNode::MoveHistoryNode(const Position& prevPos, const Position& currPos, const Color color, const PieceType pieceType)
///
/// \brief:  Create MoveHistoryNode object
///
/// \param [in]:  prevPos : previous position of piece
/// \param [in]:  currPos : updated positon of piece
/// \param [in]:  color : color of piece
/// \param [in]:  pieceType : type of piece
///
/////////////////////////////////////////////////////////////////////////////////////////////
MoveHistoryNode::MoveHistoryNode(const Position& prevPos, const Position& currPos, const Color color, const PieceType pieceType) :
	m_prevPos(prevPos),
	m_currPos(currPos),
	m_color(color),
	m_pieceType(pieceType) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  MoveHistoryNode& MoveHistoryNode::operator=(const MoveHistoryNode& rightOperand)
///
/// \brief:  Set calling object equal to right operand
///
/// \param [in]:  rightOperand : object that calling object will be set equal to
///
/// \return:  MoveHistoryNode& : reference to calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
MoveHistoryNode& MoveHistoryNode::operator=(const MoveHistoryNode& rightOperand)
{
	m_prevPos = rightOperand.m_prevPos;
	m_currPos = rightOperand.m_currPos;
	m_color = rightOperand.m_color;
	m_pieceType = rightOperand.m_pieceType;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool MoveHistoryNode::operator==(const MoveHistoryNode& rightOperand) const
///
/// \brief:  Determine whether two MoveHistoryNode objects are equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool MoveHistoryNode::operator==(const MoveHistoryNode& rightOperand) const
{
	return m_prevPos == rightOperand.m_prevPos && m_currPos == rightOperand.m_currPos;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool MoveHistoryNode::operator!=(const MoveHistoryNode& rightOperand) const
///
/// \brief:  Determine whether two MoveHistoryNode objects are not equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are not equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool MoveHistoryNode::operator!=(const MoveHistoryNode& rightOperand) const
{
	return !(*this == rightOperand);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::ostream& operator<<(std::ostream& out, const MoveHistoryNode& move)
///
/// \brief:  Output MoveHistoryNode to ostream
///
/// \param [in, out]:  out : ostream that PieceNode object will be output to
/// \param [in]:  move : MoveHistoryNode object to be output
///
/// \return:  ostream& : reference to updated ostream variable
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& out, const MoveHistoryNode& move)
{
	out << "{" << move.m_prevPos << ", " << move.m_currPos << ", " << toString(move.m_color) << ", " << toString(move.m_pieceType) << "}";
	return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  MoveNode::MoveNode(const Position& source, const Position& destination, const PieceType promotion)
///
/// \brief:  Create MoveNode object
///
/// \param [in]:  source : the position the piece moved from
/// \param [in]:  piece : the position the piece is moving to
/// \param [in]:  promotion : the piece type a pawn is being promoted to
///
/////////////////////////////////////////////////////////////////////////////////////////////
MoveNode::MoveNode(const Position& source, const Position& destination, const PieceType promotion) :
	m_source(source),
	m_destination(destination),
	m_promotion(promotion) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  MoveHistoryNode& MoveHistoryNode::operator=(const MoveHistoryNode& rightOperand)
///
/// \brief:  Set calling object equal to right operand
///
/// \param [in]:  rightOperand : object that calling object will be set equal to
///
/// \return:  MoveNode& : reference to calling object
///
/////////////////////////////////////////////////////////////////////////////////////////////
MoveNode& MoveNode::operator=(const MoveNode& rightOperand)
{
	m_source = rightOperand.m_source;
	m_destination = rightOperand.m_destination;
	m_promotion = rightOperand.m_promotion;

	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool MoveNode::operator==(const MoveNode& rightOperand) const
///
/// \brief:  Determine whether two MoveNode objects are equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool MoveNode::operator==(const MoveNode& rightOperand) const
{
	return m_source == rightOperand.m_source
		&& m_destination == rightOperand.m_destination
		&& m_promotion == rightOperand.m_promotion;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool MoveNode::operator!=(const MoveNode& rightOperand) const
///
/// \brief:  Determine whether two MoveNode objects are not equivalent
///
/// \param [in]:  rightOperand : object to compare calling object to
///
/// \return:  bool : true if objects are not equivalent, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool MoveNode::operator!=(const MoveNode& rightOperand) const
{
	return !(*this == rightOperand);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::ostream& operator<<(std::ostream& out, const MoveNode& move)
///
/// \brief:  Output MoveNode to ostream
///
/// \param [in, out]:  out : ostream that PieceNode object will be output to
/// \param [in]:  move : MoveNode object to be output
///
/// \return:  ostream& : reference to updated ostream variable
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& out, const MoveNode& move)
{
	out << "{" << move.m_source << ", " << move.m_destination << ", " << toString(move.m_promotion) << "}";
	return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  ChessState::ChessState()
///
/// \brief:  Create default starting chess state
///
/////////////////////////////////////////////////////////////////////////////////////////////
ChessState::ChessState()
{
	initialize();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  ChessState::ChessState(const std::string& gameState)
///
/// \brief:  Create ChessState object from Forsyth-Edwards Notation (FEN) string
///
/// \param [in]:  gameState: FEN string representation of a game state  
///
/////////////////////////////////////////////////////////////////////////////////////////////
ChessState::ChessState(const std::string& gameState)
{
	setState(gameState);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  ChessState::ChessState(const ChessState& source)
///
/// \brief:  Create ChessState copy
///
/// \param [in]:  source : ChessState to be copied
///
/////////////////////////////////////////////////////////////////////////////////////////////
ChessState::ChessState(const ChessState& source) :
	m_board(source.m_board),

	m_whitePieces(source.m_whitePieces),
	m_blackPieces(source.m_blackPieces),

	m_moveHistory(source.m_moveHistory),

	m_winner(source.m_winner),
	m_nextTurn(source.m_nextTurn),

	m_numHalfTurns(source.m_numHalfTurns),
	m_numFullTurns(source.m_numFullTurns),

	// Castling variables
	m_wKingSideCastle(source.m_wKingSideCastle),
	m_wQueenSideCastle(source.m_wQueenSideCastle),
	m_bKingSideCastle(source.m_bKingSideCastle),
	m_bQueenSideCastle(source.m_bQueenSideCastle) {}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  ChessState::ChessState(const ChessState& source)
///
/// \brief:  Destroy ChessState object
///
/////////////////////////////////////////////////////////////////////////////////////////////
ChessState::~ChessState()
{
	clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  Color ChessState::getNextTurn() const
///
/// \brief:  Get color of next player to move
///
/// \return:  Color : color of next player to move
///
/////////////////////////////////////////////////////////////////////////////////////////////
Color ChessState::getNextTurn() const
{
	return m_nextTurn;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  Color ChessState::getWinner() const
///
/// \brief:  Get color of winner
///
/// \return:  Color : color of winner
///
/////////////////////////////////////////////////////////////////////////////////////////////
Color ChessState::getWinner() const
{
	return m_winner;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  const std::vector<PieceNode>& ChessState::getWhitePieces() const
///
/// \brief:  Get constant reference to white player's pieces
///
/// \return:  std::vector<PieceNode>& : contains white player's pieces
///
/////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<PieceNode>& ChessState::getWhitePieces() const
{
	return m_whitePieces;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  const std::vector<PieceNode>& ChessState::getBlackPieces() const
///
/// \brief:  Get constant reference to white player's pieces
///
/// \return:  std::vector<PieceNode>& : contains black player's pieces
///
/////////////////////////////////////////////////////////////////////////////////////////////
const std::vector<PieceNode>& ChessState::getBlackPieces() const
{
	return m_blackPieces;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string ChessState::getFenString() const
///
/// \brief:  Get FEN representation of current chess state
///
/// \return:  std::string : FEN string representation of chess state
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::string ChessState::getFenString() const
{
	std::string fenString = "";

	/* ----- Convert board ----- */
	int numZeros = 0;

	for (int y = 0; y < RANK_COUNT; y++)
	{
		for (int x = 0; x < FILE_COUNT; x++)
		{
			bool occupied = false;
			char output;
			int color = WHITE;

			while (color < COLOR_COUNT && !occupied)
			{
				int pieceType = PAWN;

				while (pieceType < PIECE_TYPE_COUNT && !occupied)
				{
					if (m_board.posIsOccupied(x, y, (Color)color, (PieceType)pieceType))
					{
						output = PIECE_SYMBOLS[color][pieceType];
						occupied = true;
					}
					pieceType += 1;
				}
				color += 1;
			}

			if (occupied)
			{
				if (numZeros != 0)
				{
					fenString += std::to_string(numZeros);
					numZeros = 0;
				}

				fenString += output;
			}
			else
			{
				numZeros += 1;

				if (x == FILE_COUNT - 1 && numZeros != 0)
				{
					fenString += std::to_string(numZeros);
					numZeros = 0;
				}
			}
		}

		if (y != RANK_COUNT - 1)
		{
			fenString += '/';
		}
	}
	fenString += " ";

	// Add turn to FEN string
	fenString += (m_nextTurn == Color::WHITE ? 'w' : 'b');
	fenString += ' ';

	// Add castling variables to FEN string
	std::string castlingStr = "";

	if (m_wKingSideCastle)
		castlingStr += 'K';
	if (m_wQueenSideCastle)
		castlingStr += 'Q';
	if (m_bKingSideCastle)
		castlingStr += 'k';
	if (m_bQueenSideCastle)
		castlingStr += 'q';

	fenString += (castlingStr.empty() ? "-" : castlingStr) + " ";

	// Add en passant to FEN string
	std::string enPassantStr = "";

	if (!m_moveHistory.empty())
	{
		if (m_moveHistory.back().m_pieceType == PieceType::PAWN)
		{
			int deltaY = m_moveHistory.back().m_prevPos.y - m_moveHistory.back().m_currPos.y;

			if (deltaY * deltaY > 1)
			{
				// Add file to en passant string
				enPassantStr += (char)(m_moveHistory.back().m_currPos.x + 97);

				// Add rank to en passant string
				enPassantStr += std::to_string(RANK_COUNT - m_moveHistory.back().m_currPos.y - (int)(deltaY / 2));
			}
		}
	}

	fenString += (enPassantStr.empty() ? "-" : enPassantStr) + " ";

	// Add half turns to FEN string
	fenString += std::to_string(m_numHalfTurns) + " ";

	// Add full turns to FEN string
	fenString += std::to_string(m_numFullTurns);

	return fenString;
}

/**
 * Retrieve the board.
 *
 * \return The BitBoard instance representing the board state
 */
const BitboardSet& ChessState::getBoard() const
{
	return m_board;
}

const std::deque<MoveHistoryNode>& ChessState::getMoveHistory() const
{
	return m_moveHistory;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void setState(const std::string& gameState)
///
/// \brief:  Set game state according to FEN string
///
/// \param [in]:  gameState : FEN string representing game state
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::setState(const std::string& gameState)
{
	m_board.clear();
	m_moveHistory.clear();
	m_whitePieces.clear();
	m_blackPieces.clear();
	m_winner = NEUTRAL;

	m_wKingSideCastle = false;
	m_wQueenSideCastle = false;
	m_bKingSideCastle = false;
	m_bQueenSideCastle = false;

	// Split FEN string
	std::vector<std::string> substrings = stringSplit(gameState, ' ');

	// Seperate board string into rows
	std::vector<std::string> boardStrings = stringSplit(substrings[0], '/');

	// Fill board
	for (int y = 0; y < RANK_COUNT; y++)
	{
		int stringIndex = 0;
		for (int x = 0; x < FILE_COUNT; x++)
		{
			// If current char is capital (white piece)
			if (boardStrings[y][stringIndex] >= 'A' && boardStrings[y][stringIndex] <= 'Z')
			{
				Color color = Color::WHITE;
				PieceType pieceType = NONE;

				// Convert char to PieceType
				switch (boardStrings[y][stringIndex])
				{
				case PIECE_SYMBOLS[WHITE][PAWN]:
					pieceType = PAWN;
					break;
				case PIECE_SYMBOLS[WHITE][KNIGHT]:
					pieceType = KNIGHT;
					break;
				case PIECE_SYMBOLS[WHITE][BISHOP]:
					pieceType = BISHOP;
					break;
				case PIECE_SYMBOLS[WHITE][ROOK]:
					pieceType = ROOK;
					break;
				case PIECE_SYMBOLS[WHITE][QUEEN]:
					pieceType = QUEEN;
					break;
				case PIECE_SYMBOLS[WHITE][KING]:
					pieceType = KING;
					break;
				default:
					break;
				}

				// Add piece to board
				m_board.addPiece(x, y, color, pieceType);
				// Add piece to white piece vector
				m_whitePieces.push_back(PieceNode(x, y, pieceType));
			}
			// If current char is lowercase (black piece)
			else if (boardStrings[y][stringIndex] >= 'a' && boardStrings[y][stringIndex] <= 'z')
			{
				Color color = Color::BLACK;
				PieceType pieceType = NONE;

				// Convert char to PieceType
				switch (boardStrings[y][stringIndex])
				{
				case PIECE_SYMBOLS[BLACK][PAWN]:
					pieceType = PieceType::PAWN;
					break;
				case PIECE_SYMBOLS[BLACK][KNIGHT]:
					pieceType = PieceType::KNIGHT;
					break;
				case PIECE_SYMBOLS[BLACK][BISHOP]:
					pieceType = PieceType::BISHOP;
					break;
				case PIECE_SYMBOLS[BLACK][ROOK]:
					pieceType = PieceType::ROOK;
					break;
				case PIECE_SYMBOLS[BLACK][QUEEN]:
					pieceType = PieceType::QUEEN;
					break;
				case PIECE_SYMBOLS[BLACK][KING]:
					pieceType = PieceType::KING;
					break;
				default:
					break;
				}

				// Add piece to board
				m_board.addPiece(x, y, color, pieceType);
				// Add piece to black piece vector
				m_blackPieces.push_back(PieceNode(x, y, pieceType));
			}
			// If current char is numerical
			else if (boardStrings[y][stringIndex] >= '0' && boardStrings[y][stringIndex] <= '9')
			{
				int numEmpties = (int)boardStrings[y][stringIndex] - 48;

				while (boardStrings[y][stringIndex + 1] >= '0' && boardStrings[y][stringIndex + 1] <= '9')
				{
					numEmpties *= 10;
					numEmpties += (int)boardStrings[y][stringIndex] - 48;
					stringIndex += 1;
				}

				// Skip blank spaces
				x += numEmpties - 1;
			}
			stringIndex += 1;
		}
	}

	m_nextTurn = (substrings[1] == "w" ? Color::WHITE : Color::BLACK);

	// Set castling variables
	if (substrings[2] != "-")
	{
		for (int i = 0; i < (int)substrings[2].size(); i++)
		{
			switch (substrings[2][i])
			{
			case PIECE_SYMBOLS[WHITE][KING]:
				m_wKingSideCastle = true;
				break;
			case PIECE_SYMBOLS[WHITE][QUEEN]:
				m_wQueenSideCastle = true;
				break;
			case PIECE_SYMBOLS[BLACK][KING]:
				m_bKingSideCastle = true;
				break;
			case PIECE_SYMBOLS[BLACK][QUEEN]:
				m_bQueenSideCastle = true;
				break;
			default:
				break;
			}
		}
	}

	// If En Passant is possible, add to move history
	if (substrings[3] != "-")
	{
		int x = (int)substrings[3][0] - 97, // Convert file to x-coordinate
			y = RANK_COUNT - ((int)substrings[3][1] - 48); // Convert rank to y-coordinate

		// If last pawn to move was colorled by white
		if (m_board.posIsOccupied(x, y - 1, WHITE, PAWN))
		{
			// Add pawn move to move history
			Position prevPos(x, y + 1),
				currPos(x, y - 1);

			m_moveHistory.push_back(MoveHistoryNode(prevPos, currPos, Color::WHITE, PieceType::PAWN));
		}
		// If last pawn to move was colorled by black
		else if (m_board.posIsOccupied(x, y + 1, BLACK, PAWN))
		{
			// Add pawn move to move history
			Position prevPos(x, y - 1),
				currPos(x, y + 1);

			m_moveHistory.push_back(MoveHistoryNode(prevPos, currPos, Color::BLACK, PieceType::PAWN));
		}
	}

	m_numHalfTurns = std::stoi(substrings[4]);
	m_numFullTurns = std::stoi(substrings[5]);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void ChessState::clear()
///
/// \brief:  Clear ChessState object
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::clear()
{
	m_board.clear();

	m_whitePieces.clear();
	m_blackPieces.clear();

	m_moveHistory.clear();

	m_wKingSideCastle = false;
	m_wQueenSideCastle = false;
	m_bKingSideCastle = false;
	m_bQueenSideCastle = false;

	m_winner = Color::NEUTRAL;
	m_nextTurn = Color::WHITE;

	m_numHalfTurns = 0;
	m_numFullTurns = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void ChessState::reset()
///
/// \brief:  Resets the initial game state.
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::reset()
{
	clear();
	initialize();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void ChessState::initialize()
///
/// \brief:  Initializes ChessState instance for a new game.
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::initialize()
{
	m_board.populateBoard();

	m_winner = Color::NEUTRAL;
	m_nextTurn = Color::WHITE; // White starts by default

	m_numHalfTurns = 0;
	m_numFullTurns = 1;

	// Castling Variables
	m_wKingSideCastle = true;
	m_wQueenSideCastle = true;
	m_bKingSideCastle = true;
	m_bQueenSideCastle = true;

	/* ---- Fill Piece Vectors ---- */
	/* -- Rooks -- */
	// Add to piece vectors
	m_blackPieces.push_back(PieceNode(0, 0, PieceType::ROOK));
	m_blackPieces.push_back(PieceNode(FILE_COUNT - 1, 0, PieceType::ROOK));
	m_whitePieces.push_back(PieceNode(0, RANK_COUNT - 1, PieceType::ROOK));
	m_whitePieces.push_back(PieceNode(FILE_COUNT - 1, RANK_COUNT - 1, PieceType::ROOK));

	/* -- Knights -- */
	// Add to piece vectors
	m_blackPieces.push_back(PieceNode(1, 0, PieceType::KNIGHT));
	m_blackPieces.push_back(PieceNode(FILE_COUNT - 2, 0, PieceType::KNIGHT));
	m_whitePieces.push_back(PieceNode(1, RANK_COUNT - 1, PieceType::KNIGHT));
	m_whitePieces.push_back(PieceNode(FILE_COUNT - 2, RANK_COUNT - 1, PieceType::KNIGHT));

	/* -- Bishops -- */
	// Add to piece vectors
	m_blackPieces.push_back(PieceNode(2, 0, PieceType::BISHOP));
	m_blackPieces.push_back(PieceNode(FILE_COUNT - 3, 0, PieceType::BISHOP));
	m_whitePieces.push_back(PieceNode(2, RANK_COUNT - 1, PieceType::BISHOP));
	m_whitePieces.push_back(PieceNode(FILE_COUNT - 3, RANK_COUNT - 1, PieceType::BISHOP));

	/* -- Queens -- */
	// Add to piece vectors
	m_blackPieces.push_back(PieceNode(3, 0, PieceType::QUEEN));
	m_whitePieces.push_back(PieceNode(3, RANK_COUNT - 1, PieceType::QUEEN));

	/* -- Kings -- */
	// Add to piece vectors
	m_blackPieces.push_back(PieceNode(4, 0, PieceType::KING));
	m_whitePieces.push_back(PieceNode(4, RANK_COUNT - 1, PieceType::KING));

	/* -- Pawns -- */
	for (int i = 0; i < FILE_COUNT; i++)
	{
		// Add to piece vectors
		m_blackPieces.push_back(PieceNode(i, 1, PieceType::PAWN));
		m_whitePieces.push_back(PieceNode(i, RANK_COUNT - 2, PieceType::PAWN));
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void ChessState::print() const
///
/// \brief:  Print current board state
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::print() const
{
	m_board.print();
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void ChessState::printDebug() const
///
/// \brief:  Print all information regarding current state
///
/////////////////////////////////////////////////////////////////////////////////////////////
void ChessState::printDebug() const
{
	// Output list of remaining white pieces
	std::cout << "WHITE PIECES: ";
	for (const PieceNode& piece : m_whitePieces)
		std::cout << piece << " ";
	std::cout << std::endl;

	// Output list of remaining black pieces
	std::cout << "BLACK PIECES: ";
	for (const PieceNode& piece : m_blackPieces)
		std::cout << piece << " ";
	std::cout << std::endl;

	// Output last 8 moves
	std::cout << "MOVE HISTORY: ";
	for (const MoveHistoryNode& move : m_moveHistory)
		std::cout << move << " ";
	std::cout << std::endl;

	std::cout << "NEXT TURN: " << toString(m_nextTurn) << std::endl;
	std::cout << "NUMBER OF HALF TURNS: " << m_numHalfTurns << std::endl;
	std::cout << "NUMBER OF FULL TURNS: " << m_numFullTurns << std::endl;
	std::cout << "WINNER: " << toString(m_winner) << std::endl;

	std::cout << "CASTLING: ";
	if (m_wKingSideCastle) std::cout << PIECE_SYMBOLS[WHITE][KING];
	if (m_wQueenSideCastle) std::cout << PIECE_SYMBOLS[WHITE][QUEEN];
	if (m_bKingSideCastle) std::cout << PIECE_SYMBOLS[BLACK][KING];
	if (m_bQueenSideCastle) std::cout << PIECE_SYMBOLS[BLACK][QUEEN];
	std::cout << std::endl;

	std::cout << "BOARD:" << std::endl;
	m_board.print();
}