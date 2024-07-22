#pragma once

#include "message.h"

namespace boost
{
	namespace json
	{
		class object;
	}
}

enum GameType;

namespace websocket
{
	namespace dto
	{
		/**
		 * Message used to request the start of a new game.
		 */
		struct StartGameRequest : Message
		{
			GameType gameType;

			StartGameRequest() = default;

			/**
			 * Creates a new instance StartGameRequest and populates it using a JSON object.
			 *
			 * \param json The JSON object used to populate the new instance
			 */
			StartGameRequest(const boost::json::object& json);

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
