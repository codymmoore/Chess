#include "updateClientResponse.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
	{
		UpdateClientResponse::UpdateClientResponse(const json::object& json)
		{
			fromJson(json);
		}

		MessageType UpdateClientResponse::getMessageType() const
		{
			static const MessageType MESSAGE_TYPE = MessageType::UPDATE_CLIENT_RESPONSE;
			return MESSAGE_TYPE;
		}

		void UpdateClientResponse::fromJson(const json::object& json)
		{
		}

		json::object UpdateClientResponse::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			result["data"] = data;

			return result;
		}
	}
}
