import React from 'react';
import './Button.css';

interface ButtonProps {
    label: string;
    onClick: () => void;
    disabled?: boolean;
    style?: React.CSSProperties;
}

export default function Button({ label, onClick, style }: ButtonProps) {
    return (
        <button className='button'
            onClick={onClick}
            style={{ ...style }}
        >
            {label}
        </button>
    );
}