#pragma once

#include "message.h"
#include "../../utility.h"

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
		 * Message used to respond to a make move request.
		 */
		struct MakeMoveResponse : Message
		{
			Color nextTurn, winner;

			MakeMoveResponse() = default;

			/**
			 * Creates a new instance MakeMoveResponse and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			MakeMoveResponse(const boost::json::object& json);

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
