#include "agent.h"
#include "../../../C++/timer/timer.cpp"
#include <iomanip>
#include <Windows.h>
#include "consoleManager.h"
#include <io.h>

#define FOREGROUND_WHITE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE

using std::cout;
using std::wcout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::pair;
using std::ostream;
using std::setw;

/* ----- GLOBAL CONSTANTS ----- */
const bool ALGORITHM = true,
MINIMAX_VALUE = false,
MOVE_ORDERING = false,
QUIESCENCE = false;

/* ----- Pretty Print ----- */
const LPCTSTR BLANK = L"█";
string foreground_color(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	return ("\e[38;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m");
}
string background_color(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	return ("\e[48;2;" + std::to_string(red) + ";" + std::to_string(green) + ";" + std::to_string(blue) + "m");
}

void pretty_print(const ChessState& game, const string indent = "")
{
	ConsoleManager consoleManager(GetStdHandle(STD_OUTPUT_HANDLE));

	cout << indent << " ";
	// Print out x coordinates
	for (int i = 0; i < NUM_FILES; i++)
		cout << i;
	cout << endl;

	for (int y = 0; y < NUM_RANKS; y++)
	{
		cout << indent << y;
		for (int x = 0; x < NUM_FILES; x++)
		{
			if ((y + x) % 2)
			{
				consoleManager.set_text_attribute(BACKGROUND_RED);
			}
			else
			{
				consoleManager.set_text_attribute(BACKGROUND_BLUE);
			}

			if (game.m_board.posIsOccupiedByColor(x, y, WHITE))
			{
				consoleManager.add_text_attribute(FOREGROUND_WHITE);
			}

			bool occupied = false;
			// outputStr += BLANK;

			int color = WHITE;
			while (color < NUM_COLORS && !occupied)
			{
				int pieceType = PAWN;
				while (pieceType < NUM_PIECE_TYPES && !occupied)
				{
					if (game.m_board.posIsOccupiedByColorPiece(x, y, (Color)color, (PieceType)pieceType))
					{
						cout << PIECE_SYMBOLS[color][pieceType];
						//outputStr += PRETTY_PIECE_SYMBOLS[pieceType];
						occupied = true;
					}
					pieceType += 1;
				}
				color += 1;
			}

			if (!occupied)
			{
				//consoleManager.add_text_attribute(FOREGROUND_GREEN);
				cout << " ";
			}
			consoleManager.reset_text_attribute();
		}
		consoleManager.reset_text_attribute();
		cout << endl;
		//cout << reset << endl;
	}
}

template<typename T>
void print_vector(const vector<T>& vec, const string& indent = "  ", const string& delim = "\n")
{
	cout << indent << "{";
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i != 0)
			cout << indent;
		cout << vec[i];
		if (i != (int)vec.size() - 1)
			cout << delim;

	}
	cout << "}" << endl;
}

int main()
{
	/*
	===========================================================================================
	---------------------------------------- ALGORITHM ----------------------------------------
	=========================================================================================== */
	if (ALGORITHM)
	{
		// ChessState game("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
		ChessState game;
		// ChessState game("k6K/8/8/8/8/8/p7/8 b - - 0 1"); // promotion_black.txt
		// ChessState game("3k4/1B1pppp1/1p6/6P1/8/6P1/3BP3/4K1N1 w - - 0 1");
		// ChessState game("r1bqkbnr/p1pppppp/1p6/8/8/2N1PQ2/PPnP1PPP/1RBK1BNR b - - 1 6");

		Agent whitePlayer(Color::WHITE, game);
		Agent blackPlayer(Color::BLACK, game);
		Timer::Timer timer;

		while (game.getWinner() == Color::NEUTRAL && game.getNextTurn() != Color::NEUTRAL)
		{
			Agent* player = (game.getNextTurn() == Color::WHITE ? &whitePlayer : &blackPlayer);
			cout << "---------------------------------------- " << toString(game.getNextTurn()) << "'S TURN ----------------------------------------" << endl;
			cout << "BOARD:" << endl;
			pretty_print(game, "  ");
			cout << "FEN: " << game.getFenString() << endl;
			cout << "  WHITE PIECES: ";
			print_vector(game.getWhitePieces(), "", " ");
			cout << "  BLACK PIECES: ";
			print_vector(game.getBlackPieces(), "", " ");

			timer.start_timer();
			string move = player->prunedIterDepthLimitedMinimax();
			timer.stop_timer();
			cout << "Turn Duration: " << timer.get_duration() << endl;
			cout << "Move: " << move << endl;
		}

		switch (game.getWinner())
		{
			case Color::WHITE:
				cout << "WHITE WINS!";
				break;
			case Color::BLACK:
				cout << "BLACK WINS!";
				break;
			case Color::NEUTRAL:
				cout << "TIE!" << endl;
				break;
			default:
				break;
		}
		cout << "FINAL STATE:" << endl;
		pretty_print(game, "");
		cout << endl;
	}

	return 0;
}