#pragma once

#include "message.h"
#include "../../util/position.h"
#include "../../chess.h"

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
		 * Message used to request a move.
		 */
		struct MakeMoveRequest : Message
		{
			PiecePayload piece;
			util::Position destination;
			PieceType promotion;

			MakeMoveRequest() = default;

			/**
			 * Creates a new instance MakeMoveRequest and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			MakeMoveRequest(const boost::json::object& json);

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
