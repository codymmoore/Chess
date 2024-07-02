#pragma once

#include "message.h"
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace websocket
{
	/**
	 * Manages a WebSocket connection.
	 */
	class WebSocketManager
	{
	public:
		/**
		 * Create a new WebSocketManager instance and awaits a connection.
		 *
		 * \param endpoint The endpoint used to connect to the WebSocket server
		 */
		WebSocketManager(const boost::asio::ip::tcp::endpoint& endpoint);

		/**
		 * Destroys the calling object and closes the connection.
		 *
		 */
		~WebSocketManager();

		/**
		 * Retrieves a message from the client.
		 *
		 * \return The client-sent message
		 */
		std::unique_ptr<dto::Message> read();

		/**
		 * Send a message to the client.
		 *
		 * \param message The message to be sent to the client
		 */
		void write(const dto::Message& message);

	private:
		boost::asio::ip::tcp::endpoint _endpoint;
		boost::asio::io_context _ioContext{};
		boost::asio::ip::tcp::socket _socket;
		boost::asio::ip::tcp::acceptor _acceptor;
		boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _webSocketStream;
	};
}
