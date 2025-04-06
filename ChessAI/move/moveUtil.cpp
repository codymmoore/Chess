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
	bool isInBounds(const Position& position)
	{
		return position.x >= 0 && position.x < FILE_COUNT && position.y >= 0 && position.y < RANK_COUNT;
	}

	bool canBeCapturedByPawn(const ChessState& chessState, const Color player, const Position& position)
	{
		if ((Color::WHITE == player && position.y == 0) || (Color::BLACK == player && position.y == RANK_COUNT - 1))
		{
			return false;
		}

		const Color enemyPlayer = ~player;
		const Position backward = player == Color::WHITE ? UP : DOWN;
		const BitboardSet& board = chessState.getBoard();

		Position attackerPos = position + backward + LEFT;
		if (attackerPos.x >= 0 && board.posIsOccupied(attackerPos, enemyPlayer, PieceType::PAWN))
		{
			return true;
		}

		attackerPos = position + backward + RIGHT;
		return attackerPos.x < RANK_COUNT && board.posIsOccupied(attackerPos, enemyPlayer, PieceType::PAWN);
	}

	bool canBeCapturedByKnight(const ChessState& chessState, const Color player, const Position& position)
	{
		static std::vector<Position> knightDirections = {
			UP + RIGHT * 2,
			UP + LEFT * 2,
			DOWN + RIGHT * 2,
			DOWN + LEFT * 2,
			UP * 2 + RIGHT,
			UP * 2 + LEFT,
			DOWN * 2 + RIGHT,
			DOWN * 2 + LEFT
		};
		const Color enemyPlayer = ~player;
		const BitboardSet& board = chessState.getBoard();

		for (const Position& direction : knightDirections)
		{
			const Position attackerPos = position + direction;
			if (isInBounds(attackerPos) && board.posIsOccupied(attackerPos, enemyPlayer, PieceType::KNIGHT))
			{
				return true;
			}
		}

		return false;
	}

	bool canBeCapturedBySlider(const ChessState& chessState, const Color player, const Position& position)
	{
		const Color enemyPlayer = ~player;
		const BitboardSet& board = chessState.getBoard();

		static std::vector<Position> diagonalDirections = {
			UP + LEFT,
			UP + RIGHT,
			DOWN + LEFT,
			DOWN + RIGHT
		};

		for (const Position& direction : diagonalDirections)
		{
			Position attackerPos = position + direction;
			while (isInBounds(attackerPos))
			{
				if (board.posIsOccupied(attackerPos))
				{
					if (board.posIsOccupied(attackerPos, enemyPlayer, PieceType::BISHOP)
						|| board.posIsOccupied(attackerPos, enemyPlayer, PieceType::QUEEN))
					{
						return true;
					}
					else
					{
						break;
					}
				}
				attackerPos += direction;
			}
		}

		static std::vector<Position> straightDirections = {
			UP,
			DOWN,
			LEFT,
			RIGHT
		};

		for (const Position& direction : straightDirections)
		{
			Position attackerPos = position + direction;
			while (isInBounds(attackerPos))
			{
				if (board.posIsOccupied(attackerPos))
				{
					if (board.posIsOccupied(attackerPos, enemyPlayer, PieceType::ROOK)
						|| board.posIsOccupied(attackerPos, enemyPlayer, PieceType::QUEEN))
					{
						return true;
					}
					else
					{
						break;
					}
				}
				attackerPos += direction;
			}
		}

		return false;
	}

	bool canBeCapturedByKing(const ChessState& chessState, const Color player, const Position& position)
	{
		static std::vector<Position> kingDirections = {
			UP,
			DOWN,
			LEFT,
			RIGHT,
			UP + LEFT,
			UP + RIGHT,
			DOWN + LEFT,
			DOWN + RIGHT
		};
		const Color enemyPlayer = ~player;
		const BitboardSet& board = chessState.getBoard();

		for (const Position& direction : kingDirections)
		{
			const Position attackerPos = position + direction;
			if (isInBounds(attackerPos) && board.posIsOccupied(attackerPos, enemyPlayer, PieceType::KING))
			{
				return true;
			}
		}

		return false;
	}

	bool canBeCaptured(const Color player, const ChessState& chessState, const Position& position)
	{
		if (canBeCapturedByPawn(chessState, player, position))
		{
			return true;
		}

		if (canBeCapturedByKnight(chessState, player, position))
		{
			return true;
		}

		if (canBeCapturedBySlider(chessState, player, position))
		{
			return true;
		}

		if (canBeCapturedByKing(chessState, player, position))
		{
			return true;
		}

		return false;
	}

	bool canCastle(const Color player, const ChessState& chessState, const bool kingSide)
	{
		bool canCastle;
		Position direction;
		const Position kingPosition = KING_START_POS[player];

		if (kingSide)
		{
			canCastle = chessState.canKingSideCastle(player);
			direction = RIGHT;
		}
		else
		{
			canCastle = chessState.canQueenSideCastle(player);
			direction = LEFT;
		}

		if (canCastle)
		{
			bool blocked = false;
			Position position = kingPosition + direction;

			// Set block to true if there are any pieces between king and rook
			while (!blocked && position.x < FILE_COUNT - 1 && position.x > 0)
			{
				blocked = chessState.getBoard().posIsOccupied(position);
				position += direction;
			}

			if (!blocked && !inCheck(player, chessState))
			{
				// Set safePath to false if any position the king would pass over could be attacked
				//      (this include the position the king lands on)
				bool safePath = !(canBeCaptured(player, chessState, position + direction)
					|| canBeCaptured(player, chessState, position + direction * 2));

				if (safePath)
				{
					return true;
				}
			}
		}

		return false;
	}

	std::vector<Move> getValidMoves(const ChessState& chessState, const Color player)
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

		const Position kingStartPosition = KING_START_POS[player];
		if (canCastle(player, chessState, true))
		{
			result.emplace_back(kingStartPosition, kingStartPosition + RIGHT * 2);
		}

		if (canCastle(player, chessState, false))
		{
			result.emplace_back(kingStartPosition, kingStartPosition + LEFT * 2);
		}

		std::vector<Move>::iterator move = result.begin();
		while (move != result.end())
		{
			ChessState gameCopy(chessState);

			gameCopy.update(player, *move, PieceType::QUEEN, false);

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
			case PieceType::PAWN:
			{
				const Position FORWARD = (player == Color::WHITE ? UP : DOWN);
				const int ORIGINAL_ROW = PAWN_START_ROW[player];

				if (destination == source + FORWARD)
				{
					return !board.posIsOccupied(destination);
				}
				else if (destination == source + FORWARD * 2)
				{
					return !board.posIsOccupied(destination)
						&& !board.posIsOccupied(source + FORWARD)
						&& source.y == ORIGINAL_ROW;
				}
				else if (destination == (source + FORWARD + LEFT) || destination == (source + FORWARD + RIGHT))
				{
					const std::deque<MoveHistoryNode>& moveHistory = chessState.getMoveHistory();
					if (board.posIsOccupied(destination, ~player))
					{
						return true;
					}
					// En Passant
					else if (!moveHistory.empty() && moveHistory.back().pieceType == PieceType::PAWN)
					{
						MoveHistoryNode prevMove = moveHistory.back();

						return prevMove.source == (destination + FORWARD) && prevMove.destination == (destination - FORWARD);
					}
				}

				return false;
			}
			case PieceType::KNIGHT:
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
			case PieceType::BISHOP:
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
					if (board.posIsOccupied(index))
					{
						return false;
					}
				}

				return true;
			}
			case PieceType::ROOK:
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
					if (board.posIsOccupied(index))
					{
						return false;
					}
				}

				return true;
			}
			case PieceType::QUEEN:
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
					if (board.posIsOccupied(index))
					{
						return false;
					}
				}

				return true;
			}
			case PieceType::KING:
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
		return canBeCaptured(player, chessState, kingPosition);
	}
}