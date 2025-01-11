#include "moveUtil.h"

#include "../constants.h"
#include "../enum.h"
#include "../util/utility.h"
#include "../chess.h"
#include "../util/bitboard/bitboardUtil.h"
#include "move.h"
#include "moveGeneration.h"

#define UP Position::UP
#define DOWN Position::DOWN
#define LEFT Position::LEFT
#define RIGHT Position::RIGHT

using util::operator~;
using util::toPosition;
using util::Position;

using util::bitboard::BitboardSet;

namespace move
{
	// Forward declaration
	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player, const bool removeChecks, const bool castling);

	bool isUnderAttack(const Color player, const ChessState& chessState, const Position& position)
	{
		const auto predicate = [&position](const Move& move)
			{
				return move.destination == position;
			};
		const std::vector<Move> enemyMoves = getValidMoves(chessState, ~player, false, false);
		const auto it = std::find_if(enemyMoves.begin(), enemyMoves.end(), predicate);

		return it != enemyMoves.end();
	}

	bool canCastle(const Color player, const ChessState& game, const bool kingSide)
	{
		bool canCastle;
		Position direction;
		const Position kingPosition = ChessState::KING_START_POS[player];

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
			Position position = kingPosition + direction;

			// Set block to true if there are any pieces between king and rook
			while (!blocked && position.x < FILE_COUNT - 1 && position.x > 0)
			{
				blocked = game.m_board.posIsOccupied(position);
				position += direction;
			}

			if (!blocked && !inCheck(player, game))
			{
				// Set safePath to false if any position the king would pass over could be attacked
				//      (this include the position the king lands on)
				bool safePath = !(isUnderAttack(player, game, position + direction)
					|| isUnderAttack(player, game, position + direction * 2));

				if (safePath)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player, const bool removeChecks, const bool castling)
	{
		std::vector<Move> result = generatePawnMoves(chessState, player);

		std::vector<Move> knightMoves = generateKnightMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(knightMoves.begin()), std::make_move_iterator(knightMoves.end()));

		std::vector<Move> bishopMoves = generateBishopMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(bishopMoves.begin()), std::make_move_iterator(bishopMoves.end()));

		std::vector<Move> rookMoves = generateRookMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(rookMoves.begin()), std::make_move_iterator(rookMoves.end()));

		std::vector<Move> queenMoves = generateQueenMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(queenMoves.begin()), std::make_move_iterator(queenMoves.end()));

		std::vector<Move> kingMoves = generateKingMoves(chessState, player);
		result.insert(result.end(), std::make_move_iterator(kingMoves.begin()), std::make_move_iterator(kingMoves.end()));

		const Position kingStartPosition = ChessState::KING_START_POS[player];
		if (castling && canCastle(player, chessState, true))
		{
			result.emplace_back(kingStartPosition, kingStartPosition + RIGHT * 2);
		}

		if (castling && canCastle(player, chessState, false))
		{
			result.emplace_back(kingStartPosition, kingStartPosition + LEFT * 2);
		}

		if (removeChecks)
		{
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
		}

		return result;
	}

	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player)
	{
		return getValidMoves(chessState, player, true, true);
	}

	bool isValidMove(const Color player, const Position& source, const Position& destination, const ChessState& chessState)
	{
		if (source == destination)
		{
			return false;
		}

		if (destination.x < 0 || destination.x >= FILE_COUNT ||
			destination.y < 0 || destination.y >= RANK_COUNT)
		{
			return false;
		}

		const BitboardSet& board = chessState.getBoard();
		if (!board.posIsOccupied(source, player))
		{
			return false;
		}

		if (board.posIsOccupied(destination, player))
		{
			return false;
		}

		const PieceType pieceType = board.getPieceType(source);

		switch (pieceType)
		{
		case PAWN:
		{
			const Position FORWARD = (player == WHITE ? UP : DOWN);
			const int ORIGINAL_ROW = ChessState::PAWN_START_ROW[player];

			if (destination == source + FORWARD)
			{
				return !chessState.m_board.posIsOccupied(destination);
			}
			else if (destination == source + FORWARD * 2)
			{
				return !chessState.m_board.posIsOccupied(destination)
					&& !chessState.m_board.posIsOccupied(source + FORWARD)
					&& source.y == ORIGINAL_ROW;
			}
			else if (destination == (source + FORWARD + LEFT) || destination == (source + FORWARD + RIGHT))
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
		case KNIGHT:
		{
			int deltaXsq = destination.x - source.x,
				deltaYsq = destination.y - source.y;

			deltaXsq = deltaXsq * deltaXsq;
			deltaYsq = deltaYsq * deltaYsq;

			if ((deltaXsq == 4 && deltaYsq == 1) || (deltaXsq == 1 && deltaYsq == 4))
			{
				return true;
			}

			return false;
		}
		case BISHOP:
		{
			int deltaX = destination.x - source.x;
			Position direction(0, 0);

			if ((destination.y + deltaX) == source.y)
			{
				direction = (deltaX < 0 ? DOWN + LEFT : UP + RIGHT);
			}
			else if ((destination.y - deltaX) == source.y)
			{
				direction = (deltaX < 0 ? UP + LEFT : DOWN + RIGHT);
			}
			else
			{
				return false;
			}

			for (Position index = source + direction; index != destination; index += direction)
			{
				if (chessState.m_board.posIsOccupied(index))
				{
					return false;
				}
			}

			return true;
		}
		case ROOK:
		{
			Position direction(0, 0);

			if (destination.y == source.y)
			{
				direction = (destination.x < source.x ? LEFT : RIGHT);
			}
			else if (destination.x == source.x)
			{
				direction = (destination.y < source.y ? UP : DOWN);
			}
			else
			{
				return false;
			}

			for (Position index = source + direction; index != destination; index += direction)
			{
				if (chessState.m_board.posIsOccupied(index))
				{
					return false;
				}
			}

			return true;
		}
		case QUEEN:
		{
			int deltaX = destination.x - source.x;
			Position direction(0, 0);

			if (deltaX == 0)
			{
				if (destination.y == source.y)
				{
					direction = (destination.x < source.x ? LEFT : RIGHT);
				}
				else if (destination.x == source.x)
				{
					direction = (destination.y < source.y ? UP : DOWN);
				}
				else
				{
					return false;
				}
			}
			else
			{
				if ((destination.y + deltaX) == source.y)
				{
					direction = (deltaX < 0 ? DOWN + LEFT : UP + RIGHT);
				}
				else if ((destination.y - deltaX) == source.y)
				{
					direction = (deltaX < 0 ? UP + LEFT : DOWN + RIGHT);
				}
				else
				{
					return false;
				}
			}

			for (Position index = source + direction; index != destination; index += direction)
			{
				if (chessState.m_board.posIsOccupied(index))
				{
					return false;
				}
			}

			return true;
		}
		case KING:
		{
			int deltaX = destination.x - source.x,
				deltaY = destination.y - source.y;

			if (deltaX * deltaX <= 1 && deltaY * deltaY <= 1)
			{
				return true;
			}
			else if (deltaY == 0)
			{
				// kingside castling
				if (deltaX == 2)
				{
					return canCastle(player, chessState, true);
				}
				// queenside castling
				else if (deltaX == -2)
				{
					return canCastle(player, chessState, false);
				}
			}

			return false;
		}

		default:
			return false;
		}

		// TODO return false if move results in player being put in check
		return false;
	}

	bool inCheck(const Color player, const ChessState& chessState)
	{
		const BitboardSet& board = chessState.getBoard();
		Bitboard kingBoard = board.getBitboard(player, PieceType::KING);
		const int kingPositionIndex = util::bitboard::popLsb(kingBoard);
		const Position kingPosition = util::toPosition(kingPositionIndex);

		// Return true if king's position can be attacked by opposing player
		//      return false otherwise
		return isUnderAttack(player, chessState, kingPosition);
	}

	void makeMove(const Color player, const Move move, ChessState& chessState, const PieceType promotion)
	{
		makeMove(player, move.source, move.destination, chessState, promotion);
	}

	void makeMove(const Color player, const Position& source, const Position& destination, ChessState& chessState, const PieceType promotion)
	{
		const BitboardSet& board = chessState.getBoard();
		PieceType pieceType = board.getPieceType(source, player);

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
			chessState.m_numHalfTurns = -1;

			// If an en passant occurs
			if ((destination.x - source.x) != 0 && !chessState.m_board.posIsOccupied(destination))
			{
				const Position backward = player == Color::WHITE ? DOWN : UP;
				chessState.m_board.clearPos(destination + backward, ~player);
			}

			int endOfBoard = (player == WHITE ? 0 : RANK_COUNT - 1);
			// promotion
			if (destination.y == endOfBoard)
			{
				chessState.m_board.clearPos(source, player, PieceType::PAWN);
				chessState.m_board.addPiece(source, player, promotion);
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

				chessState.m_board.clearPos(oldRookPosition, player, PieceType::ROOK);
				chessState.m_board.addPiece(newRookPosition, player, PieceType::ROOK);
			}

			if (player == Color::WHITE)
			{
				chessState.m_wKingSideCastle = false;
				chessState.m_wQueenSideCastle = false;
			}
			else // player == Color::BLACK
			{
				chessState.m_bKingSideCastle = false;
				chessState.m_bQueenSideCastle = false;
			}
		}

		else if (pieceType == PieceType::ROOK)
		{
			if (source == Position(0, 0))
			{
				chessState.m_bQueenSideCastle = false;
			}
			else if (source == Position(FILE_COUNT - 1, 0))
			{
				chessState.m_bKingSideCastle = false;
			}
			else if (source == Position(0, RANK_COUNT - 1))
			{
				chessState.m_wQueenSideCastle = false;
			}
			else if (source == Position(FILE_COUNT - 1, RANK_COUNT - 1))
			{
				chessState.m_wKingSideCastle = false;
			}
		}

		// capture
		if (chessState.m_board.posIsOccupied(destination, ~player))
		{
			if (chessState.m_board.posIsOccupied(destination, ~player, PieceType::ROOK))
			{
				if (destination.x == 0)
				{
					if (player == Color::WHITE)
					{
						chessState.m_bQueenSideCastle = false;
					}
					else // player == Color::BLACK
					{
						chessState.m_wQueenSideCastle = false;
					}
				}
				else if (destination.x == FILE_COUNT - 1)
				{
					if (player == Color::WHITE)
					{
						chessState.m_bKingSideCastle = false;
					}
					else // player == Color::BLACK
					{
						chessState.m_wKingSideCastle = false;
					}
				}
			}
			chessState.m_board.clearPos(destination, ~player);

			// Half turns are reset when a capture occurs
			//   (half turns are incremented at end of function)
			chessState.m_numHalfTurns = -1;
		}

		chessState.m_numHalfTurns += 1;

		chessState.m_moveHistory.emplace_back(source, destination, player, pieceType);
		if (chessState.m_moveHistory.size() > 8)
		{
			chessState.m_moveHistory.pop_front();
		}

		// check for tie
		if (chessState.m_moveHistory.size() >= 8 && chessState.m_numHalfTurns >= 8)
		{
			bool tie = true;
			int prevMoveIndex = 0,
				moveHistorySize = (int)chessState.m_moveHistory.size();

			while (tie && prevMoveIndex < (moveHistorySize / 2))
			{
				tie = chessState.m_moveHistory[prevMoveIndex] == chessState.m_moveHistory[prevMoveIndex + 4];
				prevMoveIndex += 1;
			}

			if (tie)
			{
				chessState.m_nextTurn = NEUTRAL;
			}
		}

		chessState.m_board.clearPos(source, player, pieceType);
		chessState.m_board.addPiece(destination, player, pieceType);

		if (chessState.m_nextTurn == Color::BLACK)
		{
			chessState.m_numFullTurns += 1;
		}

		if (chessState.m_nextTurn != NEUTRAL)
		{
			chessState.m_nextTurn = ~player;
		}
	}
}