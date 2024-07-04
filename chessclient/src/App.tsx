//import React from 'react';
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { MainMenu, Game } from './pages';
import './App.css';
//import { WebSocketManager } from './websocket/webSocketManager';
import { WebSocketProvider } from './components/WebSocketContext/WebSocketContext';

export default function App() {
    return (
        <WebSocketProvider>
            <BrowserRouter>
                <Routes>
                    <Route path='/' element={<MainMenu />} />
                    <Route path='/play/:gameType' element={<Game />} />
                </Routes>
            </BrowserRouter>
        </WebSocketProvider>
    );
}
