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
Container,
Text,
Alert,
Icon,
FormContent} from './SigninElements'
import {Button, ButtonRouter} from '../ButtonElement';


export default function Signin() {
    const emailRef = useRef()
    const passwordRef = useRef()
    const {login} = useAuth()
    const [error, setError] = useState("")
    const [loading, setLoading] = useState(false)
    const history = useHistory()
  
    async function handleSubmit(e) {
      e.preventDefault()
  
      try {
        setError("")
        setLoading(true)
        await login(emailRef.current.value, passwordRef.current.value)
        history.push("/dashboard")
      } catch {
        setError("Failed to log in, incorrect password or unknown username.")
      }
  
      setLoading(false)
    }

    return (
        <>
        <Container>
            <FormWrap>
                <Icon to ="/">GuardMyPi</Icon>
                <FormContent>
                <Form onSubmit={handleSubmit}>
                    <FormH1>Sign in to your account</FormH1>
                    {error && <Alert variant="danger">{error}</Alert>}
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput type='email' ref={emailRef} required/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput type='password' ref={passwordRef} required/>
                    <FormButton disabled={loading} type='submit'>Continue</FormButton>
                <Text> <Link to="/forgot-password">Forgot Password?</Link></Text>
                <Text> Need an account? <Link to="/signup">Sign Up</Link></Text>
                </Form>
            </FormContent>
            </FormWrap>
        
        </Container> 
    </>
    )
        
}
