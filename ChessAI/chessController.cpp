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
#include "move.h"
#include <memory>

using namespace websocket::message;

ChessController::ChessController(ChessState& chessState) : _chessState(chessState)
{
}

StartGameResponse ChessController::startGame(const StartGameRequest& request)
{
	_chessState.reset();

	StartGameResponse response;
	response.board = std::make_unique<const BitBoard>(_chessState.getBoard());
	response.nextTurn = _chessState.getNextTurn();
	response.winner = _chessState.getWinner();

	return response;
}

GetValidMovesResponse ChessController::getValidMoves(const GetValidMovesRequest& request)
{
	GetValidMovesResponse response;

	PieceNode piece;
	piece.m_pieceType = request.piece.type;
	piece.m_position = request.piece.position;
	response.moves = Move::getValidMoves(request.piece.color, piece, _chessState);

	return response;
}

MakeMoveResponse ChessController::makeMove(const MakeMoveRequest& request)
{
	MakeMoveResponse response;
	PieceNode piece(request.piece.position, request.piece.type);

	if (Move::isValidMove(request.piece.color, piece, request.destination, _chessState))
	{
		Move::makeMove(request.piece.color, piece, request.destination, _chessState, request.promotion);
		response.success = true;
	}
	else
	{
		response.success = false;
	}
	response.board = std::make_unique<const BitBoard>(_chessState.getBoard());
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
