#pragma once

namespace websocket
{
	namespace dto
	{
		struct StartGameRequest;
		struct StartGameResponse;

		struct GetValidMovesRequest;
		struct GetValidMovesResponse;

		struct MakeMoveRequest;
		struct MakeMoveResponse;

		struct EndGameRequest;
		struct EndGameResponse;
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
	 * Starts a new chess game.
	 *
	 * \param request The request object containing game information
	 * \return The response object containing new game information
	 */
	websocket::dto::StartGameResponse startGame(const websocket::dto::StartGameRequest& request);

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

	/**
	 * Ends the current game.
	 *
	 * \param request The request object containing end game information
	 * \return The response object containing post-game information
	 */
	websocket::dto::EndGameResponse endGame(const websocket::dto::EndGameRequest& request);

private:
	ChessState& _chessState;
};
