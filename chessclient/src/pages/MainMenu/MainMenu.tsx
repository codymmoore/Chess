import React from 'react';
import { useNavigate } from 'react-router-dom';
import { Button, getWebSocketManager } from './../../components';
import chessTitle from '../../assets/chessTitle.png';
import { StartGameRequest } from '../../websocket/message';
import './MainMenu.css';
import { GameType } from '../../common/enums';

const buttonStyle: React.CSSProperties = {
    width: '75%',
    height: '70px',
    margin: 'auto',
};

function applyBackground() {
    document.body.style.backgroundImage = 'linear-gradient(45deg, #000 25%, transparent 25%, transparent 75%, #000 75%, #000), linear-gradient(45deg, #000 25%, transparent 25%, transparent 75%, #000 75%, #000)';
    document.body.style.backgroundSize = '320px 320px';
    document.body.style.backgroundPosition = '0 0, 160px 160px';
    document.body.style.backgroundColor = '#dedede';
    document.body.style.backdropFilter = 'blur(5px)';
}

export default function MainMenu() {
    const webSocketManager = getWebSocketManager();
    webSocketManager.send(new StartGameRequest({ gameType: GameType.HumanVsAi }));
    const navigate = useNavigate();

    function startGame(gameType: GameType) {
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
                onClick={() => startGame(GameType.AiVsAii)}
                style={buttonStyle}
            />
        </div>
    );
}