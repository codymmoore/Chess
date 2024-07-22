import React from 'react';
import ReactDOM from 'react-dom/client';
import { DndProvider } from 'react-dnd';
import { HTML5Backend } from 'react-dnd-html5-backend';
import App from './App.tsx';
import './index.css';
import { WebSocketProvider } from './contexts/WebSocketContext.tsx';
import { MessageQueueProvider } from './contexts/MessageQueueContext.tsx';

ReactDOM.createRoot(document.getElementById('root')!).render(
    <React.StrictMode>
        <DndProvider backend={HTML5Backend}>
            <WebSocketProvider>
                <App />
            </WebSocketProvider>
        </DndProvider>
    </React.StrictMode>,
);
