#include "chessController.h"

#include "websocket/dto/startGameRequest.h"
#include "websocket/dto/startGameResponse.h"
#include "websocket/dto/getValidMovesRequest.h"
#include "websocket/dto/getValidMovesResponse.h"
#include "websocket/dto/makeMoveRequest.h"
#include "websocket/dto/makeMoveResponse.h"
#include "websocket/dto/endGameRequest.h"
#include "websocket/dto/endGameResponse.h"

#include "chess.h"
#include "move.h"

using namespace websocket::dto;

ChessController::ChessController(ChessState& chessState) : _chessState(chessState)
{
}

StartGameResponse ChessController::startGame(const StartGameRequest& request)
{
	_chessState.reset();

	StartGameResponse response;
	response.whitePieces = _chessState.getWhitePieces();
	response.blackPieces = _chessState.getBlackPieces();

	return response;
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

EndGameResponse ChessController::endGame(const EndGameRequest& request)
{
	_chessState.clear();

	EndGameResponse response;

	return response;
}
