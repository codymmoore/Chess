import { memo, useState } from 'react';
import { useDrop } from 'react-dnd';
import { Piece, Position } from '../../common/types';
import { DraggableItem, Color, PieceType } from '../../common/enums';
import ChessPiece, { ChessPieceProps } from '../ChessPiece/ChessPiece';
import './Square.css';

export interface SquareProps {
    color: 'white' | 'black';
    position: Position;
    piece: Piece | null;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

const Square = memo(function Square(props: SquareProps) {
    const [piece, setPiece] = useState(props.piece);
    const [{ isOver }, dropRef] = useDrop({
        accept: DraggableItem.ChessPiece,
        drop: (item: ChessPieceProps) => { setPiece({ color: item.color, type: item.type }); }
    });

    return (
        <div ref={dropRef} className={`square ${props.color}`} onClick={() => { alert(`${toString(props.position)}`); }}>
            {piece &&
                <ChessPiece
                    color={piece.color}
                    type={piece.type}
                    position={props.position}
                />
            }
        </div>
    );
});

export default Square;