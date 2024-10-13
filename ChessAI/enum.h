#pragma once

/**
* Enum used to identify a chess player or the color of a chess piece.
*/
enum Color
{
	WHITE = 0,
	BLACK = 1,
	NEUTRAL = 2
};
const int COLOR_COUNT = 2;

/**
* Enum used to identify the different types of chess pieces.
*/
enum PieceType
{
	PAWN = 0,
	KNIGHT = 1,
	BISHOP = 2,
	ROOK = 3,
	QUEEN = 4,
	KING = 5,
	NONE = 6
};
const int PIECE_TYPE_COUNT = 6;
