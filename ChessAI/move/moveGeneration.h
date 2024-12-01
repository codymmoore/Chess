#pragma once

#include <vector>

#include "../enum.h"
#include "../util/position.h"

class ChessState;

namespace move
{
	struct Move
	{
		util::Position source;
		util::Position destination;
		PieceType promotion;

		Move() = default;
		Move(const util::Position& source, const util::Position& destination);
		Move(const util::Position& source, const util::Position& destination, const PieceType promotion);
		Move(const Move& source);

		Move& operator=(const Move& rightOperand);
		bool operator==(const Move& rightOperand) const;
		bool operator!=(const Move& rightOperand) const;
	};

	std::vector<Move> generatePawnMoves(const ChessState& game, const Color player);
}
