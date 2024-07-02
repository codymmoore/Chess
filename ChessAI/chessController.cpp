#include "chessController.h"

#include "startGameRequest.h"
#include "startGameResponse.h"
#include "getValidMovesRequest.h"
#include "getValidMovesResponse.h"
#include "makeMoveRequest.h"
#include "makeMoveResponse.h"
#include "endGameRequest.h"
#include "endGameResponse.h"

#include "chess.h"
#include "move.h"

using namespace websocket::dto;

ChessController::ChessController(ChessState& chessState) : _chessState(chessState)
{
}

GetValidMovesResponse ChessController::getValidMoves(const GetValidMovesRequest& request)
{
	GetValidMovesResponse response;
	response.moves = Move::getValidMoves(request.color, request.piece, _chessState);
	return response;
}

MakeMoveResponse ChessController::makeMove(const MakeMoveRequest& request)
{
	Move::makeMove(request.player, request.piece, request.destination, _chessState, request.promotion);

	MakeMoveResponse response;
	response.nextTurn = _chessState.getNextTurn();
	response.winner = _chessState.getWinner();

	return response;
}
