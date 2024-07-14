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

	for (const PieceNode& piece : _chessState.getWhitePieces())
	{
		PiecePayload piecePayload;
		piecePayload.color = Color::WHITE;
		piecePayload.type = piece.m_pieceType;
		piecePayload.position = piece.m_position;
		response.pieces.push_back(piecePayload);
	}

	for (const PieceNode& piece : _chessState.getBlackPieces())
	{
		PiecePayload piecePayload;
		piecePayload.color = Color::BLACK;
		piecePayload.type = piece.m_pieceType;
		piecePayload.position = piece.m_position;
		response.pieces.push_back(piecePayload);
	}

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
