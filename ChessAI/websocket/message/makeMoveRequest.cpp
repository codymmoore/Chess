#include "makeMoveRequest.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
	{
		MakeMoveRequest::MakeMoveRequest(const json::object& json)
		{
			fromJson(json);
		}

		MessageType MakeMoveRequest::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::MAKE_MOVE_REQUEST;
			return MESSAGE_TYPE;
		}

		void MakeMoveRequest::fromJson(const json::object& json)
		{
			json::object pieceJson = json.at("piece").as_object();
			piece.color = getColorFromString(pieceJson.at("color").as_string().c_str());
			piece.type = getPieceTypeFromString(pieceJson.at("type").as_string().c_str());

			json::object positionJson = pieceJson.at("position").as_object();
			piece.position.m_x = positionJson.at("x").as_int64();
			piece.position.m_y = positionJson.at("y").as_int64();

			json::object destinationJson = json.at("destination").as_object();
			destination.m_x = destinationJson["x"].as_int64();
			destination.m_y = destinationJson["y"].as_int64();

			promotion = getPieceTypeFromString(json.at("promotion").as_string().c_str());
		}

		json::object MakeMoveRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::object pieceJson;
			pieceJson["color"] = toString(piece.color);
			pieceJson["type"] = toString(piece.type);

			json::object positionJson;
			positionJson["x"] = piece.position.m_x;
			positionJson["y"] = piece.position.m_y;
			pieceJson["position"] = positionJson;

			data["piece"] = pieceJson;

			json::object destinationJson;
			destinationJson["x"] = destination.m_x;
			destinationJson["y"] = destination.m_y;
			data["destination"] = destinationJson;

			data["promotion"] = toString(promotion);

			result["data"] = data;

			return result;
		}
	}
}
