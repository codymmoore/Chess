//
// pch.cpp
//

#include "pch.h"

#include "../ChessAI/constants.h"

util::Position forward(const Color color)
{
	return color == Color::WHITE ? util::Position::UP : util::Position::DOWN;
}

util::Position backward(const Color color)
{
	return -forward(color);
}

util::Position right(const Color color)
{
	return color == Color::WHITE ? util::Position::RIGHT : util::Position::LEFT;
}

util::Position left(const Color color)
{
	return -right(color);
}
