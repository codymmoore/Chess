import { Square } from '..';
import { Piece, Position } from '../../common/types';
import './ChessBoard.css';

interface ChessBoardProps {
    board: (Piece | null)[][];
    makeMove: (source: Position, destination: Position) => void;
}

/**
 * React component used to render a chess board.
 * 
 * @returns The chess board React node
 */
export default function ChessBoard({ board, makeMove }: ChessBoardProps) {
    return (
        <div className='chessboard-outer-wrapper'>
            <div className="chessboard">
                {board.map((row, y) =>
                    row.map((square, x) => (
                        <Square
                            key={`${y}-${x}`}
                            color={(y + x) % 2 === 1 ? 'black' : 'white'}
                            position={{ x: x, y: y }}
                            piece={square}
                            makeMove={makeMove}
                        />
                    ))
                )}
            </div>
        </div>
    );
}