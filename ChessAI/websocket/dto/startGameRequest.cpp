#include "startGameRequest.h"
#include "../../chessServer.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
	{
		StartGameRequest::StartGameRequest(const json::object& json)
		{
			fromJson(json);
		}

		MessageType StartGameRequest::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::START_GAME_REQUEST;
			return MESSAGE_TYPE;
		}

		void StartGameRequest::fromJson(const json::object& json)
		{
			gameType = getGameTypeFromString(json.at("gameType").as_string().c_str());
		}

		json::object StartGameRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;
			data["gameType"] = toString(gameType);

			result["data"] = data;

			return result;
		}
	}
}
