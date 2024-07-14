import { BrowserRouter, Routes, Route } from 'react-router-dom';

import { MainMenu, Game } from './pages';
import './App.css';
import { useWebSocketContext } from './contexts/WebSocketContext';
import { useMessageQueueContext } from './contexts/MessageQueueContext';
import { createMessage } from './websocket/message';

export default function App() {
    const webSocketManager = useWebSocketContext();
    const messageQueue = useMessageQueueContext();

    webSocketManager.setOnMessage((event) => {
        messageQueue.enqueue(createMessage(event.data));
    })

    return (
        <BrowserRouter>
            <Routes>
                <Route path='/' element={<MainMenu />} />
                <Route path='/play/:gameType' element={<Game />} />
            </Routes>
        </BrowserRouter>
    );
}
