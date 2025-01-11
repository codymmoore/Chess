#include "getValidMovesRequest.h"
#include <boost/json.hpp>

using namespace boost;

namespace websocket
{
	namespace message
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
			// TODO
		}

		json::object GetValidMovesRequest::toJson() const
		{
			json::object result = Message::toJson();

			json::object data;

			// TODO

			result["data"] = data;
			return result;
		}
	}
}
