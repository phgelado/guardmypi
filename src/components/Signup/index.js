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



export default function Signin() {
  const emailRef = useRef()
  const passwordRef = useRef()
  const passwordConfirmRef = useRef()
  const { signup } = useAuth()
  const [error, setError] = useState("")
  const [loading, setLoading] = useState(false)
  const history = useHistory()
  
    async function handleSubmit(e) {
      e.preventDefault()
  

      if (passwordRef.current.value !== passwordConfirmRef.current.value) {
        return setError("Passwords do not match")
      }
  
      try {
        setError("")
        setLoading(true)
        await signup(emailRef.current.value, passwordRef.current.value)
        history.push("/")
      } catch {
        setError("Failed to create an account, please ensure your password is more than 6 characters long.")
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
                    <FormH1>Create your GuardMyPi account now!</FormH1>
                    {error && <Alert>{error}</Alert>}
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput type='email' ref={emailRef} required/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput type='password' ref={passwordRef} required/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput type='password' ref={passwordConfirmRef} required/>
                    <FormButton disabled={loading} type='submit'>Sign Up</FormButton>
                    <Text> <Link to="/forgot-password">Forgot Password?</Link></Text>
                    <Text> Already have an account? <Link to="/signin">Sign In</Link></Text>
                </Form>
            </FormContent>
            </FormWrap>
        
        </Container> 
    </>
    )
        
}

