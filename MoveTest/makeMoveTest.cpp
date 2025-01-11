#include "pch.h"

#define UP Position::UP
#define DOWN Position::DOWN
#define LEFT Position::LEFT
#define RIGHT Position::RIGHT

using namespace testing;
using namespace util;
using namespace move;

namespace makeMoveTest
{
	class MakeMoveTest : public testing::Test {
	protected:
		void TearDown() override {
			chessState.clear();
		}

		ChessState chessState;
	};

	TEST_F(MakeMoveTest, move_updateBoard)
	{
		const Color COLOR = Color::WHITE;
		const PieceType PIECE_TYPE = PieceType::KNIGHT;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4N3/4K3 w - - 0 1");

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_FALSE(chessState.m_board.posIsOccupied(SOURCE));
		EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PIECE_TYPE));
	}

	TEST_F(MakeMoveTest, move_updateTurn)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4N3/4K3 w - - 0 1");
		const Color CURRENT_TURN = chessState.getNextTurn();

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_EQ(~CURRENT_TURN, chessState.getNextTurn());
	}

	TEST_F(MakeMoveTest, move_updateHalfTurns)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4NN3/4K3 w - - 0 1");
		const int CURRENT_HALF_TURNS = chessState.m_numHalfTurns;

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_EQ(CURRENT_HALF_TURNS + 1, chessState.m_numHalfTurns);
	}

	TEST_F(MakeMoveTest, move_updateFullTurns)
	{
		const Color COLOR = Color::BLACK;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4n3/4K3 b - - 0 1");
		const int CURRENT_FULL_TURNS = chessState.m_numFullTurns;

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_EQ(CURRENT_FULL_TURNS + 1, chessState.m_numFullTurns);
	}

	TEST_F(MakeMoveTest, move_updateMoveHistory)
	{
		const Color COLOR = Color::WHITE;
		const PieceType PIECE_TYPE = PieceType::KNIGHT;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4NN3/4K3 w - - 0 1");
		chessState.m_moveHistory.resize(MAX_MOVE_HISTORY_SIZE, MoveHistoryNode());

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_EQ(MAX_MOVE_HISTORY_SIZE, chessState.m_moveHistory.size());
		EXPECT_EQ(MoveHistoryNode(SOURCE, DESTINATION, COLOR, PIECE_TYPE), chessState.m_moveHistory.back());
	}

	TEST_F(MakeMoveTest, move_checkForTie_true)
	{
		const Color COLOR = Color::WHITE;
		const PieceType PIECE_TYPE = PieceType::KNIGHT;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4NN3/4K3 w - - 8 8");

		chessState.m_moveHistory.resize(MAX_MOVE_HISTORY_SIZE - 1);
		for (int i = 0; i < (MAX_MOVE_HISTORY_SIZE / 2) - 1; i++)
		{
			const MoveHistoryNode moveHistoryNode = MoveHistoryNode(SOURCE, Position(i, 0), COLOR, PIECE_TYPE);
			chessState.m_moveHistory[i] = moveHistoryNode;
			chessState.m_moveHistory[i + (MAX_MOVE_HISTORY_SIZE / 2)] = moveHistoryNode;
		}
		chessState.m_moveHistory[(MAX_MOVE_HISTORY_SIZE / 2) - 1] = MoveHistoryNode(SOURCE, DESTINATION, COLOR, PIECE_TYPE);

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_EQ(Color::NEUTRAL, chessState.getNextTurn());
	}

	TEST_F(MakeMoveTest, move_checkForTie_false)
	{
		const Color COLOR = Color::WHITE;
		const PieceType PIECE_TYPE = PieceType::KNIGHT;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/4NN3/4K3 w - - 8 8");

		chessState.m_moveHistory.resize(MAX_MOVE_HISTORY_SIZE - 1);
		for (int i = 0; i < (MAX_MOVE_HISTORY_SIZE / 2) - 1; i++)
		{
			const MoveHistoryNode moveHistoryNode = MoveHistoryNode(SOURCE, Position(i, 0), COLOR, PIECE_TYPE);
			chessState.m_moveHistory[i] = moveHistoryNode;
			chessState.m_moveHistory[i + (MAX_MOVE_HISTORY_SIZE / 2)] = moveHistoryNode;
		}
		chessState.m_moveHistory[(MAX_MOVE_HISTORY_SIZE / 2) - 1] = MoveHistoryNode(SOURCE, DESTINATION + UP, COLOR, PIECE_TYPE);

		makeMove(COLOR, SOURCE, DESTINATION, chessState);

		EXPECT_NE(Color::NEUTRAL, chessState.getNextTurn());
	}

	TEST_F(MakeMoveTest, move_nonExistentPiece)
	{
		const Color COLOR = Color::WHITE;
		const Position SOURCE = Position(4, 6);
		const Position DESTINATION = SOURCE + RIGHT + UP * 2;
		chessState.setState("4k3/8/8/8/8/8/8/4K3 w - - 0 1");

		EXPECT_THROW(makeMove(COLOR, SOURCE, DESTINATION, chessState), std::exception);
	}

	namespace capture
	{
		TEST_F(MakeMoveTest, capture_updateBoard)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 6);
			const Position DESTINATION = SOURCE + RIGHT + UP * 2;
			chessState.setState("4k3/8/8/8/5p2/8/4N3/4K3 w - - 0 1");

			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, ~COLOR));
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_board.posIsOccupied(DESTINATION, ~COLOR));
		}

		TEST_F(MakeMoveTest, capture_resetHalfTurns)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 6);
			const Position DESTINATION = SOURCE + RIGHT + UP * 2;
			chessState.setState("4k3/8/8/8/5p2/8/4N3/4K3 w - - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_EQ(0, chessState.m_numHalfTurns);
		}

		TEST_F(MakeMoveTest, capture_queenSideRook_black)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(2, 2);
			const Position DESTINATION = Position(0, 0);
			chessState.setState("r3k3/8/2N5/8/8/8/4N3/4K3 b q - 0 1");

			EXPECT_TRUE(chessState.m_bQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_bQueenSideCastle);
		}

		TEST_F(MakeMoveTest, capture_queenSideRook_white)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(1, 5);
			const Position DESTINATION = Position(0, 7);
			chessState.setState("4k3/8/8/8/8/1n6/8/R3K3 w Q - 0 1");

			EXPECT_TRUE(chessState.m_wQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_wQueenSideCastle);
		}

		TEST_F(MakeMoveTest, capture_kingSideRook_black)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(6, 2);
			const Position DESTINATION = Position(7, 0);
			chessState.setState("4k2r/8/6N1/8/8/8/8/4K3 b k - 0 1");

			EXPECT_TRUE(chessState.m_bKingSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_bKingSideCastle);
		}

		TEST_F(MakeMoveTest, capture_kingSideRook_white)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(6, 5);
			const Position DESTINATION = Position(7, 7);
			chessState.setState("4k3/8/8/8/8/6n1/8/4K2R w K - 0 1");

			EXPECT_TRUE(chessState.m_wKingSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_wKingSideCastle);
		}
	}

	namespace pawn
	{
		TEST_F(MakeMoveTest, resetHalfTurns)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 6);
			const Position DESTINATION = Position(4, 5);
			chessState.setState("4k3/8/8/8/8/8/4P3/4K3 w - - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_EQ(0, chessState.m_numHalfTurns);
		}

		TEST_F(MakeMoveTest, enPassant_updateBoard_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(3, 3);
			const Position DESTINATION = Position(4, 2);
			chessState.setState("4k3/8/8/3Pp3/8/8/8/4K3 w - d6 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_board.posIsOccupied(4, 3, ~COLOR));
		}

		TEST_F(MakeMoveTest, enPassant_updateBoard_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(3, 4);
			const Position DESTINATION = Position(4, 5);
			chessState.setState("4k3/8/8/8/3pP3/8/8/4K3 b - d3 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_board.posIsOccupied(4, 4, ~COLOR));
		}

		TEST_F(MakeMoveTest, promotion_updateBoard_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 1);
			const Position DESTINATION = Position(7, 0);
			const PieceType PROMOTION = PieceType::KNIGHT;
			chessState.setState("4k3/7P/8/8/8/8/8/4K3 w - - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState, PROMOTION);

			EXPECT_FALSE(chessState.m_board.posIsOccupied(SOURCE));
			EXPECT_FALSE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PieceType::PAWN));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PROMOTION));
		}

		TEST_F(MakeMoveTest, promotion_updateBoard_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(7, 6);
			const Position DESTINATION = Position(7, 7);
			const PieceType PROMOTION = PieceType::QUEEN;
			chessState.setState("4k3/8/8/8/8/8/7p/4K3 b - - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);

			EXPECT_FALSE(chessState.m_board.posIsOccupied(SOURCE));
			EXPECT_FALSE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PieceType::PAWN));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PROMOTION));
		}
	}

	namespace king
	{
		TEST_F(MakeMoveTest, disableCastling_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const Position DESTINATION = Position(4, 6);
			chessState.setState("4k3/7P/8/8/8/8/8/R3K2R w KQ - 0 1");

			EXPECT_TRUE(chessState.m_wKingSideCastle);
			EXPECT_TRUE(chessState.m_wQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState, PieceType::KNIGHT);
			EXPECT_FALSE(chessState.m_wKingSideCastle);
			EXPECT_FALSE(chessState.m_wQueenSideCastle);
		}

		TEST_F(MakeMoveTest, disableCastling_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(4, 0);
			const Position DESTINATION = Position(4, 1);
			chessState.setState("r3k2r/8/8/8/8/8/7p/4K3 b kq - 0 1");

			EXPECT_TRUE(chessState.m_bKingSideCastle);
			EXPECT_TRUE(chessState.m_bQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_bKingSideCastle);
			EXPECT_FALSE(chessState.m_bQueenSideCastle);
		}

		TEST_F(MakeMoveTest, castle_queenSide_updateBoard)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const Position DESTINATION = Position(2, 7);
			chessState.setState("4k3/7P/8/8/8/8/8/R3K2R w KQ - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_board.posIsOccupied(0, 7));
			EXPECT_FALSE(chessState.m_board.posIsOccupied(SOURCE));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PieceType::KING));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION + RIGHT, COLOR, PieceType::ROOK));
		}

		TEST_F(MakeMoveTest, castle_kingSide_updateBoard)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(4, 7);
			const Position DESTINATION = Position(6, 7);
			chessState.setState("4k3/7P/8/8/8/8/8/R3K2R w KQ - 0 1");

			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_board.posIsOccupied(7, 7));
			EXPECT_FALSE(chessState.m_board.posIsOccupied(SOURCE));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION, COLOR, PieceType::KING));
			EXPECT_TRUE(chessState.m_board.posIsOccupied(DESTINATION + LEFT, COLOR, PieceType::ROOK));
		}
	}

	namespace rook
	{
		TEST_F(MakeMoveTest, disableCastling_queenSide_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(0, 7);
			const Position DESTINATION = Position(0, 6);
			chessState.setState("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");

			EXPECT_TRUE(chessState.m_wQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_wQueenSideCastle);
		}

		TEST_F(MakeMoveTest, disableCastling_queenSide_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(0, 0);
			const Position DESTINATION = Position(0, 1);
			chessState.setState("r3k3/8/8/8/8/8/8/4K3 b q - 0 1");

			EXPECT_TRUE(chessState.m_bQueenSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_bQueenSideCastle);
		}

		TEST_F(MakeMoveTest, disableCastling_kingSide_white)
		{
			const Color COLOR = Color::WHITE;
			const Position SOURCE = Position(7, 7);
			const Position DESTINATION = Position(7, 6);
			chessState.setState("4k3/8/8/8/8/8/8/4K2R w K - 0 1");

			EXPECT_TRUE(chessState.m_wKingSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_wKingSideCastle);
		}

		TEST_F(MakeMoveTest, disableCastling_kingSide_black)
		{
			const Color COLOR = Color::BLACK;
			const Position SOURCE = Position(7, 0);
			const Position DESTINATION = Position(7, 1);
			chessState.setState("4k2r/8/8/8/8/8/8/4K3 b k - 0 1");

			EXPECT_TRUE(chessState.m_bKingSideCastle);
			makeMove(COLOR, SOURCE, DESTINATION, chessState);
			EXPECT_FALSE(chessState.m_bKingSideCastle);
		}
	}
}