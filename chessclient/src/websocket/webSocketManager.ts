import { Message, createMessage } from './message';
import { MessageType } from '../common/enums';
import { MessageError } from '../common/errors';

/**
* Manages a WebSocket connection.
*/
export class WebSocketManager {
    private readonly webSocket: WebSocket;
    private readonly messageListeners: Map<MessageType, (message: Message) => void> = new Map();

    /**
     * Creates a new WebSocketManager instance and attempts to connect to a WebSocket server.
     * 
     * @param endpoint The endpoint of the WebSocket server to connect to
     */
    constructor(endpoint: string, onOpen: ((event: Event) => void) | null = null) {
        this.webSocket = new WebSocket(endpoint);

        if (onOpen)
            this.webSocket.onopen = onOpen;

        this.webSocket.onmessage = (messageEvent) => {
            const message = createMessage(messageEvent.data);

            if (!this.messageListeners.has(message.type)) {
                throw new MessageError(`Unexpected message type: ${message.toJson()}`);
            }

            this.messageListeners.get(message.type)!(message);
        };
    }

    /**
     * Sends a message through the WebSocket connection.
     * 
     * @param message The message to be sent
     */
    send(message: Message): void {
        this.webSocket.send(message.toJson());
    }

    /**
     * Assigns a listener to handle a message type.
     * 
     * @param messageType The type of message to be handled by the listener
     * @param listener Function that will handle the messsage type
     */
    setMessageListener(messageType: MessageType, listener: (message: Message) => void) {
        this.messageListeners.set(messageType, listener);
    }

    /**
     * Sets the function that will be executed upon disconnecting due to an error.
     * 
     * @param callback The function that will be executed
     */
    setOnError(callback: (event: Event) => void): void {
        this.webSocket.onerror = callback;
    }

    /**
     * Sets the function that will be executed upon closing the WebSocket connection.
     * 
     * @param callback The function that will be executed
     */
    setOnClose(callback: (event: Event) => void): void {
        this.webSocket.onclose = callback;
    }

    /**
     * Closes the WebSocket connection.
     */
    close(): void {
        this.webSocket.close();
    }
}