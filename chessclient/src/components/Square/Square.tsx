import { useState } from 'react';
import { Piece, Position } from '../../common/types';
import '../Square/Square.css';
import { getPieceImage } from '../../common/util';

export interface SquareProps {
    color: 'white' | 'black';
    position: Position;
    piece: Piece | null;
}

function toString(position: Position): string {
    return `Position: { x: ${position.x}, y: ${position.y} }`;
}

function pieceToString(piece: Piece | null): string {
    if (piece == null) {
        return 'Piece: {}';
    } else {
        return `Piece: { type: ${piece.type}, color: ${piece.color} }`;
    }
}

export default function Square(props: SquareProps) {
    const [piece, setPiece] = useState(props.piece);

    return (
        <div className={`square ${props.color}`} onClick={() => { alert(`${toString(props.position) + ', ' + pieceToString(piece)}`); }}>
            {piece && (<img src={getPieceImage(piece)} width='105%' height='105%' />)}
        </div>
    );
}