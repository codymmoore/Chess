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

	TEST_F(GetValidMovesTest, removeMovesThatResultInCheck_white)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(4, 7);
		const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
		chessState.setState("4k3/8/8/8/8/8/6p1/4K3 w - - 0 1");
		const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
		EXPECT_THAT(validMoves, Not(Contains(Position(5, 7))));
	}

	TEST_F(GetValidMovesTest, removeMovesThatResultInCheck_black)
	{
		const Color COLOR = Color::BLACK;
		const Position SOURCE = Position(4, 0);
		const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
		chessState.setState("4k3/6P1/8/8/8/8/8/4K3 b - - 0 1");
		const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
		EXPECT_THAT(validMoves, Not(Contains(Position(5, 0))));
	}

	namespace pawn
	{
		TEST_F(GetValidMovesTest, pawnMoveForward_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/8/3P4/RNBQKBNR w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				SOURCE + forward(COLOR),
				SOURCE + forward(COLOR) * 2
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(3, 1);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("rnbqkbnr/3p4/8/8/8/8/8/4K3 b - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				SOURCE + forward(COLOR),
				SOURCE + forward(COLOR) * 2
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_outOfBoundsAbove)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("3Pk3/8/8/8/8/8/8/4K3 w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR))));
		}

		TEST_F(GetValidMovesTest, pawnMoveForward_outOfBoundsBelow)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(3, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/8/8/4Kp2 b - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR))));
		}

		TEST_F(GetValidMovesTest, pawnCapture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/2p1p3/3P4/RNBQKBNR w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
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
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + RIGHT));
		}

		TEST_F(GetValidMovesTest, pawnEnPassant_left)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/2pP4/8/8/8/RNBQKBNR w - c6 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(SOURCE + forward(COLOR) + LEFT));
		}

		TEST_F(GetValidMovesTest, pawnEnPassant_noMoveHistory)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/3Pp3/8/8/8/RNBQKBNR w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Not(Contains(SOURCE + forward(COLOR) + RIGHT)));
		}

		TEST_F(GetValidMovesTest, pawn_blocked1)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/8/3p4/3P4/RNBQKBNR w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(validMoves.empty());
		}

		TEST_F(GetValidMovesTest, pawn_blocked2)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 6);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::PAWN);
			chessState.setState("4k3/8/8/8/3p4/8/3P4/RNBQKBNR w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
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
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				SOURCE + UP * 2 + RIGHT,
				SOURCE + UP * 2 + LEFT,
				SOURCE + DOWN * 2 + RIGHT,
				SOURCE + DOWN * 2 + LEFT,
				SOURCE + RIGHT * 2 + UP,
				SOURCE + LEFT * 2 + UP,
				SOURCE + RIGHT * 2 + DOWN,
				SOURCE + LEFT * 2 + DOWN
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, knight_outOfBoundsUpAndRight)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k2N/8/8/8/8/8/8/4K3 w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				SOURCE + DOWN * 2 + LEFT,
				SOURCE + LEFT * 2 + DOWN
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, knight_outBoundsLeftAndDown)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(0, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/8/8/8/8/8/N3K3 w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				SOURCE + UP * 2 + RIGHT,
				SOURCE + RIGHT * 2 + UP
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, knight_capture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const Position DESTINATION = SOURCE + RIGHT * 2 + UP;
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KNIGHT);
			chessState.setState("4k3/8/5p2/3N4/8/8/8/4K3 w - - 0 1");
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
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
			const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, COLOR));
			EXPECT_THAT(validMoves, Not(Contains(DESTINATION)));
		}
	}

	namespace linear
	{
		TEST_F(GetValidMovesTest, rook)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::ROOK);
			chessState.setState("4k3/8/8/3R4/8/8/8/4K3 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				Position(4, 3),
				Position(5, 3),
				Position(6, 3),
				Position(7, 3),
				Position(3, 4),
				Position(3, 5),
				Position(3, 6),
				Position(3, 7),
				Position(2, 3),
				Position(1, 3),
				Position(0, 3),
				Position(3, 2),
				Position(3, 1),
				Position(3, 0)
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, bishop)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::BISHOP);
			chessState.setState("4k3/8/8/3B4/8/8/8/4K3 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				Position(4, 4),
				Position(5, 5),
				Position(6, 6),
				Position(7, 7),
				Position(4, 2),
				Position(5, 1),
				Position(6, 0),
				Position(2, 4),
				Position(1, 5),
				Position(0, 6),
				Position(2, 2),
				Position(1, 1),
				Position(0, 0)
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, queen)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::QUEEN);
			chessState.setState("4k3/8/8/3Q4/8/8/8/4K3 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				Position(4, 4),
				Position(5, 5),
				Position(6, 6),
				Position(7, 7),
				Position(4, 2),
				Position(5, 1),
				Position(6, 0),
				Position(2, 4),
				Position(1, 5),
				Position(0, 6),
				Position(2, 2),
				Position(1, 1),
				Position(0, 0),
				Position(4, 3),
				Position(5, 3),
				Position(6, 3),
				Position(7, 3),
				Position(3, 4),
				Position(3, 5),
				Position(3, 6),
				Position(3, 7),
				Position(2, 3),
				Position(1, 3),
				Position(0, 3),
				Position(3, 2),
				Position(3, 1),
				Position(3, 0)
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, linear_capture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::QUEEN);
			const Position DESTINATION = Position(5, 3);
			chessState.setState("4k3/8/8/3Q1p2/8/8/8/4K3 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, ~COLOR));
			EXPECT_THAT(validMoves, Contains(Position(4, 3)));
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, linear_blocked)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::QUEEN);
			chessState.setState("4k3/8/8/3QP3/8/8/8/4K3 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(Position(4, 3), COLOR));
			EXPECT_THAT(validMoves, Not(Contains(Position(4, 3))));
			EXPECT_THAT(validMoves, Not(Contains(Position(5, 3))));
		}
	}

	namespace king
	{
		TEST_F(GetValidMovesTest, king_all)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			chessState.setState("4k3/8/8/3K4/8/8/8/8 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			const std::vector<Position> expectedMoves = {
				Position(2, 2),
				Position(3, 2),
				Position(4, 2),
				Position(2, 3),
				Position(4, 3),
				Position(2, 4),
				Position(3, 4),
				Position(4, 4)
			};
			EXPECT_THAT(expectedMoves, ::testing::UnorderedElementsAreArray(validMoves));
		}

		TEST_F(GetValidMovesTest, king_capture)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = Position(4, 3);
			chessState.setState("4k3/8/8/3Kp3/8/8/8/8 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, ~COLOR));
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, king_blocked)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = Position(4, 3);
			chessState.setState("4k3/8/8/3KP3/8/8/8/8 w - - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_TRUE(chessState.m_board.posIsOccupiedByColor(DESTINATION, COLOR));
			EXPECT_THAT(validMoves, Not(Contains(DESTINATION)));
		}

		TEST_F(GetValidMovesTest, king_castle_queenSide_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = SOURCE + LEFT * 2;
			chessState.setState("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, king_castle_queenSide_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = SOURCE + LEFT * 2;
			chessState.setState("r3k3/8/8/8/8/8/8/4K3 w q - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, king_castle_kingSide_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = SOURCE + RIGHT * 2;
			chessState.setState("4k3/8/8/8/8/8/8/4K2R w K - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}

		TEST_F(GetValidMovesTest, king_castle_kingSide_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 0);
			const PieceNode PIECE = PieceNode(SOURCE, PieceType::KING);
			const Position DESTINATION = SOURCE + RIGHT * 2;
			chessState.setState("4k2r/8/8/8/8/8/8/4K3 w k - 0 1");
			const const std::vector<Position> validMoves = Move::getValidMoves(COLOR, PIECE, chessState);
			EXPECT_THAT(validMoves, Contains(DESTINATION));
		}
	}
}