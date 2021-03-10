import styled from 'styled-components'
import {Link as LinkS} from 'react-scroll'
import {Link as LinkR} from 'react-router-dom'

export const Button = styled(LinkS)`
    border-radius: 50px;
    background: ${({primary}) => (primary ? 'red' : 'red')};
    white-space: nowrap;
    padding: ${({big}) => (big ? '14px 48px' : '12px 30px')};
    color: ${({dark}) => (dark ? '#fff' : '#010606')};
    font-size: ${({fontBig}) => (fontBig ? '20px' : '16px')};
    outline: none;
    border: none;
    cursor: pointer;
    display: flex;
    justify-content: center;
    align-items: center;
    transition: all 0.2s ease-in-out;

    &:hover {
        transition: all 0.2s ease-in-out;
        background: ${({ primary }) => (primary ? 'red' : 'red')};
        color: ${({dark}) => (dark ? '#010606' : '#fff')};
    }

`;

export const ButtonRouter = styled(LinkR)`
    border-radius: 50px;
    background: ${({primary}) => (primary ? 'red' : 'red')};
    white-space: nowrap;
    padding: ${({big}) => (big ? '14px 48px' : '12px 30px')};
    color: ${({dark}) => (dark ? '#fff' : '#010606')};
    font-size: ${({fontBig}) => (fontBig ? '20px' : '16px')};
    outline: none;
    border: none;
    cursor: pointer;
    display: flex;
    justify-content: center;
    align-items: center;
    transition: all 0.2s ease-in-out;
    text-decoration: none;

    &:hover {
        transition: all 0.2s ease-in-out;
        background: ${({ primary }) => (primary ? 'red' : 'red')};
        color: ${({dark}) => (dark ? '#010606' : '#fff')};
    }

`;