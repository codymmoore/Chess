import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { MainMenu, Game } from './pages';
import './App.css';

export default function App() {
    return (
        <BrowserRouter>
            <Routes>
                <Route path='/' element={<MainMenu />} />
                <Route path='/play/:gameType' element={<Game />} />
            </Routes>
        </BrowserRouter>
    );
}
