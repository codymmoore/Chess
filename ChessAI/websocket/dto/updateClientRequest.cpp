#include "updateClientRequest.h"
#include <boost/json.hpp>
#include "../../BitBoard.h"

using namespace boost;

namespace websocket
{
	namespace dto
	{
		UpdateClientRequest::UpdateClientRequest(const json::object& json)
		{
			fromJson(json);
		}

		MessageType UpdateClientRequest::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::UPDATE_CLIENT_REQUEST;
			return MESSAGE_TYPE;
		}

		void UpdateClientRequest::fromJson(const json::object& json)
		{
			const json::array boardJson = json.at("board").as_array();
			board = std::make_unique<const BitBoard>(getBoardFromJson(boardJson));
			nextTurn = getColorFromString(json.at("nextTurn").as_string().c_str());
			winner = getColorFromString(json.at("winner").as_string().c_str());
		}

		json::object UpdateClientRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			data["board"] = getJsonFromBoard(*board);
			data["nextTurn"] = toString(nextTurn);
			data["winner"] = toString(winner);

			result["data"] = data;

			return result;
		}
	}
}
