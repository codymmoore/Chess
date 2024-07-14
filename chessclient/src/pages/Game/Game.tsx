import { useParams } from 'react-router-dom';
import { ChessBoard } from './../../components';
import './Game.css';

export type GameType = 'human-vs-ai' | 'ai-vs-ai';

function applyBackground() {
    document.body.style.backgroundImage = 'radial-gradient(circle, #434343, #000000)';
    document.body.style.backgroundSize = 'cover';
}

export default function Game() {
    const { gameType } = useParams();
    applyBackground();
    return (
        <div>
            <h1>{gameType}</h1>
            <ChessBoard />
        </div>
    );
}