#include "startGameResponse.h"
#include "../../chess.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
	{
		StartGameResponse::StartGameResponse(const json::object& json)
		{
			fromJson(json);
		}

		MessageType StartGameResponse::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::START_GAME_RESPONSE;
			return MESSAGE_TYPE;
		}

		void StartGameResponse::fromJson(const json::object& json)
		{
			const json::array boardJson = json.at("board").as_array();
			board = std::make_unique<const util::BitboardSet>(util::getBoardFromJson(boardJson));
			nextTurn = util::getColorFromString(json.at("nextTurn").as_string().c_str());
			winner = util::getColorFromString(json.at("winner").as_string().c_str());
		}

		json::object StartGameResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			data["board"] = util::getJsonFromBoard(*board);
			data["nextTurn"] = util::toString(nextTurn);
			data["winner"] = util::toString(winner);

			result["data"] = data;

			return result;
		}
	}
}