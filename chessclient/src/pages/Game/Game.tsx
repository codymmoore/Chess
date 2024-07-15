import { useState } from 'react';
import { useParams, useLocation } from 'react-router-dom';
import { ChessBoard } from '../../components';
import { Piece, PiecePayload } from '../../common/types';
import { BOARD_WIDTH, BOARD_HEIGHT } from '../../common/constants';
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
    for (let piece of pieces) {
        let position = piece.position;
        board[position.y][position.x] = {
            color: piece.color,
            type: piece.type
        };
    }

    return board;
}

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


    applyBackground();
    return (
        <div>
            <h1>{gameType}</h1>
            <ChessBoard board={board} />
        </div>
    );
}