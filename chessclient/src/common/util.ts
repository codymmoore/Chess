import * as images from '../assets/pieces';
import { Piece } from '../common/types';
import { Color, PieceType } from '../common/enums'

export function getPieceImage(pieceColor: Color, pieceType: PieceType) {
    let image = '';

    switch (pieceType) {
        case 'PAWN':
            image = pieceColor === Color.White ? images.whitePawn : images.blackPawn;
            break;
        case 'KNIGHT':
            image = pieceColor === Color.White ? images.whiteKnight : images.blackKnight;
            break;
        case 'BISHOP':
            image = pieceColor === Color.White ? images.whiteBishop : images.blackBishop;
            break;
        case 'ROOK':
            image = pieceColor === Color.White ? images.whiteRook : images.blackRook;
            break;
        case 'QUEEN':
            image = pieceColor === Color.White ? images.whiteQueen : images.blackQueen;
            break;
        case 'KING':
            image = pieceColor === Color.White ? images.whiteKing : images.blackKing;
            break;
    }

    return image;
}