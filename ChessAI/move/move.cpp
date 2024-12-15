#include "move.h"

using util::Position;

namespace move
{
	Move::Move(const Position& source, const Position& destination) :
		source(source),
		destination(destination),
		promotion(PieceType::NONE)
	{
	}

	Move::Move(const Position& source, const Position& destination, const PieceType promotion) :
		source(source),
		destination(destination),
		promotion(promotion)
	{
	}

	Move::Move(const Move& source) :
		source(source.source),
		destination(source.destination),
		promotion(source.promotion)
	{
	}

	Move& Move::operator=(const Move& rightOperand)
	{
		source = rightOperand.source;
		destination = rightOperand.destination;
		promotion = rightOperand.promotion;

		return *this;
	}

	bool Move::operator==(const Move& rightOperand) const
	{
		return source == rightOperand.source
			&& destination == rightOperand.destination
			&& promotion == rightOperand.promotion;
	}

	bool Move::operator!=(const Move& rightOperand) const
	{
		return !(*this == rightOperand);
	}

	std::size_t Move::MoveHasher::operator()(const Move& move) const
	{
		static const int OFFSET = 3;
		static const Position::PositionHasher positionHasher;

		std::size_t hash = positionHasher(move.source) << (OFFSET * 3);
		hash |= positionHasher(move.destination) << OFFSET;
		hash |= move.promotion;

		return hash;
	}
}