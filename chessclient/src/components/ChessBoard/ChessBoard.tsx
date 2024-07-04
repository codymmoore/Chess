import { Square } from '..';
import { PieceType, Color } from '../../common/enums';
import { Position } from '../../common/types';
import './ChessBoard.css';

const BOARD_WIDTH = 8, BOARD_HEIGHT = 8;

export function ChessBoard() {
    const board = [];

    for (let row = 0; row < BOARD_HEIGHT; row++) {
        const squares = [];
        for (let col = 0; col < BOARD_WIDTH; col++) {
            const isBlack = (row + col) % 2 === 1;
            const position: Position = { x: row, y: col };
            squares.push(<Square key={`${row}-${col}`} color={isBlack ? 'black' : 'white'} position={position} piece={{ type: PieceType.Pawn, color: Color.White }} />);
        }
        board.push(
            <div key={row}>
                {squares}
            </div>
        );
    }

    return <div className='chessboard-outer-wrapper'>
        <div className="chessboard">
            {board}
        </div>
    </div>;
}