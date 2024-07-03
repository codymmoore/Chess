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
			json::array whitePiecesJson = json.at("whitePieces").as_array();
			for (int i = 0; i < whitePiecesJson.size(); i++)
			{
				json::object pieceJson = whitePiecesJson[i].as_object();
				PieceNode piece;
				piece.m_pieceType = getPieceTypeFromString(pieceJson.at("type").as_string().c_str());

				json::object positionJson = pieceJson.at("position").as_object();
				piece.m_position.m_x = positionJson.at("x").as_int64();
				piece.m_position.m_y = positionJson.at("y").as_int64();

				whitePieces.push_back(piece);
			}

			json::array blackPiecesJson = json.at("blackPieces").as_array();
			for (int i = 0; i < blackPiecesJson.size(); i++)
			{
				json::object pieceJson = blackPiecesJson[i].as_object();
				PieceNode piece;
				piece.m_pieceType = getPieceTypeFromString(pieceJson.at("type").as_string().c_str());

				json::object positionJson = pieceJson.at("position").as_object();
				piece.m_position.m_x = positionJson.at("x").as_int64();
				piece.m_position.m_y = positionJson.at("y").as_int64();

				blackPieces.push_back(piece);
			}
		}

		json::object StartGameResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::array whitePiecesJson;
			for (const PieceNode& piece : whitePieces)
			{
				json::object pieceJson;
				pieceJson["type"] = toString(piece.m_pieceType);

				json::object positionJson;
				positionJson["x"] = piece.m_position.m_x;
				positionJson["y"] = piece.m_position.m_y;

				pieceJson["position"] = positionJson;
				whitePiecesJson.push_back(pieceJson);
			}
			data["whitePieces"] = whitePiecesJson;

			json::array blackPiecesJson;
			for (const PieceNode& piece : blackPieces)
			{
				json::object pieceJson;
				pieceJson["type"] = toString(piece.m_pieceType);

				json::object positionJson;
				positionJson["x"] = piece.m_position.m_x;
				positionJson["y"] = piece.m_position.m_y;

				pieceJson["position"] = positionJson;
				blackPiecesJson.push_back(pieceJson);
			}
			data["blackPieces"] = blackPiecesJson;

			result["data"] = data;

			return result;
		}
	}
}