#include "chess.h"

#include "util/utility.h"

#define UP Position::UP
#define DOWN Position::DOWN
#define LEFT Position::LEFT
#define RIGHT Position::RIGHT

using namespace util;
using namespace util::bitboard;

MoveHistoryNode::MoveHistoryNode(const Position& source, const Position& destination, const Color color, const PieceType pieceType) :
	source(source),
	destination(destination),
	player(color),
	pieceType(pieceType) {}

MoveHistoryNode& MoveHistoryNode::operator=(const MoveHistoryNode& rightOperand)
{
	source = rightOperand.source;
	destination = rightOperand.destination;
	player = rightOperand.player;
	pieceType = rightOperand.pieceType;

	return *this;
}

bool MoveHistoryNode::operator==(const MoveHistoryNode& rightOperand) const
{
	return source == rightOperand.source && destination == rightOperand.destination;
}

bool MoveHistoryNode::operator!=(const MoveHistoryNode& rightOperand) const
{
	return !(*this == rightOperand);
}

std::ostream& operator<<(std::ostream& out, const MoveHistoryNode& move)
{
	out << "{" << move.source << ", " << move.destination << ", " << toString(move.player) << ", " << toString(move.pieceType) << "}";
	return out;
}

ChessState::ChessState()
{
	initialize();
}

ChessState::ChessState(const std::string& fenString)
{
	setState(fenString);
}

ChessState::ChessState(const ChessState& source) :
	_board(source._board),
	_moveHistory(source._moveHistory),
	_winner(source._winner),
	_nextTurn(source._nextTurn),
	_halfTurnCount(source._halfTurnCount),
	_fullTurnCount(source._fullTurnCount),
	_wKingSideCastle(source._wKingSideCastle),
	_wQueenSideCastle(source._wQueenSideCastle),
	_bKingSideCastle(source._bKingSideCastle),
	_bQueenSideCastle(source._bQueenSideCastle) {}

ChessState::~ChessState()
{
	clear();
}

Color ChessState::getNextTurn() const
{
	return _nextTurn;
}

std::optional<Color> ChessState::getWinner() const
{
	return _winner;
}

