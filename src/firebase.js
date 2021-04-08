import firebase from "firebase/app"
import "firebase/auth"

const app = firebase.initializeApp({
    apiKey: "AIzaSyBDP53WJGODOC1fgRY-LZfWpvjZaqggTXg",
    authDomain: "guardmypi.firebaseapp.com",
    projectId: "guardmypi",
    storageBucket: "guardmypi.appspot.com",
    messagingSenderId: "617573856435",
    appId: "1:617573856435:web:d9e35f264ef23874a4acc6",
    measurementId: "G-RLH80DTEG2"
})

export const auth = app.auth()
export default app
