#include "updateClientRequest.h"
#include <boost/json.hpp>

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
			json::object sourceJson = json.at("source").as_object();
			source.m_x = sourceJson["x"].as_int64();
			source.m_y = sourceJson["y"].as_int64();

			json::object destinationJson = json.at("destination").as_object();
			destination.m_x = destinationJson["x"].as_int64();
			destination.m_y = destinationJson["y"].as_int64();

			promotion = getPieceTypeFromString(json.at("promotion").as_string().c_str());

			nextTurn = getColorFromString(json.at("nextTurn").as_string().c_str());
			winner = getColorFromString(json.at("winner").as_string().c_str());
		}

		json::object UpdateClientRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			json::object sourceJson;
			sourceJson["x"] = source.m_x;
			sourceJson["y"] = source.m_y;
			data["source"] = sourceJson;

			json::object destinationJson;
			destinationJson["x"] = destination.m_x;
			destinationJson["y"] = destination.m_y;
			data["destination"] = destinationJson;

			data["promotion"] = toString(promotion);
			data["nextTurn"] = toString(nextTurn);
			data["winner"] = toString(winner);

			result["data"] = data;

			return result;
		}
	}
}
