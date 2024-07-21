import { useDrop } from 'react-dnd';
import { Piece, Position } from '../../common/types';
import { DraggableItem } from '../../common/enums';
import ChessPiece, { ChessPieceProps } from '../ChessPiece/ChessPiece';
import './Square.css';

export interface SquareProps {
    color: 'white' | 'black';
    position: Position;
    piece: Piece | null;
    makeMove: (source: Position, destination: Position) => void;
}

/**
 * React component used to render a square within a chess board.
 * 
 * @param param0 Properties used to render the square
 * @return The square React node
 */
export default function Square({ color, position, piece, makeMove }: SquareProps) {
    const [{ isOver }, dropRef] = useDrop({
        accept: DraggableItem.ChessPiece,
        drop: (item: ChessPieceProps) => { makeMove(item.position, position); },
        collect: (monitor) => ({
            isOver: monitor.isOver()
        })
    });

    return (
        <div ref={dropRef} className={`square ${color} ${isOver ? 'highlight' : ''}`}>
            {piece &&
                <ChessPiece
                    color={piece.color}
                    type={piece.type}
                    position={position}
                />
            }
        </div>
    );
}