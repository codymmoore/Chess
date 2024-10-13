#pragma once

#include "enum.h"

const int RANK_COUNT = 8; // number of rows in a chess board
const int FILE_COUNT = 8; // number of columns in a chess board
constexpr char PIECE_SYMBOLS[COLOR_COUNT][PIECE_TYPE_COUNT] = {
	{'P', 'N', 'B', 'R', 'Q', 'K'}, // White symbols
	{'p', 'n', 'b', 'r', 'q', 'k'} // Black symbols
};
