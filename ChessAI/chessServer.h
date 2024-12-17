#pragma once

#include "chessController.h"
#include "websocket/webSocketManager.h"
#include "websocket/message/message.h"
#include "chess.h"
#include <memory>

namespace boost
{
	namespace asio
	{
		namespace ip
		{
			class tcp;
		}
	}
}
class Agent;

/**
 * Indicates the type of chess game.
 */
enum GameType
{
	HUMAN_VS_HUMAN = 0,
	HUMAN_VS_AI = 1,
	AI_VS_AI = 2
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
std::string toString(const GameType gameType);

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
	 * Handles a human player's turn.
	 *
	 * /return true if game is still in progress, false otherwise
	 */
	bool handleHumanTurn();

	/**
	 * Handles an AI player's turn.
	 *
	 * /return true if game is still in progress, false otherwise
	 */
	bool handleAiTurn(Agent& agent);

	/**
	 * Handles client requests.
	 *
	 * \param request The message received from the client
	 * \return The response object to be sent back to the client
	 */
	std::unique_ptr<websocket::message::Message> handleRequest(const websocket::message::Message& request);

	websocket::WebSocketManager _webSocketManager;
	ChessState _chessState;
	ChessController _chessController;
};
