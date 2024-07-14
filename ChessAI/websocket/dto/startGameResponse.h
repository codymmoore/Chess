#pragma once

#include "message.h"
#include <vector>

namespace boost
{
	namespace json
	{
		class object;
	}
}

namespace websocket
{
	namespace dto
	{
		/**
		 * Message used to respond to the request of a start of a new game.
		 */
		struct StartGameResponse : Message
		{
			std::vector<PiecePayload> pieces;

			StartGameResponse() = default;

			/**
			 * Creates a new instance StartGameResponse and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			StartGameResponse(const boost::json::object& json);

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
