import { createContext, useContext, ReactNode } from 'react';
import { WebSocketManager } from '../websocket/webSocketManager';

const endpoint = 'ws://localhost:8080';
const webSocketManager = new WebSocketManager(endpoint);
const WebSocketContext = createContext<WebSocketManager>(webSocketManager);

interface WebSocketProviderProps {
    children: ReactNode
}

/**
 * React component used to provide a WebSocket connection to child components.
 * 
 * @param children The child components
 * @returns The WebSocket context provider React node
 */
export function WebSocketProvider({ children }: WebSocketProviderProps) {
    return (
        <WebSocketContext.Provider value={webSocketManager}>
            {children}
        </WebSocketContext.Provider>
    );
}

/**
 * Provides the WebSocketContext.
 * 
 * @returns The WebSocket context
 */
export function useWebSocketContext(): WebSocketManager {
    return useContext(WebSocketContext);
}