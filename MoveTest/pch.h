//
// pch.h
//

#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock-matchers.h"
#include "../ChessAI/utility.h"

Position forward(const Color color);

Position backward(const Color color);

Position right(const Color color);

Position left(const Color color);