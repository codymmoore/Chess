#include "pch.h"
#include <vector>
#include "../ChessAI/move.h"

namespace getValidMovesTest
{
	class GetValidMovesTest : public testing::Test {
	protected:
		void TearDown() override {
			chessState.clear();
		}

		ChessState chessState;
	};



	namespace pawn
	{
		TEST_F(GetValidMovesTest, getValidMoves_pawn)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			//chessState.setState("4k3/8/8/8/8/4n3/2pP4/3K4 w - c4 0 1");
			chessState.setState("4k3/8/8/8/8/4n3/2pP4/RNBQKBNR w - c3 0 1");
			std::vector<Position> tmp = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(true);
		}

		TEST_F(GetValidMovesTest, pawnMoveForward)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			//chessState.setState("4k3/8/8/8/8/4n3/2pP4/3K4 w - c4 0 1");
			chessState.setState("4k3/8/8/8/8/8/3P4/RNBQKBNR w - c3 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			//EXPECT_THAT();
			/*	EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR)));
				EXPECT_TRUE(true);*/
		}
	}

	//TEST_F(GetValidMoveTest, invalidMove_outOfBoundsLeftUh) {
	//	const Color COLOR = Color::WHITE;
	//	const Position SOURCE = Position(1, 4);
	//	const Position DESTINATION = Position(-1, 4);
	//	const PieceNode PIECE = PieceNode(SOURCE, PieceType::QUEEN);
	//	chessState.setState("8/8/8/8/1Q6/8/8/8 w - - 0 1");
	//	EXPECT_FALSE(Move::isValidMove(COLOR, PIECE, DESTINATION, chessState));
	//}
}