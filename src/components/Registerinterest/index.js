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
FormContent} from './RegisterinterestElements'

const Registerinterest = () => {
    return (
        <>
        <Container>
            <FormWrap>
                <Icon to ="/">GuardMyPi</Icon>
                <FormContent>
                <Form action = '#'>
                    <FormH1>Register your interest</FormH1>
                    <Text>GuardMyPi is on alpha testing right now, keep up to date with our development by signing up below!</Text>
                    <FormLabel htmlFor='for'>Name</FormLabel>
                    <FormInput type='name' required/>
                    <FormLabel htmlFor='for'> Email</FormLabel>
                    <FormInput type='email' required/>
                    <FormButton type='submit'>Keep me posted!</FormButton>
                </Form>
            </FormContent>
            </FormWrap>
        </Container> 
        </>
    )
}

export default Registerinterest

