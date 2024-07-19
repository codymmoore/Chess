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
    makeMove: (source: Position, destination: Position) => void;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

const Square = memo(function Square(props: SquareProps) {
    const [{ isOver }, dropRef] = useDrop({
        accept: DraggableItem.ChessPiece,
        drop: (item: ChessPieceProps) => { props.makeMove(item.position, props.position); },
        collect: (monitor) => ({
            isOver: monitor.isOver()
        })
    });

    return (
        <div ref={dropRef} className={`square ${props.color} ${isOver ? 'highlight' : ''}`} onClick={() => { alert(`${toString(props.position)}`); }}>
            {props.piece &&
                <ChessPiece
                    color={props.piece.color}
                    type={props.piece.type}
                    position={props.position}
                />
            }
        </div>
    );
});

export default Square;