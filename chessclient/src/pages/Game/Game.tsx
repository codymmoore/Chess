import { useState, useRef } from 'react';
import { useLocation } from 'react-router-dom';
import { ChessBoard } from '../../components';
import { Piece, Position } from '../../common/types';
import { PieceType, MessageType, Color } from '../../common/enums';
import { BOARD_WIDTH, BOARD_HEIGHT, PIECE_TO_SYMBOL_MAP, SYMBOL_TO_PIECE_MAP } from '../../common/constants';
import { useWebSocketContext } from '../../contexts/WebSocketContext';
import { MakeMoveRequest, MakeMoveResponse, UpdateClientRequest, UpdateClientResponse } from '../../websocket/message';
import './Game.css';

export type GameType = 'human-vs-ai' | 'ai-vs-ai';

/**
 * Creates a new chess board from JSON.
 * 
 * @param boardJson The JSON representation of the board
 * @returns A new board representation
 */
function getBoardFromJson(boardJson: string[][]): (Piece | null)[][] {
    return boardJson.map(row => row.map(symbol => SYMBOL_TO_PIECE_MAP[symbol] || null));
}

/**
 * Applies a background to the game page.
 */
function applyBackground() {
    document.body.style.backgroundImage = 'radial-gradient(circle, #434343, #000000)';
    document.body.style.backgroundSize = 'cover';
}

/**
 * React component that renders a chess game.
 * 
 * @returns The React node for the chess game
 */
export default function Game() {
    const location = useLocation();
    const state = location.state;

    const [board, setBoard] = useState<(Piece | null)[][]>(getBoardFromJson(state.board));
    const [nextTurn, setNextTurn] = useState<Color>(state.nextTurn);
    const [winner, setWinner] = useState<Color>(state.winner);

    const prevBoard = useRef<(Piece | null)[][] | null>(null);
    const webSocketManager = useWebSocketContext();

    webSocketManager.setMessageListener(MessageType.MakeMoveResponse, (message) => {
        const response = message as MakeMoveResponse;
        if (!response.success) {
            setBoard(prevBoard.current!);
            prevBoard.current = null;
        }
        setBoard(getBoardFromJson(response.board!));
        setNextTurn(response.nextTurn!);
        setWinner(response.winner!);
    });

    /**
     * Moves a piece from one square to another.
     * 
     * @param source The original position of the piece
     * @param destination The new position of the piece
     */
    function movePiece(source: Position, destination: Position) {
        const piece = board[source.y][source.x];

        prevBoard.current = board;
        const newBoard = board.map((row) => { return row.slice(); });
        newBoard[source.y][source.x] = null;
        newBoard[destination.y][destination.x] = piece;
        setBoard(newBoard);
    }

    webSocketManager.setMessageListener(MessageType.UpdateClientRequest, (message) => {
        const request = message as UpdateClientRequest;
        setBoard(getBoardFromJson(request.board!));
        setNextTurn(request.nextTurn!);
        setWinner(request.winner!);
        webSocketManager.send(new UpdateClientResponse());

        if (request.nextTurn! === Color.Neutral) {
            let gameEndMessage = 'Winner: ';
            if (request.winner === Color.White) {
                gameEndMessage += 'White';
            } else if (request.winner === Color.Black) {
                gameEndMessage += 'Black';
            } else {
                gameEndMessage += 'Tie';
            }
            alert(gameEndMessage);
        }
    });

    /**
     * Moves a piece and send a request to update the server.
     * 
     * @param source The original position of the piecce
     * @param destination The new position of the piece
     */
    function makeMove(source: Position, destination: Position) {
        if (nextTurn === Color.White) {
            const piece = board[source.y][source.x];

            const request = new MakeMoveRequest({
                piece: {
                    color: piece!.color,
                    type: piece!.type,
                    position: source
                },
                destination: destination,
                promotion: PieceType.Queen
            });
            webSocketManager.send(request);

            movePiece(source, destination);
        }
    }

    applyBackground();
    return (
        <div className="game-container">
            <div className="game-info">
                <h2>Current turn:</h2>
                <h2>{nextTurn === Color.White ? 'White' : 'Black'}</h2>
            </div>
            <div className="chessboard-container">
                <ChessBoard board={board} makeMove={makeMove} />
            </div>
        </div>
    );
}