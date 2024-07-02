#include "chessServer.h"

using tcp = boost::asio::ip::tcp;

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
	catch (const std::exception& e)
	{
		std::cerr << "Unexpected exception occurred: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}