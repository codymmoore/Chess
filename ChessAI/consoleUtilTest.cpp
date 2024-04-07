#include "agent.h"
#include "../../../C++/Libraries/timer.cpp"
#include <iomanip>
#include <Windows.h>
#include "consoleUtil.h"
#include <io.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>

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

using namespace consoleUtil;

int main()
{
	cout << reset;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

	SetConsoleTextAttribute(hStdOut, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

	//if (GetConsoleScreenBufferInfo(hStdOut, &consoleInfo))
	//{
	//	consoleInfo.wAttributes |= textColor;
	//	SetConsoleTextAttribute(hStdOut, consoleInfo.wAttributes);
	//}

	//for (int i = 0; i < 255; i++)
	//{
	//	cout << i << ": " << consoleUtil::color<> << "uh" << endl;
	//}

	return 0;
}