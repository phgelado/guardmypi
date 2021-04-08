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



export default function UpdateProfile() {
  const emailRef = useRef()
  const passwordRef = useRef()
  const passwordConfirmRef = useRef()
  const { currentUser, updatePassword, updateEmail } = useAuth()
  const [error, setError] = useState("")
  const [loading, setLoading] = useState(false)
  const history = useHistory()

  function handleSubmit(e) {
    e.preventDefault()
    if (passwordRef.current.value !== passwordConfirmRef.current.value) {
      return setError("Passwords do not match")
    }

    const promises = []
    setLoading(true)
    setError("")

    if (emailRef.current.value !== currentUser.email) {
      promises.push(updateEmail(emailRef.current.value))
    }
    if (passwordRef.current.value) {
      promises.push(updatePassword(passwordRef.current.value))
    }

    Promise.all(promises)
      .then(() => {
        history.push("/")
      })
      .catch(() => {
        setError("Failed to update account")
      })
      .finally(() => {
        setLoading(false)
      })
  }

    return (
        <>
        <Container>
            <FormWrap>
                <Icon to ="/">GuardMyPi</Icon>
                <FormContent>
                <Form onSubmit={handleSubmit}>
                    <FormH1>Update Profile</FormH1>
                    {error && <Alert>{error}</Alert>}
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput  type="email" ref={emailRef} required defaultValue={currentUser.email}/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput type="password" ref={passwordRef} placeholder="Leave blank to keep the same" required/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput  type="password" ref={passwordConfirmRef} placeholder="Leave blank to keep the same" required/>
                    <FormButton disabled={loading} type='submit'>Update</FormButton>
                    <Text><Link to="/">Cancel</Link></Text>
                </Form>
            </FormContent>
            </FormWrap>
        
        </Container> 
    </>
    )
        
}
