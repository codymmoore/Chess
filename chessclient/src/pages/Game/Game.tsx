import { useState, useRef } from 'react';
import { useParams, useLocation } from 'react-router-dom';
import { ChessBoard } from '../../components';
import { Piece, PiecePayload, Position } from '../../common/types';
import { PieceType, MessageType, Color } from '../../common/enums';
import { BOARD_WIDTH, BOARD_HEIGHT } from '../../common/constants';
import { useWebSocketContext } from '../../contexts/WebSocketContext';
import { MakeMoveRequest, MakeMoveResponse, UpdateClientRequest, UpdateClientResponse } from '../../websocket/message';
import './Game.css';

export type GameType = 'human-vs-ai' | 'ai-vs-ai';

/**
 * Creates and populates a chess board.
 * 
 * @param pieces The pieces on the board
 * @returns The newly created chess board
 */
function initializeBoard(pieces: PiecePayload[]) {
    const board = [];

    // Create board
    for (let y = 0; y < BOARD_HEIGHT; y++) {
        const row: (Piece | null)[] = [];
        for (let y = 0; y < BOARD_WIDTH; y++) {
            row.push(null);
        }
        board.push(row);
    }

    // Populate board
    for (const piece of pieces) {
        const position = piece.position;
        board[position.y][position.x] = {
            color: piece.color,
            type: piece.type
        };
    }

    return board;
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
    const { gameType } = useParams();
    const location = useLocation();
    const { pieces } = location.state;
    const [board, setBoard] = useState<(Piece | null)[][]>(initializeBoard(pieces));
    const [nextTurn, setNextTurn] = useState<Color>(Color.Neutral);
    const [winner, setWinner] = useState<Color>(Color.Neutral);
    let prevBoard = useRef<(Piece | null)[][] | null>(null);
    const webSocketManager = useWebSocketContext();

    webSocketManager.setMessageListener(MessageType.MakeMoveResponse, (message) => {
        const response = message as MakeMoveResponse;
        if (!response.success) {
            setBoard(prevBoard.current!);
            prevBoard.current = null;
        }
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
        movePiece(request.source!, request.destination!);
        setNextTurn(request.nextTurn!);
        setWinner(request.winner!);
        webSocketManager.send(new UpdateClientResponse());
    });

    /**
     * Moves a piece and send a request to update the server.
     * 
     * @param source The original position of the piecce
     * @param destination The new position of the piece
     */
    function makeMove(source: Position, destination: Position) {
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

    applyBackground();
    return (
        <div>
            <h1>{gameType}</h1>
            <ChessBoard board={board} makeMove={makeMove} />
        </div>
    );
}