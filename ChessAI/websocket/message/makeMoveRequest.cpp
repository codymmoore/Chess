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
			player = util::getColorFromString(json.at("player").as_string().c_str());

			json::object sourceJson = json.at("source").as_object();
			source.x = sourceJson["x"].as_int64();
			source.y = sourceJson["y"].as_int64();

			json::object destinationJson = json.at("destination").as_object();
			destination.x = destinationJson["x"].as_int64();
			destination.y = destinationJson["y"].as_int64();

			promotion = util::getPieceTypeFromString(json.at("promotion").as_string().c_str());
		}

		json::object MakeMoveRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			data["player"] = util::toString(player);

			json::object sourceJson;
			sourceJson["x"] = source.x;
			sourceJson["y"] = source.y;
			data["source"] = sourceJson;

			json::object destinationJson;
			destinationJson["x"] = destination.x;
			destinationJson["y"] = destination.y;
			data["destination"] = destinationJson;

			data["promotion"] = util::toString(promotion);

			result["data"] = data;

			return result;
		}
	}
}
