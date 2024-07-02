#pragma once

#include "message.h"
#include "utility.h"

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
		 * Messasge used to update the client.
		 */
		struct UpdateClientRequest : Message
		{
			Position source, destination;
			PieceType promotion;
			Color nextTurn, winner;

			UpdateClientRequest() = default;

			/**
			 * Creates a new instance UpdateClientRequest and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			UpdateClientRequest(const boost::json::object& json);

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
