#include "MakeMoveResponse.h"

#include <boost/json.hpp>

#include "../../util/bitboard/bitboardSet.h"
#include "../../util/bitboard/bitboardUtil.h"

using namespace boost;

namespace websocket
{
	namespace message
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

			const json::array boardJson = json.at("board").as_array();
			board = std::make_unique<const util::bitboard::BitboardSet>(util::bitboard::getBoardFromJson(boardJson));
			nextTurn = util::getColorFromString(json.at("nextTurn").as_string().c_str());
			winner = util::getColorFromString(json.at("winner").as_string().c_str());
		}

		json::object MakeMoveResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;
			data["success"] = success;
			data["board"] = util::bitboard::getJsonFromBoard(*board);
			data["nextTurn"] = util::toString(nextTurn);
			data["winner"] = (winner ? boost::json::value(util::toString(winner.value())) : boost::json::value(nullptr));
			result["data"] = data;

			return result;
		}
	}
}
