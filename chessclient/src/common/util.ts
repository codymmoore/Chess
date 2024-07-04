import * as images from '../assets/pieces';
import { Piece } from '../common/types';
import { Color } from '../common/enums'

export function getPieceImage(piece: Piece): string {
    let image = '';

    switch (piece.type) {
        case 'PAWN':
            image = piece.color === Color.White ? images.whitePawn : images.blackPawn;
            break;
        case 'KNIGHT':
            image = piece.color === Color.White ? images.whiteKnight : images.blackKnight;
            break;
        case 'BISHOP':
            image = piece.color === Color.White ? images.whiteBishop : images.blackBishop;
            break;
        case 'ROOK':
            image = piece.color === Color.White ? images.whiteRook : images.blackRook;
            break;
        case 'QUEEN':
            image = piece.color === Color.White ? images.whiteQueen : images.blackQueen;
            break;
        case 'KING':
            image = piece.color === Color.White ? images.whiteKing : images.blackKing;
            break;
    }

    return image;
}