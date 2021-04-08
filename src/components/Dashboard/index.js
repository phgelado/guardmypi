import React, { useRef, useState } from "react"
import { useAuth } from "../../contexts/AuthContext"
import { Link, useHistory } from "react-router-dom"

import {
Form,
FormWrap,
FormLabel,
FormInput,
FormH1,
FormButton,
FormH2,
Container,
Box,
Text,
Icon,
Alert,
FormContent} from './SigninElements'

import {Button, ButtonRouter} from '../ButtonElement';

export default function Dashboard() {
  const [error, setError] = useState("")
  const { currentUser, logout } = useAuth()
  const history = useHistory()

  async function handleLogout() {
    setError("")

    try {
      await logout()
      history.push("/signin")
    } catch {
      setError("Failed to log out")
    }
  }

    return (
        <>
        <Container>
            <FormWrap>
                <Icon to ="/">GuardMyPi</Icon>
                <FormContent>
                <Box>
                    {currentUser && <FormH1>Profile</FormH1>}
                    {error && <Alert>{error}</Alert>}
                    {currentUser && <FormH2>Email: {currentUser.email}</FormH2>}
                    {currentUser && <FormButton><Link to ="/update-profile" style={{textDecoration: 'none', color: '#ffff'}}>Update Profile</Link></FormButton>}
                    {currentUser && <FormButton onClick={handleLogout}><Link to ="/signin" style={{textDecoration: 'none', color: '#ffff'}}>Log Out</Link></FormButton>}
                    {currentUser==null && <FormButton onClick={handleLogout}><Link to ="/signin" style={{textDecoration: 'none', color: '#ffff'}}>Sign In</Link></FormButton>}
                  </Box>
            </FormContent>
            </FormWrap>
        
        </Container> 
    </>
    )
        
}
