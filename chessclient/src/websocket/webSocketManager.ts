import { Message } from './message';

/**
* Manages a WebSocket connection.
*/
export class WebSocketManager {
    private readonly webSocket: WebSocket;

    /**
     * Creates a new WebSocketManager instance and attempts to connect to a WebSocket server.
     * 
     * @param endpoint The endpoint of the WebSocket server to connect to
     */
    constructor(endpoint: string, onOpen: ((event: Event) => void) | null = null) {
        this.webSocket = new WebSocket(endpoint);

        if (onOpen)
            this.webSocket.onopen = onOpen;
    }

    /**
     * Sets the function that will be executed upon receiving a message from the WebSocket connection.
     * 
     * @param callback The function that will be executed
     */
    setOnMessage(callback: (event: MessageEvent) => void): void {
        this.webSocket.onmessage = callback;
    }

    /**
     * Sets the function that will be executed upon receiving an error from the WebSocket connection.
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
     * Sends a message through the WebSocket connection.
     * 
     * @param message The message to be sent
     */
    send(message: Message): void {
        this.webSocket.send(message.toJson());
    }

    /**
     * Closes the WebSocket connection.
     */
    close(): void {
        this.webSocket.close();
    }
}