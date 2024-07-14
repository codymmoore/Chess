import React from 'react';
import { useNavigate } from 'react-router-dom';

import './MainMenu.css';
import chessTitle from '../../assets/chessTitle.png';
import { Button } from './../../components';
import { useWebSocketContext } from '../../contexts/WebSocketContext';
import { StartGameRequest } from '../../websocket/message';
import { GameType } from '../../common/enums';

const buttonStyle: React.CSSProperties = {
    width: '75%',
    height: '70px',
    margin: 'auto',
};

/**
 * Applies a background to the main menu.
 */
function applyBackground() {
    document.body.style.backgroundImage = 'linear-gradient(45deg, #000 25%, transparent 25%, transparent 75%, #000 75%, #000), linear-gradient(45deg, #000 25%, transparent 25%, transparent 75%, #000 75%, #000)';
    document.body.style.backgroundSize = '320px 320px';
    document.body.style.backgroundPosition = '0 0, 160px 160px';
    document.body.style.backgroundColor = '#dedede';
    document.body.style.backdropFilter = 'blur(5px)';
}

/**
 * React component used to render the main menu for the chess client.
 * 
 * @returns The main menu React node
 */
export default function MainMenu() {
    const navigate = useNavigate();
    const webSocketContext = useWebSocketContext();

    function startGame(gameType: GameType) {
        webSocketContext.send(new StartGameRequest({ gameType: gameType }));
        navigate(`/play/${gameType}`);
    }

    applyBackground();

    return (
        <div className='main-menu'>
            <img src={chessTitle} />
            <Button
                label='Human vs. AI'
                onClick={() => startGame(GameType.HumanVsAi)}
                style={buttonStyle}
            />
            <Button
                label='AI vs. AI'
                onClick={() => startGame(GameType.AiVsAi)}
                style={buttonStyle}
            />
        </div>
    );
}