#include "MakeMoveResponse.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
	{
		MakeMoveResponse::MakeMoveResponse(const json::object& json)
		{
			fromJson(json);
		}

		MessageType MakeMoveResponse::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::MAKE_MOVE_RESPONSE;
			return MESSAGE_TYPE;
		}

		void MakeMoveResponse::fromJson(const json::object& json)
		{
			success = json.at("success").as_bool();
			nextTurn = getColorFromString(json.at("nextTurn").as_string().c_str());
			winner = getColorFromString(json.at("winner").as_string().c_str());
		}

		json::object MakeMoveResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;
			data["success"] = success;
			data["nextTurn"] = toString(nextTurn);
			data["winner"] = toString(winner);
			result["data"] = data;

			return result;
		}
	}
}
