//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "../ChessAI/move/move.h"
#include "../ChessAI/enum.h"
#include "../ChessAI/util/position.h"
#include "../ChessAI/util/utility.h"
#include "../ChessAI/move/moveGeneration.h"

util::Position forward(const Color color);

util::Position backward(const Color color);

util::Position right(const Color color);

util::Position left(const Color color);