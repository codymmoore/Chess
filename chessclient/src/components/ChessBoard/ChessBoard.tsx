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
 * @param param0 Properties used to render the chess board
 * @returns The chess board React node
 */
export default function ChessBoard({ board, makeMove }: ChessBoardProps) {
    return (
        <div className='chessboard-outer-wrapper'>
            <div className="chessboard">
                {board.map((row, y) =>
                    row.map((piece, x) => (
                        <Square
                            key={`${y}-${x}`}
                            color={(y + x) % 2 === 1 ? 'black' : 'white'}
                            position={{ x: x, y: y }}
                            piece={piece}
                            makeMove={makeMove}
                        />
                    ))
                )}
            </div>
        </div>
    );
}