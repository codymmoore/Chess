#include "consoleManager.h"

ConsoleManager::ConsoleManager(HANDLE hConsole)
{
	this->m_hConsole = hConsole;
	GetConsoleScreenBufferInfo(m_hConsole, &m_consoleInfo);
	set_default_text_attribute();
}

void ConsoleManager::set_default_text_attribute()
{
	m_defaultTextAttributes = m_consoleInfo.wAttributes;
}

/// Use constants defined in consoleapi2.h
///     Ex. consoleManager.set_text_attribute(FOREGROUND_BLUE | BACKGROUND_RED);
void ConsoleManager::set_text_attribute(const int textAttr)
{
	m_consoleInfo.wAttributes = textAttr;
	SetConsoleTextAttribute(m_hConsole, m_consoleInfo.wAttributes);
}

/// Use constants defined in consoleapi2.h
///     Ex. consoleManager.add_text_attribute(FOREGROUND_BLUE | BACKGROUND_RED);
void ConsoleManager::add_text_attribute(const int textAttr)
{
	m_consoleInfo.wAttributes |= textAttr;
	SetConsoleTextAttribute(m_hConsole, m_consoleInfo.wAttributes);
}

void ConsoleManager::reset_text_attribute()
{
	SetConsoleTextAttribute(m_hConsole, m_defaultTextAttributes);
}
