#include "getValidMovesRequest.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
	{
		GetValidMovesRequest::GetValidMovesRequest(const json::object& json)
		{
			fromJson(json);
		}

		MessageType GetValidMovesRequest::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::GET_VALID_MOVES_REQUEST;
			return MESSAGE_TYPE;
		}

		void GetValidMovesRequest::fromJson(const json::object& json)
		{
			json::object pieceJson = json.at("piece").as_object();
			piece.m_pieceType = getPieceTypeFromString(pieceJson.at("type").as_string().c_str());
			color = getColorFromString(pieceJson.at("color").as_string().c_str());

			json::object positionJson = pieceJson.at("position").as_object();
			piece.m_position.m_x = positionJson.at("x").as_int64();
			piece.m_position.m_y = positionJson.at("y").as_int64();
		}

		json::object GetValidMovesRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::object pieceJson;
			pieceJson["type"] = toString(piece.m_pieceType);
			pieceJson["color"] = toString(color);

			json::object positionJson;
			positionJson["x"] = piece.m_position.m_x;
			positionJson["y"] = piece.m_position.m_y;
			pieceJson["position"] = positionJson;

			data["piece"] = pieceJson;

			result["data"] = data;
			return result;
		}
	}
}
