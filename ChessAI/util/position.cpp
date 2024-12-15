#pragma once

#include "position.h"

#include "../constants.h"

namespace util
{
	Position::Position(const int x, const int y) : x(x), y(y)
	{
	}

	Position& Position::operator=(const Position& rightOperand)
	{
		x = rightOperand.x;
		y = rightOperand.y;

		return *this;
	}

	Position& Position::operator+=(const Position& rightOperand)
	{
		x += rightOperand.x;
		y += rightOperand.y;

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
		x -= rightOperand.x;
		y -= rightOperand.y;

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
		x *= multiplier;
		y *= multiplier;

		return *this;
	}

	Position Position::operator*(const int multiplier) const
	{
		Position result(*this);

		result *= multiplier;

		return result;
	}

	Position Position::operator-() const
	{
		Position result(*this);

		return result *= -1;
	}

	bool Position::operator==(const Position& rightOperand) const
	{
		return x == rightOperand.x && y == rightOperand.y;
	}

	bool Position::operator!=(const Position& rightOperand) const
	{
		return !(*this == rightOperand);
	}

	std::size_t Position::PositionHasher::operator()(const Position& position) const
	{
		static const int X_SHIFT = static_cast<int>(log2(FILE_COUNT) + 1);
		return std::hash<int>()((position.x << X_SHIFT) + position.y);
	}

	std::ostream& operator<<(std::ostream& out, const Position& pos)
	{
		out << "(" << pos.x << ", " << pos.y << ")";

		return out;
	}

	// Directions
	const Position Position::UP(0, -1);
	const Position Position::DOWN(0, 1);
	const Position Position::LEFT(-1, 0);
	const Position Position::RIGHT(1, 0);
}
