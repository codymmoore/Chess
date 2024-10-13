#include "getValidMovesResponse.h"
#include "../../util/utility.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
	{
		GetValidMovesResponse::GetValidMovesResponse(const json::object& json)
		{
			fromJson(json);
		}

		MessageType GetValidMovesResponse::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::GET_VALID_MOVES_RESPONSE;
			return MESSAGE_TYPE;
		}

		void GetValidMovesResponse::fromJson(const json::object& json)
		{
			json::array movesJson = json.at("moves").as_array();

			for (int i = 0; i < movesJson.size(); i++)
			{
				json::object moveJson = movesJson[i].as_object();
				util::Position move;
				move.x = moveJson.at("x").as_int64();
				move.y = moveJson.at("y").as_int64();
				moves.push_back(move);
			}
		}

		json::object GetValidMovesResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::array movesJson;
			for (const util::Position& move : moves)
			{
				json::object moveJson;
				moveJson["x"] = move.x;
				moveJson["y"] = move.y;
				movesJson.push_back(moveJson);
			}
			data["moves"] = movesJson;

			result["data"] = data;

			return result;
		}
	}
}
