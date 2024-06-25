import * as images from '../assets/pieces';
import { Piece } from '../common/types';

export function getPieceImage(piece: Piece): string {
    let image = '';

    switch (piece.type) {
        case 'pawn':
            image = piece.color === 'white' ? images.whitePawn : images.blackPawn;
            break;
        case 'knight':
            image = piece.color === 'white' ? images.whiteKnight : images.blackKnight;
            break;
        case 'bishop':
            image = piece.color === 'white' ? images.whiteBishop : images.blackBishop;
            break;
        case 'rook':
            image = piece.color === 'white' ? images.whiteRook : images.blackRook;
            break;
        case 'queen':
            image = piece.color === 'white' ? images.whiteQueen : images.blackQueen;
            break;
        case 'king':
            image = piece.color === 'white' ? images.whiteKing : images.blackKing;
            break;
    }

    return image;
}