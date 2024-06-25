export enum PieceType {
    Pawn = 'pawn',
    Knight = 'knight',
    Bishop = 'bishop',
    Rook = 'rook',
    Queen = 'queen',
    King = 'king'
}

export type Piece = {
    type: PieceType;
    color: 'white' | 'black';
}

export type Coordinate = {
    x: number;
    y: number;
}