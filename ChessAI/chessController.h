#pragma once

namespace websocket
{
	namespace dto
	{
		struct GetValidMovesRequest;
		struct GetValidMovesResponse;

		struct MakeMoveRequest;
		struct MakeMoveResponse;
	}
}

class ChessState;

/**
 * Controller used perform operations on a chess state.
 */
class ChessController
{
public:
	/**
	 * Creates a new instance of ChessController.
	 *
	 * \param chessState The game state to be manipulated
	 */
	ChessController(ChessState& chessState);

	/**
	 * Retrieves valid moves for a piece.
	 *
	 * \param request The request object containing piece data
	 * \return The response object containing valid moves
	 */
	websocket::dto::GetValidMovesResponse getValidMoves(const websocket::dto::GetValidMovesRequest& request);

	/**
	 * Moves a piece.
	 *
	 * \param request The request object containing move data
	 * \return The response object containing updated game information
	 */
	websocket::dto::MakeMoveResponse makeMove(const websocket::dto::MakeMoveRequest& request);

private:
	ChessState& _chessState;
};
