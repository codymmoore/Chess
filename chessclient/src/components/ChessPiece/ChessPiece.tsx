import { useState, useEffect } from 'react';
import { useDrag } from 'react-dnd';
import { Color, PieceType, DraggableItem } from '../../common/enums';
import { Position } from '../../common/types';
import { getPieceImage } from '../../common/util';
import './ChessPiece.css';

export interface ChessPieceProps {
    color: Color;
    type: PieceType;
    position: Position;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

export default function ChessPiece(props: ChessPieceProps) {
    const [color, setColor] = useState(props.color);
    const [type, setType] = useState(props.type);
    const [position, setPosition] = useState(props.position);
    const [{ opacity }, dragRef, preview] = useDrag({
        type: DraggableItem.ChessPiece,
        item: { color, type, position },
        collect: (monitor) => ({
            opacity: 1,
        })
    });

    useEffect(() => {
        const dragPreviewImage = new Image();
        dragPreviewImage.src = "";
        preview(dragPreviewImage);
    });

    return (
        <div onClick={() => { alert(`Piece: { color: ${color}, type: ${type}, position: ${toString(position)}`); }}>
            <img ref={dragRef} src={getPieceImage(color, type)} className='chess-piece' />
        </div>
    );
}