std::string ChessState::getFenString() const
{
	std::string fenString = "";

	int numZeros = 0;
	for (int y = 0; y < RANK_COUNT; y++)
	{
		for (int x = 0; x < FILE_COUNT; x++)
		{
			bool occupied = true;
			char output;

			if (_board.posIsOccupied(x, y, Color::WHITE))
			{
				output = PIECE_SYMBOLS[Color::WHITE][_board.getPieceType(x, y)];
			}
			else if (_board.posIsOccupied(x, y, Color::BLACK))
			{
				output = PIECE_SYMBOLS[Color::BLACK][_board.getPieceType(x, y)];
			}
			else
			{
				occupied = false;
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

	fenString += (_nextTurn == Color::WHITE ? 'w' : 'b');
	fenString += ' ';

	std::string castlingStr = "";
	if (_wKingSideCastle)
		castlingStr += 'K';
	if (_wQueenSideCastle)
		castlingStr += 'Q';
	if (_bKingSideCastle)
		castlingStr += 'k';
	if (_bQueenSideCastle)
		castlingStr += 'q';
	fenString += (castlingStr.empty() ? "-" : castlingStr) + " ";

	std::string enPassantStr = "";
	if (!_moveHistory.empty())
	{
		if (_moveHistory.back().pieceType == PieceType::PAWN)
		{
			int deltaY = _moveHistory.back().source.y - _moveHistory.back().destination.y;

			if (deltaY * deltaY > 1)
			{
				// Add file to en passant string
				enPassantStr += (char)(_moveHistory.back().destination.x + 97);

				// Add rank to en passant string
				enPassantStr += std::to_string(RANK_COUNT - _moveHistory.back().destination.y - (int)(deltaY / 2));
			}
		}
	}
	fenString += (enPassantStr.empty() ? "-" : enPassantStr) + " ";

	fenString += std::to_string(_halfTurnCount) + " ";
	fenString += std::to_string(_fullTurnCount);

	return fenString;
}

const BitboardSet& ChessState::getBoard() const
{
	return _board;
}

const std::deque<MoveHistoryNode>& ChessState::getMoveHistory() const
{
	return _moveHistory;
}

int ChessState::getHalfTurnCount() const
{
	return _halfTurnCount;
}

int ChessState::getFullTurnCount() const
{
	return _fullTurnCount;
}

bool ChessState::canKingSideCastle(const Color player) const
{
	return player == Color::WHITE ? _wKingSideCastle : _bKingSideCastle;
}

bool ChessState::canQueenSideCastle(const Color player) const
{
	return player == Color::WHITE ? _wQueenSideCastle : _bQueenSideCastle;
}

void ChessState::update(const Color player, const move::Move& move, const PieceType promotion)
{
	update(player, move.source, move.destination, promotion);
}

void ChessState::update(const Color player, const Position& source, const Position& destination, const PieceType promotion)
{
	PieceType pieceType = _board.getPieceType(source, player);

	if (pieceType == PieceType::NONE)
	{
		std::string errorMessage = "Piece not found at position (" + std::to_string(source.x) +
			", " + std::to_string(source.y) + ")";
		throw std::exception(errorMessage.c_str());
	}

	if (pieceType == PieceType::PAWN)
	{
		// Pawn advancement resets half turns to 0
		//   (half turns are incremented at end of function)
		_halfTurnCount = -1;

		// If an en passant occurs
		if ((destination.x - source.x) != 0 && !_board.posIsOccupied(destination))
		{
			const Position backward = player == Color::WHITE ? DOWN : UP;
			_board.clearPos(destination + backward, ~player);
		}

		int endOfBoard = (player == WHITE ? 0 : RANK_COUNT - 1);
		// promotion
		if (destination.y == endOfBoard)
		{
			_board.clearPos(source, player, PieceType::PAWN);
			_board.addPiece(source, player, promotion);
			pieceType = promotion;
		}
	}

	else if (pieceType == PieceType::KING)
	{
		int deltaX = destination.x - source.x;

		// If castling occurs
		if (deltaX * deltaX > 1)
		{
			Position oldRookPosition, newRookPosition;

			// If kingside castling occurs
			if (deltaX > 1)
			{
				oldRookPosition.x = FILE_COUNT - 1;
				newRookPosition = destination + LEFT;
			}
			// If queenside castling occurs
			else
			{
				oldRookPosition.x = 0;
				newRookPosition = destination + RIGHT;
			}
			oldRookPosition.y = source.y;

			_board.clearPos(oldRookPosition, player, PieceType::ROOK);
			_board.addPiece(newRookPosition, player, PieceType::ROOK);
		}

		if (player == Color::WHITE)
		{
			_wKingSideCastle = false;
			_wQueenSideCastle = false;
		}
		else // player == Color::BLACK
		{
			_bKingSideCastle = false;
			_bQueenSideCastle = false;
		}
	}

	else if (pieceType == PieceType::ROOK)
	{
		if (source == Position(0, 0))
		{
			_bQueenSideCastle = false;
		}
		else if (source == Position(FILE_COUNT - 1, 0))
		{
			_bKingSideCastle = false;
		}
		else if (source == Position(0, RANK_COUNT - 1))
		{
			_wQueenSideCastle = false;
		}
		else if (source == Position(FILE_COUNT - 1, RANK_COUNT - 1))
		{
			_wKingSideCastle = false;
		}
	}

	// capture
	if (_board.posIsOccupied(destination, ~player))
	{
		if (_board.posIsOccupied(destination, ~player, PieceType::ROOK))
		{
			if (destination.x == 0)
			{
				if (player == Color::WHITE)
				{
					_bQueenSideCastle = false;
				}
				else // player == Color::BLACK
				{
					_wQueenSideCastle = false;
				}
			}
			else if (destination.x == FILE_COUNT - 1)
			{
				if (player == Color::WHITE)
				{
					_bKingSideCastle = false;
				}
				else // player == Color::BLACK
				{
					_wKingSideCastle = false;
				}
			}
		}
		_board.clearPos(destination, ~player);

		// Half turns are reset when a capture occurs
		//   (half turns are incremented at end of function)
		_halfTurnCount = -1;
	}

	_halfTurnCount += 1;

	_moveHistory.emplace_back(source, destination, player, pieceType);
	if (_moveHistory.size() > 8)
	{
		_moveHistory.pop_front();
	}

	// check for tie
	if (_moveHistory.size() >= 8 && _halfTurnCount >= 8)
	{
		bool tie = true;
		int prevMoveIndex = 0,
			moveHistorySize = (int)_moveHistory.size();

		while (tie && prevMoveIndex < (moveHistorySize / 2))
		{
			tie = _moveHistory[prevMoveIndex] == _moveHistory[prevMoveIndex + 4];
			prevMoveIndex += 1;
		}

		if (tie)
		{
			_nextTurn = NEUTRAL;
		}
	}

	_board.clearPos(source, player, pieceType);
	_board.addPiece(destination, player, pieceType);

	if (_nextTurn == Color::BLACK)
	{
		_fullTurnCount += 1;
	}

	if (_nextTurn != NEUTRAL)
	{
		_nextTurn = ~player;
	}
}

void ChessState::clear()
{
	_board.clear();

	_moveHistory.clear();

	_wKingSideCastle = false;
	_wQueenSideCastle = false;
	_bKingSideCastle = false;
	_bQueenSideCastle = false;

	_winner = std::nullopt;
	_nextTurn = Color::WHITE;

	_halfTurnCount = 0;
	_fullTurnCount = 0;
}

void ChessState::reset()
{
	clear();
	initialize();
}

void ChessState::initialize()
{
	_board.populateBoard();

	_winner = std::nullopt;
	_nextTurn = Color::WHITE; // White starts by default

	_halfTurnCount = 0;
	_fullTurnCount = 1;

	_wKingSideCastle = true;
	_wQueenSideCastle = true;
	_bKingSideCastle = true;
	_bQueenSideCastle = true;
}

void ChessState::setState(const std::string& fenString)
{
	_board.clear();
	_moveHistory.clear();
	_winner = std::nullopt;

	_wKingSideCastle = false;
	_wQueenSideCastle = false;
	_bKingSideCastle = false;
	_bQueenSideCastle = false;

	std::vector<std::string> substrings = stringSplit(fenString, ' ');
	std::vector<std::string> boardStrings = stringSplit(substrings[0], '/');

	for (int y = 0; y < RANK_COUNT; y++)
	{
		int stringIndex = 0;
		for (int x = 0; x < FILE_COUNT; x++)
		{
			if (boardStrings[y][stringIndex] >= 'A' && boardStrings[y][stringIndex] <= 'Z')
			{
				Color color = Color::WHITE;
				PieceType pieceType = PieceType::NONE;

				switch (boardStrings[y][stringIndex])
				{
					case PIECE_SYMBOLS[Color::WHITE][PieceType::PAWN]:
						pieceType = PieceType::PAWN;
						break;
					case PIECE_SYMBOLS[Color::WHITE][PieceType::KNIGHT]:
						pieceType = PieceType::KNIGHT;
						break;
					case PIECE_SYMBOLS[Color::WHITE][PieceType::BISHOP]:
						pieceType = PieceType::BISHOP;
						break;
					case PIECE_SYMBOLS[Color::WHITE][PieceType::ROOK]:
						pieceType = PieceType::ROOK;
						break;
					case PIECE_SYMBOLS[Color::WHITE][PieceType::QUEEN]:
						pieceType = PieceType::QUEEN;
						break;
					case PIECE_SYMBOLS[Color::WHITE][PieceType::KING]:
						pieceType = PieceType::KING;
						break;
					default:
						break;
				}

				_board.addPiece(x, y, color, pieceType);
			}
			else if (boardStrings[y][stringIndex] >= 'a' && boardStrings[y][stringIndex] <= 'z')
			{
				Color color = Color::BLACK;
				PieceType pieceType = PieceType::NONE;

				switch (boardStrings[y][stringIndex])
				{
					case PIECE_SYMBOLS[Color::BLACK][PieceType::PAWN]:
						pieceType = PieceType::PAWN;
						break;
					case PIECE_SYMBOLS[Color::BLACK][PieceType::KNIGHT]:
						pieceType = PieceType::KNIGHT;
						break;
					case PIECE_SYMBOLS[Color::BLACK][PieceType::BISHOP]:
						pieceType = PieceType::BISHOP;
						break;
					case PIECE_SYMBOLS[Color::BLACK][PieceType::ROOK]:
						pieceType = PieceType::ROOK;
						break;
					case PIECE_SYMBOLS[Color::BLACK][PieceType::QUEEN]:
						pieceType = PieceType::QUEEN;
						break;
					case PIECE_SYMBOLS[Color::BLACK][PieceType::KING]:
						pieceType = PieceType::KING;
						break;
					default:
						break;
				}

				_board.addPiece(x, y, color, pieceType);
			}
			else if (boardStrings[y][stringIndex] >= '0' && boardStrings[y][stringIndex] <= '9')
			{
				int numEmpties = (int)boardStrings[y][stringIndex] - 48;

				while (boardStrings[y][stringIndex + 1] >= '0' && boardStrings[y][stringIndex + 1] <= '9')
				{
					numEmpties *= 10;
					numEmpties += (int)boardStrings[y][stringIndex] - 48;
					stringIndex += 1;
				}
				x += numEmpties - 1;
			}
			stringIndex += 1;
		}
	}

	_nextTurn = (substrings[1] == "w" ? Color::WHITE : Color::BLACK);

	if (substrings[2] != "-")
	{
		for (int i = 0; i < (int)substrings[2].size(); i++)
		{
			switch (substrings[2][i])
			{
				case PIECE_SYMBOLS[Color::WHITE][PieceType::KING]:
					_wKingSideCastle = true;
					break;
				case PIECE_SYMBOLS[Color::WHITE][PieceType::QUEEN]:
					_wQueenSideCastle = true;
					break;
				case PIECE_SYMBOLS[Color::BLACK][PieceType::KING]:
					_bKingSideCastle = true;
					break;
				case PIECE_SYMBOLS[Color::BLACK][PieceType::QUEEN]:
					_bQueenSideCastle = true;
					break;
				default:
					break;
			}
		}
	}

	if (substrings[3] != "-")
	{
		int x = (int)substrings[3][0] - 97,
			y = RANK_COUNT - ((int)substrings[3][1] - 48);

		if (_board.posIsOccupied(x, y - 1, Color::WHITE, PieceType::PAWN))
		{
			Position source(x, y + 1),
				currPos(x, y - 1);

			_moveHistory.push_back(MoveHistoryNode(source, currPos, Color::WHITE, PieceType::PAWN));
		}
		else if (_board.posIsOccupied(x, y + 1, Color::BLACK, PieceType::PAWN))
		{
			// Add pawn move to move history
			Position source(x, y - 1),
				currPos(x, y + 1);

			_moveHistory.push_back(MoveHistoryNode(source, currPos, Color::BLACK, PieceType::PAWN));
		}
	}

	_halfTurnCount = std::stoi(substrings[4]);
	_fullTurnCount = std::stoi(substrings[5]);
}

void ChessState::print() const
{
	_board.print();
}

#ifdef _DEBUG
void ChessState::printDebug() const
{
	// Output last 8 moves
	std::cout << "MOVE HISTORY: ";
	for (const MoveHistoryNode& move : _moveHistory)
		std::cout << move << " ";
	std::cout << std::endl;

	std::cout << "NEXT TURN: " << toString(_nextTurn) << std::endl;
	std::cout << "NUMBER OF HALF TURNS: " << _halfTurnCount << std::endl;
	std::cout << "NUMBER OF FULL TURNS: " << _fullTurnCount << std::endl;
	std::cout << "WINNER: " << (_winner ? toString(_winner.value()) : "") << std::endl;

	std::cout << "CASTLING: ";
	if (_wKingSideCastle) std::cout << PIECE_SYMBOLS[Color::WHITE][PieceType::KING];
	if (_wQueenSideCastle) std::cout << PIECE_SYMBOLS[Color::WHITE][PieceType::QUEEN];
	if (_bKingSideCastle) std::cout << PIECE_SYMBOLS[Color::BLACK][PieceType::KING];
	if (_bQueenSideCastle) std::cout << PIECE_SYMBOLS[Color::BLACK][PieceType::QUEEN];
	std::cout << std::endl;

	std::cout << "BOARD:" << std::endl;
	_board.print();
}
#endif