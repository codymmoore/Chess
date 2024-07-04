import { PieceType, Color } from './enums'

export type Piece = {
    type: PieceType;
    color: Color;
}

export type Position = {
    x: number;
    y: number;
};

export type PieceData = {
    type: PieceType;
    position: Position;
};