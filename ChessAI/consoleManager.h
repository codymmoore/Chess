#pragma once

#include <Windows.h>

/// @brief Manage attributes for a console.
class ConsoleManager
{
public:
	/// @brief Construct a new ConsoleUtil object.
	/// @param hConsole The console to be managed.
	ConsoleManager(HANDLE hConsole);

	/// @brief Set the default text attributes for a console.
	void set_default_text_attribute();

	/// @brief Set the text attributes for a console.
	/// @param textAttr The text attributes for the console.
	void set_text_attribute(const int textAttr);

	/// @brief Add a text attribute to a console.
	/// @param textAttr The text attributes for the console. 
	void add_text_attribute(const int textAttr);

	/// @brief Reset text attributes to the set default.
	void reset_text_attribute();

private:
	HANDLE m_hConsole;
	CONSOLE_SCREEN_BUFFER_INFO m_consoleInfo;
	WORD m_defaultTextAttributes;
};

//namespace consoleUtil
//{	
//	/// @brief Retrieves the default text attributes for the standard output console.
//	/// @return The default text attributes for the standard output console or NULL if the attributes fail to be retrieved.
//	WORD get_default_text_attributes()
//	{
//		static WORD defaultTextAttributes = NULL;
//
//		if (!defaultTextAttributes)
//		{
//			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//			CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
//
//			if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo))
//			{
//				defaultTextAttributes = consoleInfo.wAttributes;
//			}
//		}
//
//		return defaultTextAttributes;
//	}
//
//	/// @brief Set the attributes for the text to be written to the standard output console.
//	///        Ex. cout << set_text_attr<FOREGROUND_RED | BACKGROUND_BLUE>;
//	/// @tparam textAttr The attribute to be added to the console text. Use constants defined in consoleapi2.h.
//	/// @param ostream The stream being used to output text to the console.
//	/// @return The output stream.
//	template <int textAttr>
//	std::ostream& set_text_attr(std::ostream& ostream)
//	{
//		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
//
//		// Set default text attributes before changing
//		get_default_text_attributes();
//
//		if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo))
//		{
//			consoleInfo.wAttributes |= textColor;
//			SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);
//		}
//
//		return ostream;
//	}
//
//	/// @brief Add an attribute to the text to be written to the standard output console.
//	/// @tparam textAttr The attribute to be added to the console text. Use constants defined in consoleapi2.h.
//	/// @param ostream The stream being used to output text to the console.
//	/// @return The output stream.
//	template <int textAttr>
//	std::ostream& add_text_attr(std::ostream& ostream)
//	{
//		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
//
//		// Set default text attributes before changing
//		get_default_text_attributes();
//
//		if (GetConsoleScreenBufferInfo(hConsole, &consoleInfo))
//		{
//			consoleInfo.wAttributes |= textAttr;
//			SetConsoleTextAttribute(hConsole, consoleInfo.wAttributes);
//		}
//
//		return ostream;
//	}
//
//	/// @brief Reset the text attributes for the standard output console to the default.
//	/// @param ostream The stream being used to output text to the console.
//	/// @return The output stream.
//	std::ostream& reset_text_attr(std::ostream& ostream)
//	{
//		WORD defaultAttributes = get_default_text_attributes();
//
//		if (defaultAttributes != NULL)
//		{
//			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//			SetConsoleTextAttribute(hConsole, defaultAttributes);
//		}
//
//		return ostream;
//	}
//};