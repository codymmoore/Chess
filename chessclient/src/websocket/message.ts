import { MessageType, GameType, Color, PieceType } from '../common/enums';
import { PiecePayload, Position } from '../common/types'
import { MessageError } from '../common/errors';

/**
 * Base type for all messages sent and received from WebSocket connection.
 */
export abstract class Message {
    messageType: MessageType;

    /**
     * Creates a new instance of Message.
     * 
     * @param messageType The type of message being created
     */
    constructor(messageType: MessageType) {
        this.messageType = messageType;
    }

    /**
     * Populates the message data using a JSON string.
     * 
     * @param json The JSON string containing message data
     */
    abstract fromJson(json: string): void;

    /**
     * Generates a JSON string for the message.
     * 
     * @returns JSON string
     */
    toJson(): string {
        return JSON.stringify({
            type: this.messageType,
            data: this.getData()
        });
    }

    /**
     * Retrieves the data for a message.
     * 
     * @returns object containing message data
     */
    protected abstract getData(): object;
}

/**
* Request to start a new game.
*/
export class StartGameRequest extends Message {
    gameType: GameType | null = null;

    /**
     * Creates an instance of StartGameRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: { gameType: GameType } | null = null) {
        super(MessageType.StartGameRequest);

        if (data) {
            this.gameType = data.gameType;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.gameType = data['gameType'] as GameType;
    }

    getData(): object {
        return {
            gameType: this.gameType
        };
    }
}

/**
 * Response to StartGameRequest.
 */
export class StartGameResponse extends Message {
    pieces: PiecePayload[] | null = null;

    /**
     * Creates an instance of StartGameResponse.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: { pieces: PiecePayload[] } | null = null) {
        super(MessageType.StartGameResponse);

        if (data) {
            this.pieces = data.pieces;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.pieces = data['pieces'];
    }

    getData(): object {
        return {
            pieces: this.pieces
        };
    }
}

/**
 * Request to get valid moves for a piece.
 */
export class GetValidMovesRequest extends Message {
    piece: PiecePayload | null = null;

    /**
     * Creates an instance of GetValidMovesRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: { piece: PiecePayload } | null = null) {
        super(MessageType.GetValidMovesRequest);

        if (data) {
            this.piece = data.piece;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.piece = data['piece'];
    }

    getData(): object {
        return {
            piece: this.piece
        };
    }
}

/**
 * Response to GetValidMovesRequest.
 */
export class GetValidMovesResponse extends Message {
    moves: Position[] | null = null;

    /**
     * Creates an instance of GetValidMovesResponse.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: { moves: Position[] } | null = null) {
        super(MessageType.GetValidMovesResponse);

        if (data) {
            this.moves = data.moves;
        }
    }
    
    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.moves = data['moves'];
    }

    getData(): object {
        return {
            moves: this.moves
        };
    }
}

/**
 * Request to move a piece.
 */
export class MakeMoveRequest extends Message {
    piece: PiecePayload | null = null;
    destination: Position | null = null;
    promotion: PieceType | null = null;

    /**
     * Creates an instance of MakeMoveRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: {
        piece: PiecePayload,
        destination: Position,
        promotion: PieceType
    } | null = null) {
        super(MessageType.MakeMoveRequest);

        if (data) {
            this.piece = data.piece;
            this.destination = data.destination;
            this.promotion = data.promotion;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.piece = data['piece'];
        this.destination = data['destination'];
        this.promotion = data['promotion'] as PieceType;
    }

    getData(): object {
        return {
            piece: this.piece,
            destination: this.destination,
            promotion: this.promotion
        };
    }
}

/**
 * Response to MakeMoveRequest.
 */
export class MakeMoveResponse extends Message {
    success: boolean | null = null;
    nextTurn: Color | null = null;
    winner: Color | null = null;

    /**
     * Creates an instance of MakeMoveResponse.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: { success: boolean, nextTurn: Color, winner: Color } | null = null) {
        super(MessageType.MakeMoveResponse);

        if (data) {
            this.success = data.success;
            this.nextTurn = data.nextTurn;
            this.winner = data.winner;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.success = data['success'];
        this.nextTurn = data['nextTurn'] as Color;
        this.winner = data['winner'] as Color;
    }

    getData(): object {
        return {
            success: this.success,
            nextTurn: this.nextTurn,
            winner: this.winner
        };
    }
}

/**
 * Request to update the client.
 */
export class UpdateClientRequest extends Message {
    source: Position | null = null;
    destination: Position | null = null;
    promotion: PieceType | null = null;
    nextTurn: Color | null = null;
    winner: Color | null = null;

    /**
     * Creates an instance of UpdateClientRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: {
        source: Position,
        destination: Position,
        promotion: PieceType,
        nextTurn: Color,
        winner: Color;
    } | null = null) {
        super(MessageType.UpdateClientRequest);

        if (data) {
            this.source = data.source;
            this.destination = data.destination;
            this.promotion = data.promotion;
            this.nextTurn = data.nextTurn;
            this.winner = data.winner;
        }
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];

        this.source = data['source'];
        this.destination = data['destination'];
        this.promotion = data['promotion'];
        this.nextTurn = data['nextTurn'];
        this.winner = data['winner'];
    }

    getData(): object {
        return {
            source: this.source,
            destination: this.destination,
            promotion: this.promotion,
            nextTurn: this.nextTurn,
            winner: this.winner
        };
    }
}

/**
 * Response to UpdateClientRequest.
 */
export class UpdateClientResponse extends Message {
    /**
     * Creates an instance of UpdateClientResponse.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: {} | null = null) {
        super(MessageType.UpdateClientResponse);
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];
    }

    getData(): object {
        return {};
    }
}

/**
 * Request to end the current chess game.
 */
export class EndGameRequest extends Message {
    /**
     * Creates an instance of EndGameRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: {} | null = null) {
        super(MessageType.EndGameRequest);
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];
    }

    getData(): object {
        return {};
    }
}

/**
 * Response the EndGameRequest.
 */
export class EndGameResponse extends Message {
    /**
     * Creates an instance of EndGameRequest.
     * 
     * @param data The data used to populate the message
     */
    constructor(data: {} | null = null) {
        super(MessageType.EndGameResponse);
    }

    fromJson(json: string): void {
        const data = JSON.parse(json)['data'];
    }

    getData(): object {
        return {};
    }
}

/**
 * Creates a Message based on a JSON string.
 * 
 * @param json The string used to create the Message object
 * @returns New instance of Message
 */
export function createMessage(json: string): Message {
    const jsonObject = JSON.parse(json);
    const data = jsonObject.data;

    if (!jsonObject['type'])
        throw new MessageError('Missing message type');

    switch (jsonObject['type'] as MessageType) {
        case MessageType.StartGameRequest:
            return new StartGameRequest(data);
        case MessageType.StartGameResponse:
            return new StartGameResponse(data);
        case MessageType.GetValidMovesRequest:
            return new GetValidMovesRequest(data);
        case MessageType.GetValidMovesResponse:
            return new GetValidMovesResponse(data);
        case MessageType.MakeMoveRequest:
            return new MakeMoveRequest(data);
        case MessageType.MakeMoveResponse:
            return new MakeMoveResponse(data);
        case MessageType.UpdateClientRequest:
            return new UpdateClientRequest(data);
        case MessageType.UpdateClientResponse:
            return new UpdateClientResponse(data);
        case MessageType.EndGameRequest:
            return new EndGameRequest(data);
        case MessageType.EndGameResponse:
            return new EndGameResponse(data);
        default:
            throw new MessageError(`Unsupported message type: ${jsonObject['type']}`);
    }
}