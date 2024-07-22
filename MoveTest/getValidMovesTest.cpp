#include "pch.h"
#include <vector>
#include "../ChessAI/move.h"

using namespace testing;

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
		TEST_F(GetValidMovesTest, pawnMoveForward_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/8/3P4/RNBQKBNR w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR)));
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) * 2));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(3, 1);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("rnbqkbnr/3p4/8/8/8/8/8/4K3 b - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR)));
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) * 2));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_outOfBoundsAbove)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("3Pk3/8/8/8/8/8/8/4K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR))));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_outOfBoundsBelow)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(3, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/8/8/4Kp2 b - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR))));
		}

		TEST_F(GetValidMovesTest, pawnCapture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/2p1p3/3P4/RNBQKBNR w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(SOURCE + forward(COLOR) + RIGHT, ~COLOR));
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + RIGHT));
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(SOURCE + forward(COLOR) + LEFT, ~COLOR));
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + LEFT));
		}

		TEST_F(GetValidMovesTest, pawnEnPassant_right)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/3Pp3/8/8/8/RNBQKBNR w - e6 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + RIGHT));
		}

		TEST_F(GetValidMovesTest, pawnEnPassant_left)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/2pP4/8/8/8/RNBQKBNR w - c6 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + LEFT));
		}

		TEST_F(GetValidMovesTest, pawnEnPassant_noMoveHistory)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/3Pp3/8/8/8/RNBQKBNR w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR) + RIGHT)));
		}

		TEST_F(GetValidMovesTest, pawn_blocked1)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/3p4/3P4/RNBQKBNR w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(validMoves.empty());
		}

		TEST_F(GetValidMovesTest, pawn_blocked2)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/3p4/8/3P4/RNBQKBNR w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR) * 2)));
		}
	}

	namespace knight
	{
		TEST_F(GetValidMovesTest, knight_moves)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/8/3N4/8/8/8/4K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + UP * 2 + RIGHT));
			EXPECT_THAT(validMoves, Contains(SOURCE + UP * 2 + LEFT));
			EXPECT_THAT(validMoves, Contains(SOURCE + DOWN * 2 + RIGHT));
			EXPECT_THAT(validMoves, Contains(SOURCE + DOWN * 2 + LEFT));
			EXPECT_THAT(validMoves, Contains(SOURCE + RIGHT * 2 + UP));
			EXPECT_THAT(validMoves, Contains(SOURCE + LEFT * 2 + UP));
			EXPECT_THAT(validMoves, Contains(SOURCE + RIGHT * 2 + DOWN));
			EXPECT_THAT(validMoves, Contains(SOURCE + LEFT * 2 + DOWN));
		}

		TEST_F(GetValidMovesTest, knight_outOfBoundsUpAndRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k2N/8/8/8/8/8/8/4K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + DOWN * 2 + LEFT));
			EXPECT_THAT(validMoves, Contains(SOURCE + LEFT * 2 + DOWN));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + UP * 2 + RIGHT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + UP * 2 + LEFT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + DOWN * 2 + RIGHT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + RIGHT * 2 + UP)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + LEFT * 2 + UP)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + RIGHT * 2 + DOWN)));
		}

		TEST_F(GetValidMovesTest, knight_outBoundsLeftAndDown)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(0, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/8/8/8/8/8/N3K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + UP * 2 + RIGHT));
			EXPECT_THAT(validMoves, Contains(SOURCE + RIGHT * 2 + UP));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + UP * 2 + LEFT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + DOWN * 2 + RIGHT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + DOWN * 2 + LEFT)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + LEFT * 2 + UP)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + RIGHT * 2 + DOWN)));
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + LEFT * 2 + DOWN)));
		}

		TEST_F(GetValidMovesTest, knight_capture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const Position DESTINATION = SOURCE + RIGHT * 2 + UP;
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/5p2/3N4/8/8/8/4K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, ~COLOR));
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, knight_blocked)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const Position DESTINATION = SOURCE + RIGHT * 2 + UP;
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/5P2/3N4/8/8/8/4K3 w - - 0 1");
			std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, COLOR));
			EXPECT_THAT(validMoves, Not(Contains(DESTINATION)));
		}
	}

	//namespace linear
	//{
	//	TEST_F(GetValidMovesTest, )
	//	{

	//	}
	//}

	//TEST_F(GetValidMoveTest, invalidMove_outOfBoundsLeftUh) {
	//	const Color COLOR = Color::WHITE;
	//	const Position SOURCE = Position(1, 4);
	//	const Position DESTINATION = Position(-1, 4);
	//	const PieceNode PIECE = PieceNode(SOURCE, PieceType::QUEEN);
	//	chessState.setState("8/8/8/8/1Q6/8/8/8 w - - 0 1");
	//	EXPECT_FALSE(Move::isValidMove(COLOR, PIECE, DESTINATION, chessState));
	//}
}