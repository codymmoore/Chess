import { Color, PieceType } from './enums';
import { Piece } from './types';

export const BOARD_WIDTH = 8, BOARD_HEIGHT = 8;

export const PIECE_TO_SYMBOL_MAP = {
    [Color.White]: {
        [PieceType.Pawn]: 'P',
        [PieceType.Knight]: 'N',
        [PieceType.Bishop]: 'B',
        [PieceType.Rook]: 'R',
        [PieceType.Queen]: 'Q',
        [PieceType.King]: 'K',
    },
    [Color.Black]: {
        [PieceType.Pawn]: 'p',
        [PieceType.Knight]: 'n',
        [PieceType.Bishop]: 'b',
        [PieceType.Rook]: 'r',
        [PieceType.Queen]: 'q',
        [PieceType.King]: 'k',
    }
};

export const SYMBOL_TO_PIECE_MAP: { [symbol: string]: Piece | null; } = {
    'P': { color: Color.White, type: PieceType.Pawn },
    'N': { color: Color.White, type: PieceType.Knight },
    'B': { color: Color.White, type: PieceType.Bishop },
    'R': { color: Color.White, type: PieceType.Rook },
    'Q': { color: Color.White, type: PieceType.Queen },
    'K': { color: Color.White, type: PieceType.King },
    'p': { color: Color.Black, type: PieceType.Pawn },
    'n': { color: Color.Black, type: PieceType.Knight },
    'b': { color: Color.Black, type: PieceType.Bishop },
    'r': { color: Color.Black, type: PieceType.Rook },
    'q': { color: Color.Black, type: PieceType.Queen },
    'k': { color: Color.Black, type: PieceType.King },
    ' ': null
};