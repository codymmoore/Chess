#include "startGameResponse.h"
#include "../../chess.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace dto
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
			json::array piecesJson = json.at("pieces").as_array();
			for (int i = 0; i < piecesJson.size(); i++)
			{
				json::object pieceJson = piecesJson[i].as_object();
				PiecePayload piece;

				piece.color = getColorFromString(pieceJson.at("color").as_string().c_str());
				piece.type = getPieceTypeFromString(pieceJson.at("type").as_string().c_str());

				json::object positionJson = pieceJson.at("position").as_object();
				Position position;
				position.m_x = positionJson.at("x").as_int64();
				position.m_y = positionJson.at("y").as_int64();
				piece.position = position;

				pieces.push_back(piece);

				nextTurn = getColorFromString(json.at("nextTurn").as_string().c_str());
				winner = getColorFromString(json.at("winner").as_string().c_str());
			}
		}

		json::object StartGameResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::array piecesJson;
			for (const PiecePayload& piece : pieces)
			{
				json::object pieceJson;
				pieceJson["color"] = toString(piece.color);
				pieceJson["type"] = toString(piece.type);

				json::object positionJson;
				positionJson["x"] = piece.position.m_x;
				positionJson["y"] = piece.position.m_y;

				pieceJson["position"] = positionJson;
				piecesJson.push_back(pieceJson);
			}
			data["pieces"] = piecesJson;

			data["nextTurn"] = toString(nextTurn);
			data["winner"] = toString(winner);

			result["data"] = data;

			return result;
		}
	}
}