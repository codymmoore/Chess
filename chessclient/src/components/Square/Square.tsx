import { useState } from 'react';
import { Piece, Coordinate } from '../../common/types';
import '../Square/Square.css';
import { getPieceImage } from '../../common/util';

export interface SquareProps {
    color: 'white' | 'black';
    coordinate: Coordinate;
    piece: Piece | null;
}

function coordToString(coordinate: Coordinate): string {
    return `Coordinate: { x: ${coordinate.x}, y: ${coordinate.y} }`;
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
        <div className={`square ${props.color}`} onClick={() => { alert(`${coordToString(props.coordinate) + ', ' + pieceToString(piece)}`); }}>
            {piece && (<img src={getPieceImage(piece)} width='105%' height='105%' />)}
        </div>
    );
}