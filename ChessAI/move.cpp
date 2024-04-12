#include "move.h"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void Move::getValidMovesPawn(const Color player, const PieceNode& piece, 
///         const ChessState& game, std::vector<Position>& moves)
///
/// \brief:  Get valid moves for a pawn
///
/// \param [in]:  player : color of pawn
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  game : state of board
/// \param [out]:  moves : stores valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
void Move::getValidMovesPawn(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves)
{
	Position forward;
	int originalRow;

	// Set forward direction and starting row based on piece's color
	if (player == WHITE)
	{
		forward = UP;
		originalRow = NUM_RANKS - 2;
	}
	else // player == BLACK
	{
		forward = DOWN;
		originalRow = 1;
	}

	Position newPos = piece.m_position + forward;

	// If new position exists on board
	if (newPos.m_y >= 0 && newPos.m_y < NUM_RANKS)
	{
		/* ----- Check if pawn can move forward ----- */
		if (!game.m_board.posIsOccupied(newPos))
		{
			moves.push_back(newPos);

			if (piece.m_position.m_y == originalRow)
			{
				newPos += forward;

				if (!game.m_board.posIsOccupied(newPos))
				{
					moves.push_back(newPos);
				}
			}
		}

		/* ----- Check if capture is possible ----- */
		if (piece.m_position.m_x + 1 < NUM_FILES)
		{
			newPos = piece.m_position + forward + RIGHT;

			if (game.m_board.posIsOccupiedByColor(newPos, ~player))
			{
				moves.push_back(newPos);
			}
		}

		if (piece.m_position.m_x - 1 >= 0)
		{
			newPos = piece.m_position + forward + LEFT;

			if (game.m_board.posIsOccupiedByColor(newPos, ~player))
			{
				moves.push_back(newPos);
			}
		}

		/* ----- Check if en passant is possible ----- */
		if (!game.m_moveHistory.empty())
		{
			MoveNode prevMove = game.m_moveHistory.back();

			// If previous move was made by an opposing pawn
			if (prevMove.m_pieceType == PAWN && prevMove.m_color != player)
			{
				newPos = piece.m_position + forward;

				if (prevMove.m_prevPos == (newPos + forward + LEFT) && prevMove.m_currPos == (piece.m_position + LEFT))
				{
					moves.push_back(newPos + LEFT);
				}
				else if (prevMove.m_prevPos == (newPos + forward + RIGHT) && prevMove.m_currPos == (piece.m_position + RIGHT))
				{
					moves.push_back(newPos + RIGHT);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void Move::getValidMovesKnight(const Color player, const PieceNode& piece, 
///         const ChessState& game, std::vector<Position>& moves)
///
/// \brief:  Get valid moves for a knight
///
/// \param [in]:  player : color of knight
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  game : state of board
/// \param [out]:  moves : stores valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
void Move::getValidMovesKnight(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves)
{
	// Add all possible knight moves to vector
	// std::vector<Position> destinations = {
	//     piece.m_position + UP * 2 + RIGHT,
	//     piece.m_position + UP * 2 + LEFT,
	//     piece.m_position + DOWN * 2 + RIGHT,
	//     piece.m_position + DOWN * 2 + LEFT,
	//     piece.m_position + LEFT * 2 + UP,
	//     piece.m_position + LEFT * 2 + DOWN,
	//     piece.m_position + RIGHT * 2 + UP,
	//     piece.m_position + RIGHT * 2 + DOWN
	// };

	// Add all possible knight moves to vector
	static const std::vector<Position> knightMoves = {
		UP * 2 + RIGHT,
		UP * 2 + LEFT,
		DOWN * 2 + RIGHT,
		DOWN * 2 + LEFT,
		LEFT * 2 + UP,
		LEFT * 2 + DOWN,
		RIGHT * 2 + UP,
		RIGHT * 2 + DOWN
	};

	for (const Position& move : knightMoves)
	{
		Position newPos = piece.m_position + move;

		// If position exists on board
		if (newPos.m_x >= 0 && newPos.m_x < NUM_FILES
			&& newPos.m_y >= 0 && newPos.m_y < NUM_RANKS)
		{
			// If piece of same color is not in new position
			if (!game.m_board.posIsOccupiedByColor(newPos, player))
			{
				moves.push_back(newPos);
			}
		}
	}

	// for(const Position& newPos : destinations)
	// {
	//     // If position exists on board
	//     if(newPos.m_x >= 0 && newPos.m_x < NUM_FILES
	//         && newPos.m_y >= 0 && newPos.m_y < NUM_RANKS)
	//     {
	//         // If piece of same color is not in new position
	//         if(!game.m_board.posIsOccupiedByColor(newPos, player))
	//         {
	//             moves.push_back(newPos);
	//         }
	//     }
	// }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void Move::getValidMovesLinear(const Color player, const PieceNode& piece, 
///         const ChessState& game, const std::vector<Position>& directions, std::vector<Position>& moves)
///
/// \brief:  Get valid moves for a piece that moves in a straight line (bishop, rook, queen)
///
/// \param [in]:  player : color of piece
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  game : state of board
/// \param [in]:  directions : vector of directions piece can travel in
/// \param [out]:  moves : stores valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
void Move::getValidMovesLinear(const Color player, const PieceNode& piece, const ChessState& game,
	const std::vector<Position>& directions, std::vector<Position>& moves)
{
	/* ----- Check each position in each direction ----- */
	for (const Position& direction : directions)
	{
		Position newPos = piece.m_position + direction;
		bool inBounds = newPos.m_x >= 0 && newPos.m_x < NUM_FILES &&
			newPos.m_y >= 0 && newPos.m_y < NUM_RANKS;

		while (inBounds && !game.m_board.posIsOccupied(newPos))
		{
			moves.push_back(newPos);

			newPos += direction;
			inBounds = newPos.m_x >= 0 && newPos.m_x < NUM_FILES &&
				newPos.m_y >= 0 && newPos.m_y < NUM_RANKS;
		}

		// If a capture at new position is possible
		if (inBounds && game.m_board.posIsOccupiedByColor(newPos, ~player))
		{
			moves.push_back(newPos);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void Move::getValidMovesKing(const Color player, const PieceNode& piece, 
///         const ChessState& game, std::vector<Position>& moves)
///
/// \brief:  Get valid moves for a king
///
/// \param [in]:  player : color of king
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  game : state of board
/// \param [out]:  moves : stores valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
void Move::getValidMovesKing(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves)
{
	// // Add all possible king moves to vector
	// std::vector<Position> destinations = {
	//     piece.m_position + UP,
	//     piece.m_position + DOWN,
	//     piece.m_position + LEFT,
	//     piece.m_position + RIGHT,
	//     piece.m_position + UP + RIGHT,
	//     piece.m_position + UP + LEFT,
	//     piece.m_position + DOWN + RIGHT,
	//     piece.m_position + DOWN + LEFT
	// };

	// // Loop through each possible move
	// for(const Position& newPos : destinations)
	// {
	//     bool inBounds = newPos.m_x >= 0 && newPos.m_x < NUM_FILES &&
	//                     newPos.m_y >= 0 && newPos.m_y < NUM_RANKS;

	//     // If position exists on board and does not contain piece of same color
	//     if(inBounds && !game.m_board.posIsOccupiedByColor(newPos, player))
	//     {
	//         moves.push_back(newPos);
	//     }
	// }
	// Add all possible king moves to vector
	static const std::vector<Position> kingMoves = {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		UP + RIGHT,
		UP + LEFT,
		DOWN + RIGHT,
		DOWN + LEFT
	};

	// Loop through each possible move
	for (const Position& move : kingMoves)
	{
		Position newPos = piece.m_position + move;

		bool inBounds = newPos.m_x >= 0 && newPos.m_x < NUM_FILES &&
			newPos.m_y >= 0 && newPos.m_y < NUM_RANKS;

		// If position exists on board and does not contain piece of same color
		if (inBounds && !game.m_board.posIsOccupiedByColor(newPos, player))
		{
			moves.push_back(newPos);
		}
	}

	/* ----- CHECK IF CASTLING IS POSSIBLE ----- */
	// static auto castlingCheck = [&player, &piece, &game, &moves](const Position& direction)
	// {
	//     bool blocked = false;
	//     Position pos = piece.m_position + direction;

	//     // Set block to true if there are any pieces between king and rook
	//     while(!blocked && pos.m_x < NUM_FILES - 1 && pos.m_x > 0)
	//     {
	//         blocked = game.m_board.posIsOccupied(pos);
	//         pos += direction;
	//     }

	//     if(!blocked)
	//     {
	//         if(!inCheck(player, game))
	//         {
	//             // Set safePath to false if any position the king would pass over could be attacked
	//             //      (this include the position the king lands on)
	//             bool safePath = !(isUnderAttack(player, game, piece.m_position + direction)
	//                 || isUnderAttack(player, game, piece.m_position + direction * 2));

	//             if(safePath)
	//             {
	//                 moves.push_back(piece.m_position + direction * 2);
	//             }
	//         }
	//     }
	// };

	// If king can kind-side castle
	if (canCastle(player, piece, game, true))
	{
		moves.push_back(piece.m_position + RIGHT * 2);
	}

	// If king can queen-side castle
	if (canCastle(player, piece, game, false))
	{
		moves.push_back(piece.m_position + LEFT * 2);
	}

	// bool kingSideCastling = (player == WHITE ? game.m_wKingSideCastle : game.m_bKingSideCastle);

	// if(kingSideCastling)
	// {
	//     castlingCheck(RIGHT);
	// }

	// bool queenSideCastling = (player == WHITE ? game.m_wQueenSideCastle : game.m_bQueenSideCastle);

	// if(queenSideCastling)
	// {
	//     castlingCheck(LEFT);
	// }
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Move::isUnderAttack(const Color player, const ChessState& game, const Position& position)
///
/// \brief:  Determine whether an enemy piece can capture a position
///
/// \param [in]:  player : color of ally player
/// \param [in]:  game : state of board
/// \param [in]:  position : position to be checked
///
/// \return:  bool : true if enemy can move to position, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool Move::isUnderAttack(const Color player, const ChessState& game, const Position& position)
{
	bool underAttack = false;
	Color enemy = ~player;
	std::vector<PieceNode>::const_iterator enemyPiece, end;

	if (player == WHITE)
	{
		enemyPiece = game.m_blackPieces.begin();
		end = game.m_blackPieces.end();
	}
	else // player == BLACK
	{
		enemyPiece = game.m_whitePieces.begin();
		end = game.m_whitePieces.end();
	}

	// Check if moving to position is valid for any opposing piece
	while (!underAttack && enemyPiece != end)
	{
		underAttack = isValidMove(enemy, *enemyPiece, position, game);
		enemyPiece++;
	}

	return underAttack;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Move::canCastle(const Color player, const PieceNode& piece, 
///         const ChessState& game, const bool kingSide)
///
/// \brief:  Determine whether a king can castle
///
/// \param [in]:  player : color of ally player
/// \param [in]:  piece : contains position and type of piece
/// \param [in]:  game : state of board
/// \param [in]:  kingSide : true if king-side castle is being check, otherwise check queen-side
///
/// \return:  bool : true if king can castle, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool Move::canCastle(const Color player, const PieceNode& piece, const ChessState& game, const bool kingSide)
{
	bool canCastle;
	Position direction;

	if (kingSide)
	{
		canCastle = (player == WHITE ? game.m_wKingSideCastle : game.m_bKingSideCastle);
		direction = RIGHT;
	}
	else
	{
		canCastle = (player == WHITE ? game.m_wQueenSideCastle : game.m_bQueenSideCastle);
		direction = LEFT;
	}

	if (canCastle)
	{
		bool blocked = false;
		Position pos = piece.m_position + direction;

		// Set block to true if there are any pieces between king and rook
		while (!blocked && pos.m_x < NUM_FILES - 1 && pos.m_x > 0)
		{
			blocked = game.m_board.posIsOccupied(pos);
			pos += direction;
		}

		if (!blocked && !inCheck(player, game))
		{
			// Set safePath to false if any position the king would pass over could be attacked
			//      (this include the position the king lands on)
			bool safePath = !(isUnderAttack(player, game, piece.m_position + direction)
				|| isUnderAttack(player, game, piece.m_position + direction * 2));

			if (safePath)
			{
				return true;
			}
		}
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::vector<Position> Move::getValidMoves(const Color player, const PieceNode& piece, 
///         const ChessState& game)
///
/// \brief:  Get valid moves for a piece
///
/// \param [in]:  player : color of piece
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  game : state of board
///
/// \return:  std::vector<Position> : contains list of valid moves
///
/////////////////////////////////////////////////////////////////////////////////////////////
std::vector<Position> Move::getValidMoves(const Color player, const PieceNode& piece, const ChessState& game)
{
	static const std::vector<Position> bishopDirections = { UP + LEFT, UP + RIGHT, DOWN + LEFT, DOWN + RIGHT };
	static const std::vector<Position> rookDirections = { UP, DOWN, LEFT, RIGHT };
	static const std::vector<Position> queenDirections = {
		UP + LEFT,
		UP + RIGHT,
		DOWN + LEFT,
		DOWN + RIGHT,
		UP,
		DOWN,
		LEFT,
		RIGHT
	};

	// Stores resulting position of each valid move
	std::vector<Position> moves;

	// Generate valid moves based on piece type
	switch (piece.m_pieceType)
	{
	case PAWN:
		getValidMovesPawn(player, piece, game, moves);
		break;

	case KNIGHT:
		getValidMovesKnight(player, piece, game, moves);
		break;

	case BISHOP:
		getValidMovesLinear(player, piece, game, bishopDirections, moves);
		break;

	case ROOK:
		getValidMovesLinear(player, piece, game, rookDirections, moves);
		break;

	case QUEEN:
		getValidMovesLinear(player, piece, game, queenDirections, moves);
		break;

	case KING:
		getValidMovesKing(player, piece, game, moves);
		break;

	default:
		break;
	}

	std::vector<Position>::iterator move = moves.begin();

	// Remove all moves that result in player being in check
	while (move != moves.end())
	{
		ChessState gameCopy(game);

		std::vector<PieceNode>::iterator pieceCopy = (player == Color::WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
			end = (player == Color::WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

		// Find piece in game copy
		while (pieceCopy != end && pieceCopy->m_position != piece.m_position)
		{
			pieceCopy++;
		}

		makeMove(player, *pieceCopy, *move, gameCopy);

		if (inCheck(player, gameCopy))
		{
			// Erase current move and set move equal to next move
			move = moves.erase(move);
		}
		else
		{
			move++;
		}
	}

	return moves;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Move::isValidMove(const Color player, const PieceNode& piece, 
///         const Position& destination, const ChessState& game)
///
/// \brief:  Determine whether a move is valid for a piece
///
/// \param [in]:  player : color of piece
/// \param [in]:  piece : contains position and piece type
/// \param [in]:  destination : end position of move being checked
/// \param [in]:  game : state of board
///
/// \return:  bool : true if piece can move to destination, false otherwise
///
/// \note: returns true even if move results in player being put in check
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool Move::isValidMove(const Color player, const PieceNode& piece, const Position& destination, const ChessState& game)
{
	// If piece has moved and is in bounds
	if (destination.m_x >= 0 && destination.m_x < NUM_FILES &&
		destination.m_y >= 0 && destination.m_y < NUM_RANKS &&
		destination != piece.m_position)
	{
		switch (piece.m_pieceType)
		{
			/* ---------- PAWN ---------- */
		case PAWN:
		{
			const Position FORWARD = (player == WHITE ? UP : DOWN);
			const int ORIGINAL_ROW = (player == WHITE ? NUM_RANKS - 2 : 1);

			// If pawn is moving one space forward
			if (destination == (piece.m_position + FORWARD))
			{
				return !game.m_board.posIsOccupied(destination);
			}
			// If pawn is moving two spaces forward
			else if (destination == (piece.m_position + FORWARD * 2))
			{
				return !game.m_board.posIsOccupied(piece.m_position + FORWARD) && !game.m_board.posIsOccupied(destination)
					&& piece.m_position.m_y == ORIGINAL_ROW;
			}
			// If pawn is moving diagonal
			else if (destination == (piece.m_position + FORWARD + LEFT) || destination == (piece.m_position + FORWARD + RIGHT))
			{
				if (game.m_board.posIsOccupiedByColor(destination, ~player))
				{
					return true;
				}
				// En Passant
				else if (!game.m_moveHistory.empty() && game.m_moveHistory.back().m_pieceType == PAWN)
				{
					MoveNode prevMove = game.m_moveHistory.back();

					return prevMove.m_prevPos == (destination + FORWARD) && prevMove.m_currPos == (destination - FORWARD);
				}
			}

			return false;
		}

		/* ---------- KNIGHT ---------- */
		case KNIGHT:
		{
			int deltaXsq = destination.m_x - piece.m_position.m_x,
				deltaYsq = destination.m_y - piece.m_position.m_y;

			deltaXsq = deltaXsq * deltaXsq;
			deltaYsq = deltaYsq * deltaYsq;

			if (((deltaXsq == 4 && deltaYsq == 1) || (deltaXsq == 1 && deltaYsq == 4))
				&& !game.m_board.posIsOccupiedByColor(destination, player))
			{
				return true;
			}

			return false;
		}

		/* ---------- BISHOP ---------- */
		case BISHOP:
		{
			int deltaX = destination.m_x - piece.m_position.m_x;
			Position direction(0, 0);

			// If piece is moving in an up right or down left diagonal
			if ((destination.m_y + deltaX) == piece.m_position.m_y)
			{
				direction = (deltaX < 0 ? DOWN + LEFT : UP + RIGHT);
			}
			// If piece is moving in an up left or down right diagonal
			else if ((destination.m_y - deltaX) == piece.m_position.m_y)
			{
				direction = (deltaX < 0 ? UP + LEFT : DOWN + RIGHT);
			}
			else
			{
				return false;
			}

			for (Position index = piece.m_position + direction; index != destination; index += direction)
			{
				if (game.m_board.posIsOccupied(index))
				{
					return false;
				}
			}

			// Return true if destination is not occupied by same color
			return !game.m_board.posIsOccupiedByColor(destination, player);
		}

		/* ---------- ROOK ---------- */
		case ROOK:
		{
			Position direction(0, 0);

			// If piece is moving horizontally
			if (destination.m_y == piece.m_position.m_y)
			{
				direction = (destination.m_x < piece.m_position.m_x ? LEFT : RIGHT);
			}
			// If piece is moving vertically
			else if (destination.m_x == piece.m_position.m_x)
			{
				direction = (destination.m_y < piece.m_position.m_y ? UP : DOWN);
			}
			else
			{
				return false;
			}

			for (Position index = piece.m_position + direction; index != destination; index += direction)
			{
				if (game.m_board.posIsOccupied(index))
				{
					return false;
				}
			}

			return !game.m_board.posIsOccupiedByColor(destination, player);
		}

		/* ---------- QUEEN ---------- */
		case QUEEN:
		{
			PieceNode bishop(piece), rook(piece);
			bishop.m_pieceType = BISHOP;
			rook.m_pieceType = ROOK;

			// Return true if move is valid for bishop or rook
			return isValidMove(player, bishop, destination, game) || isValidMove(player, rook, destination, game);
		}

		/* ---------- KING ---------- */
		case KING:
		{
			int deltaX = destination.m_x - piece.m_position.m_x,
				deltaY = destination.m_y - piece.m_position.m_y;

			if (deltaX * deltaX <= 1 && deltaY * deltaY <= 1)
			{
				return !game.m_board.posIsOccupiedByColor(destination, player);
			}
			else if (deltaY == 0)
			{
				// kingside castling
				if (deltaX == 2)
				{
					return canCastle(player, piece, game, true);
				}
				// queenside castling
				else if (deltaX == -2)
				{
					return canCastle(player, piece, game, false);
				}
			}

			return false;
		}

		default:
			return false;
		}
	}

	// TODO: return false if move results in player being put in check

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Move::inCheck(const Color player, const ChessState& game)
///
/// \brief:  Determine whether a player is in check
///
/// \param [in]:  player : color of player being examined
/// \param [in]:  game : state of board
///
/// \return:  bool : true if player is in check, false otherwise
///
/////////////////////////////////////////////////////////////////////////////////////////////
bool Move::inCheck(const Color player, const ChessState& game)
{
	std::vector<PieceNode> allies = (player == WHITE ? game.getWhitePieces() : game.getBlackPieces());
	Position kingPos;
	std::vector<PieceNode>::const_iterator piece = allies.begin();

	// Find ally king's current position
	while (piece != allies.end() && piece->m_pieceType != KING)
	{
		piece++;
	}

	kingPos = piece->m_position;

	// Return true if king's position can be attacked by opposing player
	//      return false otherwise
	return isUnderAttack(player, game, kingPos);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  void Move::makeMove(const Color player, PieceNode& piece, const Position& destination, 
///         ChessState& game, const PieceType promotion = QUEEN)
///
/// \brief:  Move a piece from one position to another
///
/// \param [in]:  player : color of player being examined
/// \param [in]:  destination : position piece is being moved to
/// \param [in]:  game : state of board
/// \param [in]:  promotion : piece type a pawn will be promoted to if a promotion occurs
///
/// \note:  assumes move being made is valid
///
/////////////////////////////////////////////////////////////////////////////////////////////
void Move::makeMove(const Color player, PieceNode& piece, const Position& destination, ChessState& game, const PieceType promotion)
{
	if (piece.m_pieceType == PAWN)
	{
		// Pawn advancement resets half turns to 0
		game.m_numHalfTurns = -1; // (half turns are incremented at end of function)

		// If an en passant occurs
		if ((destination.m_x - piece.m_position.m_x) != 0 && !game.m_board.posIsOccupied(destination))
		{
			Position backward;
			std::vector<PieceNode>* enemies;

			if (player == WHITE)
			{
				backward = DOWN;
				enemies = &game.m_blackPieces;
			}
			else // player == BLACK
			{
				backward = UP;
				enemies = &game.m_whitePieces;
			}

			/* ----- Remove captured piece ----- */
			game.m_board.clearPos(destination + backward, ~player);

			std::vector<PieceNode>::iterator enemyPiece = enemies->begin(),
				end = enemies->end();

			// Find captured piece in opposing player's piece vector
			while (enemyPiece != end && enemyPiece->m_position != (destination + backward))
			{
				enemyPiece++;
			}

			enemies->erase(enemyPiece);
		}

		int endOfBoard = (player == WHITE ? 0 : NUM_RANKS - 1);

		// If a promotion can occur
		if (destination.m_y == endOfBoard)
		{
			piece.m_pieceType = promotion;

			// Update board
			game.m_board.clearPos(piece.m_position, player, PAWN);
			game.m_board.addPiece(piece.m_position, player, promotion);
		}
	}

	else if (piece.m_pieceType == KING)
	{
		int deltaX = destination.m_x - piece.m_position.m_x;

		// If castling occurs
		if (deltaX * deltaX > 1)
		{
			Position oldRookPosition, newRookPosition;

			oldRookPosition.m_y = piece.m_position.m_y;

			// If kingside castling occurs
			if (deltaX > 1)
			{
				oldRookPosition.m_x = NUM_FILES - 1;
				newRookPosition = destination + LEFT;
			}
			// If queenside castling occurs
			else
			{
				oldRookPosition.m_x = 0;
				newRookPosition = destination + RIGHT;
			}

			/* ----- Move Rook ----- */
			// Update board
			game.m_board.clearPos(oldRookPosition, player, ROOK);
			game.m_board.addPiece(newRookPosition, player, ROOK);

			std::vector<PieceNode>::iterator allyPiece, end;

			if (player == WHITE)
			{
				allyPiece = game.m_whitePieces.begin();
				end = game.m_whitePieces.end();
			}
			else // player == BLACK
			{
				allyPiece = game.m_blackPieces.begin();
				end = game.m_blackPieces.end();
			}

			while (allyPiece != end && allyPiece->m_position != oldRookPosition)
				allyPiece++;

			// Update Rooks position
			allyPiece->m_position = newRookPosition;

		}

		// Castling is not allowed after a king moves
		if (player == WHITE)
		{
			game.m_wKingSideCastle = false;
			game.m_wQueenSideCastle = false;
		}
		else // player == BLACK
		{
			game.m_bKingSideCastle = false;
			game.m_bQueenSideCastle = false;
		}
	}

	else if (piece.m_pieceType == ROOK)
	{
		// If black queen side rook is moved
		if (piece.m_position == Position(0, 0))
		{
			game.m_bQueenSideCastle = false;
		}
		// If black king side rook is moved
		else if (piece.m_position == Position(NUM_FILES - 1, 0))
		{
			game.m_bKingSideCastle = false;
		}
		// If white queen side rook is moved
		else if (piece.m_position == Position(0, NUM_RANKS - 1))
		{
			game.m_wQueenSideCastle = false;
		}
		// If white king side rook is moved
		else if (piece.m_position == Position(NUM_FILES - 1, NUM_RANKS - 1))
		{
			game.m_wKingSideCastle = false;
		}
	}

	// If a capture occura
	if (game.m_board.posIsOccupiedByColor(destination, ~player))
	{
		// If a rook is captured
		if (game.m_board.posIsOccupiedByColorPiece(destination, ~player, ROOK))
		{
			if (destination.m_x == 0)
			{
				bool* queenSideCastle = (player == WHITE ? &game.m_bQueenSideCastle : &game.m_wQueenSideCastle);

				// Opponent cannot queen side castle if queen side rook has been captured
				*queenSideCastle = false;
			}
			else if (destination.m_x == NUM_FILES - 1)
			{
				bool* kingSideCastle = (player == WHITE ? &game.m_bKingSideCastle : &game.m_wKingSideCastle);

				// Opponent cannot king side castle if king side rook has been captured
				*kingSideCastle = false;
			}
		}

		// Half turns are reset when a capture occurs
		game.m_numHalfTurns = -1;
		std::vector<PieceNode>* enemies = (player == Color::WHITE ? &game.m_blackPieces : &game.m_whitePieces);

		std::vector<PieceNode>::iterator enemyPiece = enemies->begin(),
			end = enemies->end();

		while (enemyPiece != end && enemyPiece->m_position != destination)
		{
			enemyPiece++;
		}

		// Remove captured piece
		game.m_board.clearPos(destination, ~player, enemyPiece->m_pieceType);
		enemies->erase(enemyPiece);
	}

	game.m_numHalfTurns += 1;

	/* ----- Update move history ----- */
	game.m_moveHistory.push_back(MoveNode(player, piece, destination));
	// Only last 8 moves need to be kept in move history
	if (game.m_moveHistory.size() > 8)
	{
		game.m_moveHistory.pop_front();
	}

	// If it has been 8 half turns since last pawn advance or capture, check for tie
	if (game.m_moveHistory.size() >= 8 && game.m_numHalfTurns >= 8)
	{
		bool tie = true;
		int prevMoveIndex = 0,
			moveHistorySize = (int)game.m_moveHistory.size();

		// Check if game is a draw
		while (tie && prevMoveIndex < (moveHistorySize / 2))
		{
			tie = game.m_moveHistory[prevMoveIndex] == game.m_moveHistory[prevMoveIndex + 4];
			prevMoveIndex += 1;
		}

		// If a draw has occured, indicate it by setting next turn to neutral
		if (tie)
		{
			game.m_nextTurn = NEUTRAL;
		}
	}

	/* ----- Move Piece ----- */
	// Update board
	game.m_board.clearPos(piece.m_position, player, piece.m_pieceType);
	game.m_board.addPiece(destination, player, piece.m_pieceType);
	// Update piece's position
	piece.m_position = destination;

	// Update turn
	if (game.m_nextTurn != NEUTRAL)
	{
		game.m_nextTurn = ~player;
	}

	// Increment full turns if black moved
	if (game.m_nextTurn == Color::BLACK)
	{
		game.m_numFullTurns += 1;
	}
}