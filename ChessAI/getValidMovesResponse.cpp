#include "getValidMovesResponse.h"
#include "utility.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
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
				Position move;
				move.m_x = moveJson.at("x").as_int64();
				move.m_y = moveJson.at("y").as_int64();
				moves.push_back(move);
			}
		}

		json::object GetValidMovesResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::array movesJson;
			for (const Position& move : moves)
			{
				json::object moveJson;
				moveJson["x"] = move.m_x;
				moveJson["y"] = move.m_y;
				movesJson.push_back(moveJson);
			}
			data["moves"] = movesJson;

			result["data"] = data;

			return result;
		}
	}
}
