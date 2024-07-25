#pragma once

#include "../../utility.h"

#include <string>
#include <memory>

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
		 * Stores piece information to be sent via WebSocket.
		 */
		struct PiecePayload
		{
			Color color;
			PieceType type;
			Position position;
		};

		/**
		 * Describes the different types of messages to be sent/received through a WebSocket connection.
		 */
		enum MessageType
		{
			START_GAME_REQUEST = 0,
			START_GAME_RESPONSE = 1,
			GET_VALID_MOVES_REQUEST = 2,
			GET_VALID_MOVES_RESPONSE = 3,
			MAKE_MOVE_REQUEST = 4,
			MAKE_MOVE_RESPONSE = 5,
			UPDATE_CLIENT_REQUEST = 6,
			UPDATE_CLIENT_RESPONSE = 7,
			END_GAME_REQUEST = 8,
			END_GAME_RESPONSE = 9
		};

		/**
		* Converts a string to a value of MessageType.
		*
		* \param messageTypeString The string to be converted
		* \return The corresponding MessageType value
		*/
		MessageType getMessageTypeFromString(const std::string& messageTypeString);

		/**
		 * Converts MessageType value to a string.
		 *
		 * \param messageType The MessageType value to be converted
		 * \return The corresponding string
		 */
		std::string toString(MessageType messageType);

		/**
		 * Base type for all messages sent and received from a WebSocket connection.
		 */
		struct Message
		{
			virtual MessageType getMessageType() const = 0;

			/**
			 * Populates the values of the calling object using a JSON object.
			 *
			 * \param json The JSON object used to populate the message
			 */
			virtual void fromJson(const boost::json::object& json) = 0;

			/**
			 * Generates a JSON object based on the calling object.
			 *
			 * \return The resulting JSON object
			 */
			virtual boost::json::object toJson() const;

			/**
			* Creates the appropriate Message object from a JSON object.
			*
			* \param json The JSON object used to create the message
			* \return A pointer to the newly created Message
			*/
			static std::unique_ptr<Message> createMessage(const boost::json::object& json);
		};
	}
}
