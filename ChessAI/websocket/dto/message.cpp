#include "message.h"

#include "startGameRequest.h"
#include "startGameResponse.h"
#include "getValidMovesRequest.h"
#include "getValidMovesResponse.h"
#include "makeMoveRequest.h"
#include "makeMoveResponse.h"
#include "updateClientRequest.h"
#include "updateClientResponse.h"
#include "endGameRequest.h"
#include "endGameResponse.h"

#include <boost/json.hpp>

using namespace boost;

const unsigned int NUMBER_OF_MESSAGE_TYPES = 10;

constexpr const char* MESSAGE_TYPE_STRINGS[NUMBER_OF_MESSAGE_TYPES] = {
	"START_GAME_REQUEST",
	"START_GAME_RESPONSE",
	"GET_VALID_MOVES_REQUEST",
	"GET_VALID_MOVES_RESPONSE",
	"MAKE_MOVE_REQUEST",
	"MAKE_MOVE_RESPONSE",
	"UPDATE_CLIENT_REQUEST",
	"UPDATE_CLIENT_RESPONSE",
	"END_GAME_REQUEST",
	"END_GAME_RESPONSE"
};

namespace websocket
{
	namespace dto
	{
		MessageType getMessageTypeFromString(const std::string& messageTypeString)
		{
			for (int i = 0; i < NUMBER_OF_MESSAGE_TYPES; i++)
			{
				if (MESSAGE_TYPE_STRINGS[i] == messageTypeString)
					return static_cast<MessageType>(i);
			}

			std::string errorMessage = "No matching value of MessageType for string \"" + messageTypeString + "\"";
			throw std::exception(errorMessage.c_str());
		}

		std::string toString(MessageType messageType)
		{
			return MESSAGE_TYPE_STRINGS[messageType];
		}

		json::object Message::toJson() const
		{
			json::object result;
			result["type"] = toString(getMessageType());
			return result;
		}

		std::unique_ptr<Message> Message::createMessage(const json::object& json)
		{
			const MessageType messageType = getMessageTypeFromString(json.at("type").as_string().c_str());
			const json::object dataJson = json.at("data").as_object();

			switch (messageType)
			{
			case START_GAME_REQUEST:
				return std::make_unique<StartGameRequest>(StartGameRequest(dataJson));
			case START_GAME_RESPONSE:
				return std::make_unique<StartGameResponse>(StartGameResponse(dataJson));
			case GET_VALID_MOVES_REQUEST:
				return std::make_unique<GetValidMovesRequest>(GetValidMovesRequest(dataJson));
			case GET_VALID_MOVES_RESPONSE:
				return std::make_unique<GetValidMovesResponse>(GetValidMovesResponse(dataJson));
			case MAKE_MOVE_REQUEST:
				return std::make_unique<MakeMoveRequest>(MakeMoveRequest(dataJson));
			case MAKE_MOVE_RESPONSE:
				return std::make_unique<MakeMoveResponse>(MakeMoveResponse(dataJson));
			case UPDATE_CLIENT_REQUEST:
				return std::make_unique<UpdateClientRequest>(UpdateClientRequest(dataJson));
			case UPDATE_CLIENT_RESPONSE:
				return std::make_unique<UpdateClientResponse>(UpdateClientResponse(dataJson));
			case END_GAME_REQUEST:
				return std::make_unique<EndGameRequest>(EndGameRequest(dataJson));
			case END_GAME_RESPONSE:
				return std::make_unique<EndGameResponse>(EndGameResponse(dataJson));
			}
		}
	}
}
