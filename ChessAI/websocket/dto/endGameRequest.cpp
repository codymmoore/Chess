#include "endGameRequest.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
	{
		EndGameRequest::EndGameRequest(const json::object& json)
		{
			fromJson(json);
		}

		MessageType EndGameRequest::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::END_GAME_REQUEST;
			return MESSAGE_TYPE;
		}

		void EndGameRequest::fromJson(const json::object& requestJson)
		{

		};

		json::object EndGameRequest::toJson() const
		{
			json::object result = Message::toJson();
			result["data"] = json::object();
			return result;
		}
	}
}
