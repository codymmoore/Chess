import { useState } from 'react';
import { Color, PieceType } from '../../common/enums';
import { Position } from '../../common/types';
import './ChessPiece.css';
import { getPieceImage } from '../../common/util';

export interface PieceProps {
    color: Color;
    type: PieceType;
    position: Position;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

export default function ChessPiece(props: PieceProps) {
    const [color, setColor] = useState(props.color);
    const [type, setType] = useState(props.type);
    const [position, setPosition] = useState(props.position);

    return (
        <div className="piece" onClick={() => { alert(`Piece: { color: ${color}, type: ${type}, position: ${toString(position)}`); }}>
            <img src={getPieceImage(color, type)} width='105 % ' height='105 %' />
        </div>
    );
}