#include "chessServer.h"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main(int arc, char* argv[])
{
	try
	{
		while (true)
		{
			const tcp::endpoint endpoint(tcp::v4(), 8080);
			ChessServer chessServer(endpoint);
			chessServer.run();
		}
	}
	catch (const boost::system::system_error& e)
	{
		if (e.code() == asio::error::connection_aborted)
		{
			std::cerr << "Connection aborted" << std::endl;
		}
		else
		{
			std::cerr << "Unexpected error from Boost: " << e.code().message() << std::endl;
		}
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected exception occurred: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}