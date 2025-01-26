#include "pch.h"

using namespace testing;
using namespace move;

namespace inCheckTest
{
	class InCheckTest : public testing::Test {
	protected:
		void TearDown() override
		{
			if (chessState)
			{
				chessState.release();
			}
		}

		std::unique_ptr<ChessState> chessState;
	};

	TEST_F(InCheckTest, true_white)
	{
		const Color COLOR = Color::WHITE;
		chessState = std::make_unique<ChessState>("4k3/4r3/8/8/8/8/8/4K3 w - - 0 1");
		EXPECT_TRUE(inCheck(COLOR, *chessState));
	}

	TEST_F(InCheckTest, false_white)
	{
		const Color COLOR = Color::WHITE;
		chessState = std::make_unique<ChessState>("4k3/3r4/8/8/8/8/8/4K3 w - - 0 1");
		EXPECT_FALSE(inCheck(COLOR, *chessState));
	}

	TEST_F(InCheckTest, true_black)
	{
		const Color COLOR = Color::BLACK;
		chessState = std::make_unique<ChessState>("4k3/8/8/8/8/8/4R3/4K3 b - - 0 1");
		EXPECT_TRUE(inCheck(COLOR, *chessState));
	}

	TEST_F(InCheckTest, false_black)
	{
		const Color COLOR = Color::BLACK;
		chessState = std::make_unique<ChessState>("4k3/8/8/8/8/8/3R4/4K3 b - - 0 1");
		EXPECT_FALSE(inCheck(COLOR, *chessState));
	}

	TEST_F(InCheckTest, arstnaiest)
	{
		const Color COLOR = Color::BLACK;
		chessState = std::make_unique<ChessState>("r2q1b1r/8/pp3ppp/1k1p1n2/PP6/3P2P1/2P2P1P/RN2KBNR w KQ - 1 19");
		chessState->print();
		EXPECT_TRUE(inCheck(COLOR, *chessState));
	}
}
