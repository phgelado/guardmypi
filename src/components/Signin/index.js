import React from 'react'
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
FormContent} from './SigninElements'

const Signin = () => {
    return (
        <>
        <Container>
            <FormWrap>
                <Icon to ="/">GuardMyPi</Icon>
                <FormContent>
                <Form action = '#'>
                    <FormH1>Sign in to your account</FormH1>
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput type='email' required/>
                    <FormLabel htmlFor='for'>Password</FormLabel>
                    <FormInput type='password' required/>
                    <FormButton type='submit'>Continue</FormButton>
                </Form>
            </FormContent>
            </FormWrap>
        </Container> 
        </>
    )
}

export default Signin
