#include "chessController.h"

#include "websocket/message/startGameRequest.h"
#include "websocket/message/startGameResponse.h"
#include "websocket/message/getValidMovesRequest.h"
#include "websocket/message/getValidMovesResponse.h"
#include "websocket/message/makeMoveRequest.h"
#include "websocket/message/makeMoveResponse.h"
#include "websocket/message/endGameRequest.h"
#include "websocket/message/endGameResponse.h"

#include "chess.h"
#include "move/moveUtil.h"
#include <memory>

using namespace websocket::message;

ChessController::ChessController(ChessState& chessState) : _chessState(chessState)
{
}

StartGameResponse ChessController::startGame(const StartGameRequest& request)
{
	_chessState.reset();

	StartGameResponse response;
	response.board = std::make_unique<const util::bitboard::BitboardSet>(_chessState.getBoard());
	response.nextTurn = _chessState.getNextTurn();
	response.winner = _chessState.getWinner();

	return response;
}

GetValidMovesResponse ChessController::getValidMoves(const GetValidMovesRequest& request)
{
	GetValidMovesResponse response;

	// TODO

	return response;
}

MakeMoveResponse ChessController::makeMove(const MakeMoveRequest& request)
{
	MakeMoveResponse response;

	if (move::isValidMove(request.player, request.source, request.destination, _chessState))
	{
		_chessState.update(request.player, request.source, request.destination, request.promotion);
		response.success = true;
	}
	else
	{
		response.success = false;
	}
	response.board = std::make_unique<const util::bitboard::BitboardSet>(_chessState.getBoard());
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
