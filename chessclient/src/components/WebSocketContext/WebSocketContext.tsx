import React, { useState, useContext } from 'react';
import { WebSocketManager } from '../../websocket/webSocketManager';

const SERVER_URL = "ws://localhost:8080";
const WebSocketContext = React.createContext<WebSocketManager | null>(null);

/**
 * Provides WebSocketManager for child components.
 * 
 * @returns WebSocketProvider element
 */
export function WebSocketProvider({ children }) {
    const [webSocketManager] = useState(new WebSocketManager(SERVER_URL));


    return (
        <WebSocketContext.Provider value={webSocketManager}>
            {children}
        </WebSocketContext.Provider>
    );
}

export function getWebSocketManager() {
    const context = useContext(WebSocketContext);

    return context;
}