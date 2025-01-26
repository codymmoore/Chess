#include "pch.h"

using namespace util;
using namespace move;

namespace isValidMoveTest
{
	class IsValidMoveTest : public testing::Test
	{
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

	TEST_F(IsValidMoveTest, invalidMove_outOfBoundsLeft)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(1, 4);
		const Position DESTINATION = Position(-1, 4);
		chessState = std::make_unique<ChessState>("8/8/8/8/1Q6/8/8/8 w - - 0 1");
		EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
	}

	TEST_F(IsValidMoveTest, invalidMove_outOfBoundsRight)
	{
		const Color COLOR = Color::BLACK;
		const Position SOURCE = Position(7, 3);
		const Position DESTINATION = Position(8, 3);
		chessState = std::make_unique<ChessState>("8/8/8/7r/8/8/8/8 b - - 0 1");
		EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
	}

	TEST_F(IsValidMoveTest, invalidMove_outOfBoundsTop)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(2, 0);
		const Position DESTINATION = Position(2, -1);
		chessState = std::make_unique<ChessState>("2P5/8/8/8/8/8/8/8 w - - 0 1");
		EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
	}

	TEST_F(IsValidMoveTest, invalidMove_outOfBoundsBottom)
	{
		const Color COLOR = Color::BLACK;
		const Position SOURCE = Position(4, 7);
		const Position DESTINATION = Position(4, 8);
		chessState = std::make_unique<ChessState>("8/8/8/8/8/8/8/4r3 b - - 0 1");
		EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
	}

	namespace pawn
	{
		TEST_F(IsValidMoveTest, pawnMoveForward_valid)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 7);
			const Position DESTINATION = SOURCE + forward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/8/8/8/7P w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForward_invalidWrongDirection)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(7, 6);
			const Position DESTINATION = SOURCE + backward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/8/8/7P/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForward_invalidOccupied)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(0, 0);
			const Position DESTINATION = SOURCE + forward(COLOR);
			chessState = std::make_unique<ChessState>("p7/p7/8/8/8/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForwardTwo_valid)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(0, 1);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2;
			chessState = std::make_unique<ChessState>("8/p7/8/8/8/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForwardTwo_invalidNotStartRow)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 5);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2;
			chessState = std::make_unique<ChessState>("8/8/8/8/8/7P/8/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForwardTwo_invalidOccupiedDestination)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(0, 1);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2;
			chessState = std::make_unique<ChessState>("8/p7/8/r7/8/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnMoveForwardTwo_invalidOccupiedPath)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 6);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2;
			chessState = std::make_unique<ChessState>("8/8/8/8/8/7n/7P/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnCapture_validLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(0, 2);
			const Position DESTINATION = SOURCE + forward(COLOR) + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/p7/1N6/8/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnCapture_validRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(0, 5);
			const Position DESTINATION = SOURCE + forward(COLOR) + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/1r6/P7/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnCapture_invalidAlly)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(0, 2);
			const Position DESTINATION = SOURCE + forward(COLOR) + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/p7/1r6/8/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnEnPassant_validLeft)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 3);
			const Position DESTINATION = SOURCE + forward(COLOR) + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/6pP/8/8/8/8 w - g6 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnEnPassant_validRight)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(7, 4);
			const Position DESTINATION = SOURCE + forward(COLOR) + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/6Pp/8/8/8 b - g3 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnEnPassant_invalidNoMoveHistory)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 3);
			const Position DESTINATION = SOURCE + forward(COLOR) + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/6pP/8/8/8/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, pawnEnPassant_invalidNotPawn)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(7, 4);
			const Position DESTINATION = Position(6, 3);
			chessState = std::make_unique<ChessState>("8/8/8/8/6Np/8/8/8 b - g3 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}
	}

	namespace knight
	{
		TEST_F(IsValidMoveTest, knightMove_validForwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2 + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validForwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + forward(COLOR) * 2 + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4n3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validRightForward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + right(COLOR) * 2 + forward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validLeftForward)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + left(COLOR) * 2 + forward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4n3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validBackwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + backward(COLOR) * 2 + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validBackwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + backward(COLOR) * 2 + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4n3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validRightBackward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + right(COLOR) * 2 + backward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_validLeftBackward)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + left(COLOR) * 2 + backward(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4n3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_invalidOccupied)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + backward(COLOR) * 2 + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/5P2/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightMove_invalidDestination)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4n3/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, knightCapture_valid)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 6);
			chessState = std::make_unique<ChessState>("8/8/8/8/4N3/8/5p2/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}
	}

	namespace bishop
	{
		TEST_F(IsValidMoveTest, bishopMove_validForwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + forward(COLOR) + right(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4B3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopMove_validForwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + (forward(COLOR) + left(COLOR)) * 2;
			chessState = std::make_unique<ChessState>("8/8/8/8/4b3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopMove_validBackwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + (backward(COLOR) + right(COLOR)) * 2;
			chessState = std::make_unique<ChessState>("8/8/8/8/4B3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopMove_validBackwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + backward(COLOR) + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4b3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopCapture_valid)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + (forward(COLOR) + right(COLOR)) * 2;
			chessState = std::make_unique<ChessState>("8/8/8/8/4B3/8/8/6p1 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopMove_invalidDestination)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + left(COLOR);
			chessState = std::make_unique<ChessState>("8/8/8/8/4b3/8/8/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, bishopMove_invalidBlockedPath)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = SOURCE + (backward(COLOR) + right(COLOR)) * 3;
			chessState = std::make_unique<ChessState>("8/8/8/8/4B3/8/6p1/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}
	}

	namespace rook
	{
		TEST_F(IsValidMoveTest, rookMove_validForward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(4, 3);
			chessState = std::make_unique<ChessState>("8/8/8/8/4R3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookMove_validRight)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(6, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4r3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookMove_validBackward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(4, 6);
			chessState = std::make_unique<ChessState>("8/8/8/8/4R3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookMove_validLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4r3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookCapture_valid)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(4, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4R3/4r3/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookMove_invalidBlockedPath)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(0, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/1P2r3/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, rookMove_invalidDestination)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4R3/4r3/8/8 w - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}
	}

	namespace king
	{
		TEST_F(IsValidMoveTest, kingMove_validForward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(4, 3);
			chessState = std::make_unique<ChessState>("8/8/8/8/4K3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validRight)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4k3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validBackward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(4, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4K3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4k3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validForwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 3);
			chessState = std::make_unique<ChessState>("8/8/8/8/4K3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validForwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 3);
			chessState = std::make_unique<ChessState>("8/8/8/8/4k3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validBackwardRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4K3/8/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_validBackwardLeft)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4k3/8/8/8 b - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingCastle_validKingside)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const Position DESTINATION = Position(6, 7);
			chessState = std::make_unique<ChessState>("4k3/8/8/8/8/8/8/4K2R w K - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingCastle_validQueenside)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 0);
			const Position DESTINATION = Position(2, 0);
			chessState = std::make_unique<ChessState>("r3k3/8/8/8/8/8/8/4K3 b q - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingCapture_valid)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(5, 5);
			chessState = std::make_unique<ChessState>("8/8/8/8/4K3/5r2/8/8 w - - 0 1");
			EXPECT_TRUE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_invalidDestination)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(2, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/4k3/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}

		TEST_F(IsValidMoveTest, kingMove_invalidBlocked)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 4);
			const Position DESTINATION = Position(3, 4);
			chessState = std::make_unique<ChessState>("8/8/8/8/3rk3/8/8/8 b - - 0 1");
			EXPECT_FALSE(isValidMove(COLOR, SOURCE, DESTINATION, *chessState));
		}
	}
}