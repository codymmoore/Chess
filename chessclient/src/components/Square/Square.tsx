import { memo } from 'react';
import { Piece, Position } from '../../common/types';
import ChessPiece from '../ChessPiece/ChessPiece';
import './Square.css';

export interface SquareProps {
    color: 'white' | 'black';
    position: Position;
    piece: Piece | null;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

const Square = memo(function Square({ color, position, piece }: SquareProps) {
    return (
        <div className={`square ${color}`} onClick={() => { alert(`${toString(position)}`); }}>
            {piece &&
                <ChessPiece
                    color={piece.color}
                    type={piece.type}
                    position={position}
                />
            }
        </div>
    );
});

export default Square;