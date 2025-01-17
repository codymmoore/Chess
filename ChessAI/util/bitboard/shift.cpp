#include "shift.h"

namespace util
{
	namespace bitboard
	{
		Shift::Shift() :
			horizontal(0),
			vertical(0)
		{
		}

		Shift::Shift(const int horizontal, const int vertical) :
			horizontal(horizontal),
			vertical(vertical)
		{
		}

		Shift& Shift::operator+=(const Shift& rightOperand)
		{
			horizontal += rightOperand.horizontal;
			vertical += rightOperand.vertical;
			return *this;
		}

		Shift Shift::operator+(const Shift& rightOperand) const
		{
			Shift result(*this);
			result += rightOperand;
			return result;
		}

		Shift& Shift::operator-=(const Shift& rightOperand)
		{
			horizontal -= rightOperand.horizontal;
			vertical -= rightOperand.vertical;
			return *this;
		}

		Shift Shift::operator-(const Shift& rightOperand) const
		{
			Shift result(*this);
			result += rightOperand;
			return result;
		}

		Shift Shift::operator-() const
		{
			Shift result(-horizontal, -vertical);
			return result;
		}

		Shift& Shift::operator*=(const int multiplier)
		{
			horizontal *= multiplier;
			vertical *= multiplier;
			return *this;
		}

		Shift Shift::operator*(const int multiplier) const
		{
			Shift result(*this);
			result *= multiplier;
			return result;
		}

		Shift up(const int magnitude)
		{
			return Shift(0, -magnitude);
		}

		Shift down(const int magnitude)
		{
			return Shift(0, magnitude);
		}

		Shift left(const int magnitude)
		{
			return Shift(-magnitude, 0);
		}

		Shift right(const int magnitude)
		{
			return Shift(magnitude, 0);
		}
	}
}
