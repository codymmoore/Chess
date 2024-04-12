//
// pch.cpp
//

#include "pch.h"

Position forward(const Color color)
{
	return color == Color::WHITE ? UP : DOWN;
}

Position backward(const Color color)
{
	return ~forward(color);
}

Position right(const Color color)
{
	return color == Color::WHITE ? RIGHT : LEFT;
}

Position left(const Color color)
{
	return ~right(color);
}