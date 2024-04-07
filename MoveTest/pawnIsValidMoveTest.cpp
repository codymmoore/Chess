#include "pch.h"
#include "../ChessAI/move.h"

class PawnTest : public testing::Test {
protected:
	void SetUp() override {
		chessState.clear();
	}

	ChessState chessState;
};
TEST_F(PawnTest, moveForward_valid) {
	const Position source = Position(7, 7);
	const Position destination = Position(7, 6);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/8/8/8/7P w - - 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, moveForward_invalidOutOfBounds) {
	const Position source = Position(0, 7);
	const Position destination = Position(0, 8);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/8/8/8/p7 b - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, moveForward_invalidWrongDirection) {
	const Position source = Position(7, 6);
	const Position destination = Position(7, 7);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/8/8/7P/8 w - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, moveForward_invalidOccupied) {
	const Position source = Position(0, 0);
	const Position destination = Position(0, 1);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("p7/p7/8/8/8/8/8/8 b - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, moveForwardTwo_valid) {
	const Position source = Position(0, 1);
	const Position destination = Position(0, 3);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/p7/8/8/8/8/8/8 b - - 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, moveForwardTwo_invalidNotStartRow) {
	const Position source = Position(7, 5);
	const Position destination = Position(7, 3);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/8/7P/8/8 w - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, moveForwardTwo_invalidOccupiedDestination) {
	const Position source = Position(0, 1);
	const Position destination = Position(0, 3);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/p7/8/r7/8/8/8/8 b - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, moveForwardTwo_invalidOccupiedPath) {
	const Position source = Position(7, 6);
	const Position destination = Position(7, 4);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/8/7n/7P/8 w - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, attack_validLeft) {
	const Position source = Position(0, 2);
	const Position destination = Position(1, 3);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/p7/1N6/8/8/8/8 b - - 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, attack_validRight) {
	const Position source = Position(0, 5);
	const Position destination = Position(1, 4);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/1r6/P7/8/8 w - - 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, attack_invalidAlly) {
	const Position source = Position(0, 2);
	const Position destination = Position(1, 3);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/p7/1r6/8/8/8/8 b - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, enPassant_validLeft) {
	const Position source = Position(7, 3);
	const Position destination = Position(6, 2);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/6pP/8/8/8/8 w - g6 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, enPassant_validRight) {
	const Position source = Position(7, 4);
	const Position destination = Position(6, 5);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/6Pp/8/8/8 b - g3 0 1");
	EXPECT_TRUE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}

TEST_F(PawnTest, enPassant_invalidNoMoveHistory) {
	const Position source = Position(7, 3);
	const Position destination = Position(6, 2);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/6pP/8/8/8/8 w - - 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::WHITE, piece, destination, chessState));
}

TEST_F(PawnTest, enPassant_invalidNotPawn) {
	const Position source = Position(7, 4);
	const Position destination = Position(6, 5);
	const PieceNode piece = PieceNode(source, PieceType::PAWN);
	chessState.setState("8/8/8/8/6Np/8/8/8 b - g3 0 1");
	EXPECT_FALSE(Move::isValidMove(Color::BLACK, piece, destination, chessState));
}