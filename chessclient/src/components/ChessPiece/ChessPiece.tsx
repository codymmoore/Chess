import { useEffect } from 'react';
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

/**
 * React component used to render a chess piece.
 * 
 * @param param0 Properties used to render the chess piece
 * @returns The chess piece React node
 */
export default function ChessPiece({ color, type, position }: ChessPieceProps) {
    const [{ isDragging }, dragRef, preview] = useDrag({
        type: DraggableItem.ChessPiece,
        item: { color, type, position },
        collect: (monitor) => ({
            isDragging: monitor.isDragging()
        })
    });

    // Solves issues with background being dragged
    useEffect(() => {
        const dragPreviewImage = new Image();
        dragPreviewImage.src = "";
        preview(dragPreviewImage);
    });

    return (
        <img ref={dragRef} src={getPieceImage(color, type)} className='chess-piece' />
    );
}