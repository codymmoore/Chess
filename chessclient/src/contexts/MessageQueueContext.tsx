import { createContext, useContext, ReactNode } from 'react';
import MessageQueue from '../websocket/messageQueue';

const messageQueue = new MessageQueue();
const MessageQueueContext = createContext<MessageQueue>(messageQueue);

interface MessageQueueProviderProps {
    children: ReactNode
}

/**
 * React component used to provide a message queue to child components.
 * 
 * @param children The child components
 * @returns The message queue context provider React node
 */
export function MessageQueueProvider({ children }: MessageQueueProviderProps) {
    return (
        <MessageQueueContext.Provider value={messageQueue}>
            {children}
        </MessageQueueContext.Provider>
    );
}

/**
 * Provides the message queue context.
 * 
 * @returns The message queue context
 */
export function useMessageQueueContext(): MessageQueue {
    return useContext(MessageQueueContext);
}