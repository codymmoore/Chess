#include "moveUtil.h"

#include "../constants.h"
#include "../enum.h"
#include "../util/utility.h"
#include "../chess.h"
#include "move.h"
#include "moveGeneration.h"

#define UP Position::UP
#define DOWN Position::DOWN
#define LEFT Position::LEFT
#define RIGHT Position::RIGHT

using namespace util;

namespace move
{
	void getValidMovesKnight(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves)
	{
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
			if (newPos.x >= 0 && newPos.x < FILE_COUNT
				&& newPos.y >= 0 && newPos.y < RANK_COUNT)
			{
				// If piece of same color is not in new position
				if (!game.m_board.posIsOccupied(newPos, player))
				{
					moves.push_back(newPos);
				}
			}
		}
	}

	void getValidMovesLinear(const Color player, const PieceNode& piece, const ChessState& game,
		const std::vector<Position>& directions, std::vector<Position>& moves)
	{
		/* ----- Check each position in each direction ----- */
		for (const Position& direction : directions)
		{
			Position newPos = piece.m_position + direction;
			bool inBounds = newPos.x >= 0 && newPos.x < FILE_COUNT &&
				newPos.y >= 0 && newPos.y < RANK_COUNT;

			while (inBounds && !game.m_board.posIsOccupied(newPos))
			{
				moves.push_back(newPos);

				newPos += direction;
				inBounds = newPos.x >= 0 && newPos.x < FILE_COUNT &&
					newPos.y >= 0 && newPos.y < RANK_COUNT;
			}

			// If a capture at new position is possible
			if (inBounds && game.m_board.posIsOccupied(newPos, ~player))
			{
				moves.push_back(newPos);
			}
		}
	}

	bool isUnderAttack(const Color player, const ChessState& game, const Position& position)
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

	bool canCastle(const Color player, const PieceNode& piece, const ChessState& game, const bool kingSide)
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
			while (!blocked && pos.x < FILE_COUNT - 1 && pos.x > 0)
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

	void getValidMovesKing(const Color player, const PieceNode& piece, const ChessState& game, std::vector<Position>& moves)
	{
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

			bool inBounds = newPos.x >= 0 && newPos.x < FILE_COUNT &&
				newPos.y >= 0 && newPos.y < RANK_COUNT;

			// If position exists on board and does not contain piece of same color
			if (inBounds && !game.m_board.posIsOccupied(newPos, player))
			{
				moves.push_back(newPos);
			}
		}

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
	}

	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player)
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

		const std::vector<PieceNode>& pieces = player == Color::WHITE ? chessState.getWhitePieces() : chessState.getBlackPieces();
		std::vector<Move> result = generatePawnMoves(chessState, player);

		std::vector<Move> knightMoves = generateKnightMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(knightMoves.begin()), std::make_move_iterator(knightMoves.end()));

		std::vector<Move> bishopMoves = generateBishopMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(bishopMoves.begin()), std::make_move_iterator(bishopMoves.end()));

		std::vector<Move> rookMoves = generateRookMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(rookMoves.begin()), std::make_move_iterator(rookMoves.end()));

		std::vector<Move> queenMoves = generateQueenMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(queenMoves.begin()), std::make_move_iterator(queenMoves.end()));

		for (const PieceNode& piece : pieces)
		{
			std::vector<Position> moves;
			switch (piece.m_pieceType)
			{
			case KING:
				getValidMovesKing(player, piece, chessState, moves);
				break;
			default:
				break;
			}

			for (const Position& destination : moves)
			{
				result.emplace_back(piece.m_position, destination);
			}
		}

		std::vector<Move>::iterator move = result.begin();

		while (move != result.end())
		{
			ChessState gameCopy(chessState);

			makeMove(player, *move, gameCopy);

			if (inCheck(player, gameCopy))
			{
				// Erase current move and set move equal to next move
				move = result.erase(move);
			}
			else
			{
				move++;
			}
		}

		return result;
	}

	bool isValidMove(const Color player, const PieceNode& piece, const Position& destination, const ChessState& chessState)
	{
		// If piece has moved and is in bounds
		if (destination.x >= 0 && destination.x < FILE_COUNT &&
			destination.y >= 0 && destination.y < RANK_COUNT &&
			destination != piece.m_position)
		{
			switch (piece.m_pieceType)
			{
				/* ---------- PAWN ---------- */
			case PAWN:
			{
				const Position FORWARD = (player == WHITE ? UP : DOWN);
				const int ORIGINAL_ROW = (player == WHITE ? RANK_COUNT - 2 : 1);

				// If pawn is moving one space forward
				if (destination == (piece.m_position + FORWARD))
				{
					return !chessState.m_board.posIsOccupied(destination);
				}
				// If pawn is moving two spaces forward
				else if (destination == (piece.m_position + FORWARD * 2))
				{
					return !chessState.m_board.posIsOccupied(destination)
						&& !chessState.m_board.posIsOccupied(piece.m_position + FORWARD)
						&& piece.m_position.y == ORIGINAL_ROW;
				}
				// If pawn is moving diagonal
				else if (destination == (piece.m_position + FORWARD + LEFT) || destination == (piece.m_position + FORWARD + RIGHT))
				{
					if (chessState.m_board.posIsOccupied(destination, ~player))
					{
						return true;
					}
					// En Passant
					else if (!chessState.m_moveHistory.empty() && chessState.m_moveHistory.back().m_pieceType == PAWN)
					{
						MoveHistoryNode prevMove = chessState.m_moveHistory.back();

						return prevMove.m_prevPos == (destination + FORWARD) && prevMove.m_currPos == (destination - FORWARD);
					}
				}

				return false;
			}

			/* ---------- KNIGHT ---------- */
			case KNIGHT:
			{
				int deltaXsq = destination.x - piece.m_position.x,
					deltaYsq = destination.y - piece.m_position.y;

				deltaXsq = deltaXsq * deltaXsq;
				deltaYsq = deltaYsq * deltaYsq;

				if (((deltaXsq == 4 && deltaYsq == 1) || (deltaXsq == 1 && deltaYsq == 4))
					&& !chessState.m_board.posIsOccupied(destination, player))
				{
					return true;
				}

				return false;
			}

			/* ---------- BISHOP ---------- */
			case BISHOP:
			{
				int deltaX = destination.x - piece.m_position.x;
				Position direction(0, 0);

				// If piece is moving in an up right or down left diagonal
				if ((destination.y + deltaX) == piece.m_position.y)
				{
					direction = (deltaX < 0 ? DOWN + LEFT : UP + RIGHT);
				}
				// If piece is moving in an up left or down right diagonal
				else if ((destination.y - deltaX) == piece.m_position.y)
				{
					direction = (deltaX < 0 ? UP + LEFT : DOWN + RIGHT);
				}
				else
				{
					return false;
				}

				for (Position index = piece.m_position + direction; index != destination; index += direction)
				{
					if (chessState.m_board.posIsOccupied(index))
					{
						return false;
					}
				}

				// Return true if destination is not occupied by same color
				return !chessState.m_board.posIsOccupied(destination, player);
			}

			/* ---------- ROOK ---------- */
			case ROOK:
			{
				Position direction(0, 0);

				// If piece is moving horizontally
				if (destination.y == piece.m_position.y)
				{
					direction = (destination.x < piece.m_position.x ? LEFT : RIGHT);
				}
				// If piece is moving vertically
				else if (destination.x == piece.m_position.x)
				{
					direction = (destination.y < piece.m_position.y ? UP : DOWN);
				}
				else
				{
					return false;
				}

				for (Position index = piece.m_position + direction; index != destination; index += direction)
				{
					if (chessState.m_board.posIsOccupied(index))
					{
						return false;
					}
				}

				return !chessState.m_board.posIsOccupied(destination, player);
			}

			/* ---------- QUEEN ---------- */
			case QUEEN:
			{
				PieceNode bishop(piece), rook(piece);
				bishop.m_pieceType = BISHOP;
				rook.m_pieceType = ROOK;

				// Return true if move is valid for bishop or rook
				return isValidMove(player, bishop, destination, chessState) || isValidMove(player, rook, destination, chessState);
			}

			/* ---------- KING ---------- */
			case KING:
			{
				int deltaX = destination.x - piece.m_position.x,
					deltaY = destination.y - piece.m_position.y;

				if (deltaX * deltaX <= 1 && deltaY * deltaY <= 1)
				{
					return !chessState.m_board.posIsOccupied(destination, player);
				}
				else if (deltaY == 0)
				{
					// kingside castling
					if (deltaX == 2)
					{
						return canCastle(player, piece, chessState, true);
					}
					// queenside castling
					else if (deltaX == -2)
					{
						return canCastle(player, piece, chessState, false);
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

	bool inCheck(const Color player, const ChessState& chessState)
	{
		std::vector<PieceNode> allies = (player == WHITE ? chessState.getWhitePieces() : chessState.getBlackPieces());
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
		return isUnderAttack(player, chessState, kingPos);
	}

	void makeMove(const Color player, const Move move, ChessState& chessState, const PieceType promotion)
	{
		makeMove(player, move.source, move.destination, chessState, promotion);
	}

	void makeMove(const Color player, const Position& source, const Position& destination, ChessState& chessState, const PieceType promotion)
	{
		std::vector<PieceNode>& pieces = player == WHITE ? chessState.m_whitePieces : chessState.m_blackPieces;
		std::vector<PieceNode>::iterator pieceIt = std::find_if(pieces.begin(), pieces.end(), [&source](const PieceNode& piece) {
			return piece.m_position == source;
			});

		if (pieceIt == pieces.end())
		{
			std::string errorMessage = "Piece not found at position (" + std::to_string(source.x) +
				", " + std::to_string(source.y) + ")";
			throw std::exception(errorMessage.c_str());
		}

		PieceNode& pieceRef = *pieceIt;

		if (pieceRef.m_pieceType == PAWN)
		{
			// Pawn advancement resets half turns to 0
			chessState.m_numHalfTurns = -1; // (half turns are incremented at end of function)

			// If an en passant occurs
			if ((destination.x - pieceRef.m_position.x) != 0 && !chessState.m_board.posIsOccupied(destination))
			{
				Position backward;
				std::vector<PieceNode>* enemies;

				if (player == WHITE)
				{
					backward = DOWN;
					enemies = &chessState.m_blackPieces;
				}
				else // player == BLACK
				{
					backward = UP;
					enemies = &chessState.m_whitePieces;
				}

				/* ----- Remove captured piece ----- */
				chessState.m_board.clearPos(destination + backward, ~player);

				std::vector<PieceNode>::iterator enemyPiece = enemies->begin(),
					end = enemies->end();

				// Find captured piece in opposing player's piece vector
				while (enemyPiece != end && enemyPiece->m_position != (destination + backward))
				{
					enemyPiece++;
				}

				enemies->erase(enemyPiece);
			}

			int endOfBoard = (player == WHITE ? 0 : RANK_COUNT - 1);

			// If a promotion can occur
			if (destination.y == endOfBoard)
			{
				pieceRef.m_pieceType = promotion;

				// Update board
				chessState.m_board.clearPos(source, player, PAWN);
				chessState.m_board.addPiece(source, player, promotion);
			}
		}

		else if (pieceRef.m_pieceType == KING)
		{
			int deltaX = destination.x - source.x;

			// If castling occurs
			if (deltaX * deltaX > 1)
			{
				Position oldRookPosition, newRookPosition;

				oldRookPosition.y = source.y;

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

				/* ----- Move Rook ----- */
				// Update board
				chessState.m_board.clearPos(oldRookPosition, player, ROOK);
				chessState.m_board.addPiece(newRookPosition, player, ROOK);

				std::vector<PieceNode>::iterator allyPiece, end;

				if (player == WHITE)
				{
					allyPiece = chessState.m_whitePieces.begin();
					end = chessState.m_whitePieces.end();
				}
				else // player == BLACK
				{
					allyPiece = chessState.m_blackPieces.begin();
					end = chessState.m_blackPieces.end();
				}

				while (allyPiece != end && allyPiece->m_position != oldRookPosition)
					allyPiece++;

				// Update Rooks position
				allyPiece->m_position = newRookPosition;

			}

			// Castling is not allowed after a king moves
			if (player == WHITE)
			{
				chessState.m_wKingSideCastle = false;
				chessState.m_wQueenSideCastle = false;
			}
			else // player == BLACK
			{
				chessState.m_bKingSideCastle = false;
				chessState.m_bQueenSideCastle = false;
			}
		}

		else if (pieceRef.m_pieceType == ROOK)
		{
			// If black queen side rook is moved
			if (source == Position(0, 0))
			{
				chessState.m_bQueenSideCastle = false;
			}
			// If black king side rook is moved
			else if (source == Position(FILE_COUNT - 1, 0))
			{
				chessState.m_bKingSideCastle = false;
			}
			// If white queen side rook is moved
			else if (source == Position(0, RANK_COUNT - 1))
			{
				chessState.m_wQueenSideCastle = false;
			}
			// If white king side rook is moved
			else if (source == Position(FILE_COUNT - 1, RANK_COUNT - 1))
			{
				chessState.m_wKingSideCastle = false;
			}
		}

		// If a capture occura
		if (chessState.m_board.posIsOccupied(destination, ~player))
		{
			// If a rook is captured
			if (chessState.m_board.posIsOccupied(destination, ~player, ROOK))
			{
				if (destination.x == 0)
				{
					bool* queenSideCastle = (player == WHITE ? &chessState.m_bQueenSideCastle : &chessState.m_wQueenSideCastle);

					// Opponent cannot queen side castle if queen side rook has been captured
					*queenSideCastle = false;
				}
				else if (destination.x == FILE_COUNT - 1)
				{
					bool* kingSideCastle = (player == WHITE ? &chessState.m_bKingSideCastle : &chessState.m_wKingSideCastle);

					// Opponent cannot king side castle if king side rook has been captured
					*kingSideCastle = false;
				}
			}

			// Half turns are reset when a capture occurs
			chessState.m_numHalfTurns = -1;
			std::vector<PieceNode>* enemies = (player == Color::WHITE ? &chessState.m_blackPieces : &chessState.m_whitePieces);

			std::vector<PieceNode>::iterator enemyPiece = enemies->begin(),
				end = enemies->end();

			while (enemyPiece != end && enemyPiece->m_position != destination)
			{
				enemyPiece++;
			}

			// Remove captured piece
			chessState.m_board.clearPos(destination, ~player, enemyPiece->m_pieceType);
			enemies->erase(enemyPiece);
		}

		chessState.m_numHalfTurns += 1;

		/* ----- Update move history ----- */
		chessState.m_moveHistory.push_back(MoveHistoryNode(player, pieceRef, destination));
		// Only last 8 moves need to be kept in move history
		if (chessState.m_moveHistory.size() > 8)
		{
			chessState.m_moveHistory.pop_front();
		}

		// If it has been 8 half turns since last pawn advance or capture, check for tie
		if (chessState.m_moveHistory.size() >= 8 && chessState.m_numHalfTurns >= 8)
		{
			bool tie = true;
			int prevMoveIndex = 0,
				moveHistorySize = (int)chessState.m_moveHistory.size();

			// Check if game is a draw
			while (tie && prevMoveIndex < (moveHistorySize / 2))
			{
				tie = chessState.m_moveHistory[prevMoveIndex] == chessState.m_moveHistory[prevMoveIndex + 4];
				prevMoveIndex += 1;
			}

			// If a draw has occured, indicate it by setting next turn to neutral
			if (tie)
			{
				chessState.m_nextTurn = NEUTRAL;
			}
		}

		/* ----- Move Piece ----- */
		// Update board
		chessState.m_board.clearPos(source, player, pieceRef.m_pieceType);
		chessState.m_board.addPiece(destination, player, pieceRef.m_pieceType);

		// Update piece's position
		pieceRef.m_position = destination;

		// Increment full turns if black moved
		if (chessState.m_nextTurn == Color::BLACK)
		{
			chessState.m_numFullTurns += 1;
		}

		// Update turn
		if (chessState.m_nextTurn != NEUTRAL)
		{
			chessState.m_nextTurn = ~player;
		}
	}
}