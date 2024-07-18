export enum PieceType {
    Pawn = 'PAWN',
    Knight = 'KNIGHT',
    Bishop = 'BISHOP',
    Rook = 'ROOK',
    Queen = 'QUEEN',
    King = 'KING',
    None = 'NONE'
}

export enum Color {
    White = 'WHITE',
    Black = 'BLACK',
    Neutral = 'NEUTRAL'
}

export enum MessageType {
    StartGameRequest = 'START_GAME_REQUEST',
    StartGameResponse = 'START_GAME_RESPONSE',
    GetValidMovesRequest = 'GET_VALID_MOVES_REQUEST',
    GetValidMovesResponse = 'GET_VALID_MOVES_RESPONSE',
    MakeMoveRequest = 'MAKE_MOVE_REQUEST',
    MakeMoveResponse = 'MAKE_MOVE_RESPONSE',
    UpdateClientRequest = 'UPDATE_CLIENT_REQUEST',
    UpdateClientResponse = 'UPDATE_CLIENT_RESPONSE',
    EndGameRequest = 'END_GAME_REQUEST',
    EndGameResponse = 'END_GAME_RESPONSE'
}

export enum GameType {
    HumanVsAi = 'HUMAN_VS_AI',
    AiVsAi = 'AI_VS_AI'
}

export enum DraggableItem {
    ChessPiece = 'CHESS_PIECE'
}