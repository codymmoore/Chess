#include "endGameResponse.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
	{
		EndGameResponse::EndGameResponse(const json::object& json)
		{
			fromJson(json);
		}

		MessageType EndGameResponse::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::END_GAME_RESPONSE;
			return MESSAGE_TYPE;
		}

		void EndGameResponse::fromJson(const json::object& json)
		{
		}

		json::object EndGameResponse::toJson() const
		{

			json::object result = Message::toJson();

			json::object data;
			result["data"] = data;

			return result;
		}
	}
}
