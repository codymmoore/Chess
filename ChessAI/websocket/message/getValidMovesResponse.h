#pragma once

#include "message.h"
#include <vector>

struct Position;

namespace boost
{
	namespace json
	{
		class object;
	}
}

namespace websocket
{
	namespace message
	{
		/**
		 * Messge used to respond to a valid moves request.
		 */
		struct GetValidMovesResponse : Message
		{
			std::vector<util::Position> moves;

			GetValidMovesResponse() = default;

			/**
			 * Creates a new instance of GetValidMovesResponse and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			GetValidMovesResponse(const boost::json::object& json);

			MessageType getMessageType() const override;

			/**
			 * Populates the values of the calling object using a JSON object.
			 *
			 * \param json The JSON object used to populate the message
			 */
			void fromJson(const boost::json::object& json) override;

			/**
			 * Generates a JSON object based on the calling object.
			 *
			 * \return The resulting JSON object
			 */
			boost::json::object toJson() const override;
		};
	}
}
