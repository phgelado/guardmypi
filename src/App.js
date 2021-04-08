import React from 'react'
import './App.css';
import {BrowserRouter as Router, Switch, Route} from 'react-router-dom'
import Home from './pages';
import SignInPage from './pages/signin';
import SignUpPage from './pages/signup';
import {AuthProvider} from './contexts/AuthContext'
import Signup from "./pages/signup2"
import { Container } from "react-bootstrap"
import Dashboard from "./pages/Dashboard"
import Login from "./pages/Login"
import PrivateRoute from "./pages/PrivateRoute"
import ForgotPassword2 from "./pages/ForgotPassword2"
import ForgotPassword from "./pages/forgotpassword"
import UpdateProfilePage from "./pages/UpdateProfile"
import UpdateProfile2 from "./pages/updateprofile2"
import Dashboard2 from "./pages/Dashboard2"

function App() {
  return (
    <Router>
      <AuthProvider>
      <Switch>
        <Route path='/' component={Home} exact />
        <Route path='/signin' component={SignInPage} exact />
        <PrivateRoute path="/update-profile" component={UpdateProfile2} />
        <PrivateRoute path="/update-profile2" component={UpdateProfilePage} />
        <Route path="/signup2" component={Signup} />
        <Route path="/signup" component={SignUpPage} />
        <Route path="/login" component={Login} />
        <Route path="/forgot-password2" component={ForgotPassword2} />
        <Route path="/forgot-password" component={ForgotPassword} />
        <PrivateRoute path="/dashboard" component={Dashboard2} />
      </Switch>
      </AuthProvider>
    </Router>
  );
}

export default App;
