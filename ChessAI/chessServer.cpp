#include "chessServer.h"
#include "agent.h"

#include "websocket/message/startGameRequest.h"
#include "websocket/message/startGameResponse.h"
#include "websocket/message/getValidMovesRequest.h"
#include "websocket/message/getValidMovesResponse.h"
#include "websocket/message/makeMoveRequest.h"
#include "websocket/message/makeMoveResponse.h"
#include "websocket/message/updateClientRequest.h"
#include "websocket/message/updateClientResponse.h"
#include "websocket/message/endGameRequest.h"
#include "websocket/message/endGameResponse.h"

#include <boost/asio/ip/tcp.hpp>

using namespace websocket::message;
using tcp = boost::asio::ip::tcp;

const unsigned int NUMBER_OF_GAME_TYPES = 3;

constexpr const char* GAME_TYPE_STRINGS[NUMBER_OF_GAME_TYPES] = {
	"HUMAN_VS_HUMAN",
	"HUMAN_VS_AI",
	"AI_VS_AI"
};

GameType getGameTypeFromString(const std::string& gameTypeString)
{
	for (int i = 0; i < NUMBER_OF_GAME_TYPES; i++)
	{
		if (GAME_TYPE_STRINGS[i] == gameTypeString)
			return static_cast<GameType>(i);
	}

	const std::string exceptionMessage = "No matching value of GameType for string \"" + gameTypeString + "\"";
	throw std::exception(exceptionMessage.c_str());
}

std::string toString(const GameType gameType)
{
	return GAME_TYPE_STRINGS[gameType];
}

ChessServer::ChessServer(const tcp::endpoint& endpoint) : _webSocketManager(endpoint), _chessController(_chessState)
{
}

void ChessServer::run()
{
	while (true)
	{
		const std::unique_ptr<Message> message = _webSocketManager.read();

		if (message->getMessageType() != START_GAME_REQUEST)
		{
			std::string errorMessage = "Unexpected message type received. Game session has not been started: " + toString(message->getMessageType());
			throw std::exception(errorMessage.c_str());
		}
		const GameType gameType = dynamic_cast<StartGameRequest*>(message.get())->gameType;

		StartGameResponse response = _chessController.startGame(static_cast<StartGameRequest&>(*message));
		_webSocketManager.write(response);

		switch (gameType)
		{
		case HUMAN_VS_AI:
			humanVsAi();
			break;
		case AI_VS_AI:
			aiVsAi();
			break;
		}
	}
}

void ChessServer::humanVsAi()
{
	bool gameInProgress = true;
	Agent agent(Color::BLACK, _chessState);

	while (gameInProgress)
	{
		gameInProgress = handleHumanTurn();

		if (_chessState.getNextTurn() != NEUTRAL)
			gameInProgress = handleAiTurn(agent);
	}
}

void ChessServer::aiVsAi()
{
	bool gameInProgress = true;
	Agent agentWhite(Color::WHITE, _chessState);
	Agent agentBlack(Color::BLACK, _chessState);

	while (gameInProgress)
	{
		gameInProgress = handleAiTurn(_chessState.getNextTurn() == WHITE ? agentWhite : agentBlack);
	}
}

bool ChessServer::handleHumanTurn()
{
	bool run = true;
	while (run)
	{
		const std::unique_ptr<Message> message = _webSocketManager.read();
		const MessageType messageType = message->getMessageType();

		const std::unique_ptr<Message> response = handleRequest(*message);
		_webSocketManager.write(*response);

		if (messageType == MessageType::END_GAME_REQUEST)
			return false;
		run = messageType != MessageType::MAKE_MOVE_REQUEST || !static_cast<MakeMoveResponse&>(*response).success;
	}

	return _chessState.getNextTurn() != NEUTRAL;
}

bool ChessServer::handleAiTurn(Agent& agent)
{
	MoveNode move = agent.prunedIterDepthLimitedMinimax();

	UpdateClientRequest updateClientRequest;
	updateClientRequest.board = std::make_unique<const util::BitboardSet>(_chessState.getBoard());
	updateClientRequest.nextTurn = _chessState.getNextTurn();
	updateClientRequest.winner = _chessState.getWinner();

	_webSocketManager.write(updateClientRequest);
	const std::unique_ptr<Message> message = _webSocketManager.read();

	if (message->getMessageType() == END_GAME_REQUEST)
	{
		const EndGameResponse response = _chessController.endGame(static_cast<EndGameRequest&>(*message));
		_webSocketManager.write(response);
		return false;
	}

	return _chessState.getNextTurn() != NEUTRAL;
}

std::unique_ptr<Message> ChessServer::handleRequest(const Message& request)
{
	switch (request.getMessageType())
	{
	case GET_VALID_MOVES_REQUEST:
	{
		const GetValidMovesRequest& getValidMovesRequest = dynamic_cast<const GetValidMovesRequest&>(request);
		return std::make_unique<GetValidMovesResponse>(_chessController.getValidMoves(getValidMovesRequest));
	}
	case MAKE_MOVE_REQUEST:
	{
		const MakeMoveRequest& makeMoveRequest = dynamic_cast<const MakeMoveRequest&>(request);
		return std::make_unique<MakeMoveResponse>(_chessController.makeMove(makeMoveRequest));
	}
	case END_GAME_REQUEST:
	{
		return std::make_unique<EndGameResponse>();
	}
	default:
	{
		std::string errorMessage = "Unexpected message type received: " + toString(request.getMessageType());
		throw std::exception(errorMessage.c_str());
	}
	}
}
