#pragma once

#include "message.h"
#include "chess.h"

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
		 * Message used to request valid moves for a piece.
		 */
		struct GetValidMovesRequest : Message
		{
			PieceNode piece;
			Color color;

			GetValidMovesRequest() = default;

			/**
			 * Creates a new instance of GetValidMovesRequest and populates it using the provided JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			GetValidMovesRequest(const boost::json::object& json);

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
