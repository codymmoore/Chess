#pragma once

#include "chessController.h"
#include "webSocketManager.h"
#include "message.h"
#include "chess.h"
#include <memory>

namespace boost
{
	namespace json
	{
		class object;
	}
	namespace asio
	{
		namespace ip
		{
			class tcp;
		}
	}
}

/**
 * Indicates the type of chess game.
 */
enum GameType
{
	HUMAN_VS_HUMAN,
	HUMAN_VS_AI,
	AI_VS_AI
};

/**
 * Converts a string to a value of GameType.
 *
 * \param gameTypeString The string to be converted
 * \return The corresponding GameType value
 */
GameType getGameTypeFromString(const std::string& string);

/**
 * Converts a GameType value to a string.
 *
 * \param gameType The GameType value to be converted
 * \return The corresponding string
 */
std::string toString(const GameType gamaeType);

/**
 * Manages client-server communications and game flow.
 */
class ChessServer
{
public:
	/**
	 * Creates a new instance of ChessServer.
	 *
	 * \param endpoint The endpoint used to connect to the server
	 */
	ChessServer(const boost::asio::ip::tcp::endpoint& endpoint);

	/**
	 * Runs the game server.
	 *
	 */
	void run();

private:
	/**
	 * Runs a human-vs-AI game.
	 *
	 */
	void humanVsAi();

	/**
	 * Runs an AI-vs-AI game.
	 *
	 */
	void aiVsAi();

	/**
	 * Handles client requests.
	 *
	 * \param request The message received from the client
	 * \return The response object to be sent back to the client
	 */
	std::unique_ptr<websocket::dto::Message> handleRequest(const websocket::dto::Message& request);

	websocket::WebSocketManager _webSocketManager;
	ChessState _chessState;
	ChessController _chessController;
};
