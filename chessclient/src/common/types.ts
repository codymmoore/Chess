import { PieceType, Color } from './enums'

export type Piece = {
    type: PieceType;
    color: Color;
}

export type Position = {
    x: number;
    y: number;
};

export type PiecePayload = {
    color: Color;
    type: PieceType;
    position: Position;
};