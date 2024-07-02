#include "WebSocketManager.h"
#include <boost/beast/core.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
namespace json = boost::json;


namespace websocket
{
	WebSocketManager::WebSocketManager(const tcp::endpoint& endpoint) :
		_endpoint(endpoint),
		_socket(_ioContext),
		_acceptor(_ioContext, _endpoint),
		_webSocketStream(_acceptor.accept())
	{
		_webSocketStream.accept();
	}

	WebSocketManager::~WebSocketManager()
	{
		_webSocketStream.close(beast::websocket::normal);
	}

	std::unique_ptr<dto::Message> WebSocketManager::read()
	{
		beast::flat_buffer buffer;
		_webSocketStream.read(buffer);

		const std::string messageString = beast::buffers_to_string(buffer.data());
		const json::object messageJson = json::parse(messageString).as_object();

		return dto::Message::createMessage(messageJson);
	}

	void WebSocketManager::write(const dto::Message& message)
	{
		const std::string messageString = json::serialize(message.toJson());
		_webSocketStream.write(net::buffer(messageString));
	}
}