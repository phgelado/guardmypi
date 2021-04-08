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
Icon,
Alert,
FormContent} from './SigninElements'

import {Button, ButtonRouter} from '../ButtonElement';


export default function ForgotPassword() {
  const emailRef = useRef()
  const { resetPassword } = useAuth()
  const [error, setError] = useState("")
  const [message, setMessage] = useState("")
  const [loading, setLoading] = useState(false)

  async function handleSubmit(e) {
    e.preventDefault()

    try {
      setMessage("")
      setError("")
      setLoading(true)
      await resetPassword(emailRef.current.value)
      setMessage("Check your inbox for further instructions")
    } catch {
      setError("Username not recognised, failed to reset password")
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
                    <FormH1>Password Reset</FormH1>
                    {error && <Alert>{error}</Alert>}
                    {message && <Alert>{message}</Alert>}
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput type='email' ref={emailRef} required/>
                    <FormButton disabled={loading} type='submit'>Reset Password</FormButton>
                    <Text><Link to="/signin">Sign In</Link></Text>
                <Text> Need an account? <Link to="/signup">Sign Up</Link></Text>
                </Form>
            </FormContent>
            </FormWrap>
        
        </Container> 
    </>
    )
        
}