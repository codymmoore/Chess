#include "chessServer.h"
#include "agent.h"

#include "websocket/dto/startGameRequest.h"
#include "websocket/dto/startGameResponse.h"
#include "websocket/dto/getValidMovesRequest.h"
#include "websocket/dto/getValidMovesResponse.h"
#include "websocket/dto/makeMoveRequest.h"
#include "websocket/dto/makeMoveResponse.h"
#include "websocket/dto/updateClientRequest.h"
#include "websocket/dto/updateClientResponse.h"
#include "websocket/dto/endGameRequest.h"
#include "websocket/dto/endGameResponse.h"

#include <boost/asio/ip/tcp.hpp>

using namespace websocket::dto;
using tcp = boost::asio::ip::tcp;

GameType getGameTypeFromString(const std::string& gameTypeString)
{
	if (gameTypeString == "HUMAN_VS_HUMAN")
		return GameType::HUMAN_VS_HUMAN;
	else if (gameTypeString == "HUMAN_VS_AI")
		return GameType::HUMAN_VS_AI;
	else if (gameTypeString == "AI_VS_AI")
		return GameType::AI_VS_AI;
	else
	{
		const std::string exceptionMessage = "No matching value of GameType for string \"" + gameTypeString + "\"";
		throw std::exception(exceptionMessage.c_str());
	}
}

std::string toString(const GameType gameType)
{
	switch (gameType)
	{
	case GameType::HUMAN_VS_HUMAN:
		return "HUMAN_VS_HUMAN";
	case GameType::HUMAN_VS_AI:
		return "HUMAN_VS_AI";
	case GameType::AI_VS_AI:
		return "AI_VS_AI";
	}
}

ChessServer::ChessServer(const tcp::endpoint& endpoint) : _webSocketManager(endpoint), _chessController(_chessState)
{
}

void ChessServer::run()
{
	while (true)
	{
		std::unique_ptr<Message> message = _webSocketManager.read();

		if (message->getMessageType() != START_GAME_REQUEST)
		{
			std::string errorMessage = "Unexpected message type received. Game session has not been started: " + toString(message->getMessageType());
			throw std::exception(errorMessage.c_str());
		}

		const GameType gameType = dynamic_cast<StartGameRequest*>(message.get())->gameType;

		StartGameResponse response;
		response.whitePieces = _chessState.getWhitePieces();
		response.blackPieces = _chessState.getBlackPieces();
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
		_chessState.reset();
	}
}

void ChessServer::humanVsAi()
{
	bool gameInProgress = true;
	Agent agent(Color::BLACK, _chessState);

	while (gameInProgress)
	{
		MessageType messageType;

		// Handle user input
		do
		{
			std::unique_ptr<Message> message = _webSocketManager.read();
			messageType = message->getMessageType();

			std::unique_ptr<Message> response = handleRequest(*message);
			_webSocketManager.write(*response);

			if (messageType == MessageType::END_GAME_REQUEST)
				return;
		} while (messageType != MessageType::MAKE_MOVE_REQUEST);

		UpdateClientRequest updateClientRequest;
		if (_chessState.getNextTurn() != NEUTRAL)
		{
			// Handle AI move
			MoveNode move = agent.prunedIterDepthLimitedMinimax();

			updateClientRequest.source = move.m_source;
			updateClientRequest.destination = move.m_destination;
			updateClientRequest.promotion = move.m_promotion;
		}
		updateClientRequest.nextTurn = _chessState.getNextTurn();
		updateClientRequest.winner = _chessState.getWinner();

		_webSocketManager.write(updateClientRequest);
		_webSocketManager.read();

		std::unique_ptr<Message> message = _webSocketManager.read();

		if (message->getMessageType() == END_GAME_REQUEST)
		{
			gameInProgress = false;
			_webSocketManager.write(EndGameResponse());
		}

		gameInProgress = _chessState.getNextTurn() != NEUTRAL;
	}
}

void ChessServer::aiVsAi()
{
	bool gameInProgress = true;
	int it = 0;
	Agent agent1(Color::WHITE, _chessState);
	Agent agent2(Color::BLACK, _chessState);

	while (gameInProgress)
	{
		Agent& agent = it % 2 == 0 ? agent1 : agent2;

		MoveNode move = agent.prunedIterDepthLimitedMinimax();

		UpdateClientRequest updateClientRequest;

		gameInProgress = _chessState.getNextTurn() != NEUTRAL;

		if (gameInProgress)
		{
			updateClientRequest.source = move.m_source;
			updateClientRequest.destination = move.m_destination;
			updateClientRequest.promotion = move.m_promotion;
		}
		updateClientRequest.nextTurn = _chessState.getNextTurn();
		updateClientRequest.winner = _chessState.getWinner();

		_webSocketManager.write(updateClientRequest);
		std::unique_ptr<Message> message = _webSocketManager.read();

		if (message->getMessageType() == END_GAME_REQUEST)
		{
			gameInProgress = false;
			_webSocketManager.write(EndGameResponse());
		}
		else if (message->getMessageType() != UPDATE_CLIENT_RESPONSE)
		{
			std::string errorMessage = "Unexpected request type received: " + toString(message->getMessageType());
			throw std::exception(errorMessage.c_str());
		}
	}
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
