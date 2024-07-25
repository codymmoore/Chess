#include "pch.h"
#include "../ChessAI/move.h"

using namespace testing;

namespace inCheckTest
{
	class InCheckTest : public testing::Test {
	protected:
		void TearDown() override {
			chessState.clear();
		}

		ChessState chessState;
	};

	TEST_F(InCheckTest, true_white)
	{
		const Color COLOR = Color::WHITE;
		chessState.setState("4k3/4r3/8/8/8/8/8/4K3 w - - 0 1");
		EXPECT_TRUE(Move::inCheck(COLOR, chessState));
	}

	TEST_F(InCheckTest, false_white)
	{
		const Color COLOR = Color::WHITE;
		chessState.setState("4k3/3r4/8/8/8/8/8/4K3 w - - 0 1");
		EXPECT_FALSE(Move::inCheck(COLOR, chessState));
	}

	TEST_F(InCheckTest, true_black)
	{
		const Color COLOR = Color::BLACK;
		chessState.setState("4k3/8/8/8/8/8/4R3/4K3 b - - 0 1");
		EXPECT_TRUE(Move::inCheck(COLOR, chessState));
	}

	TEST_F(InCheckTest, false_black)
	{
		const Color COLOR = Color::BLACK;
		chessState.setState("4k3/8/8/8/8/8/3R4/4K3 b - - 0 1");
		EXPECT_FALSE(Move::inCheck(COLOR, chessState));
	}
}
