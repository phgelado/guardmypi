import React from 'react'
import './App.css';
import {BrowserRouter as Router, Switch, Route} from 'react-router-dom'
import Home from './pages';
import SignInPage from './pages/signin';
import RegisterInterestPage from './pages/interest'

function App() {
  return (
    <Router>
      <Switch>
        <Route path='/' component={Home} exact />
        <Route path='/signin' component={SignInPage} exact />
        <Route path='/interest' component={RegisterInterestPage} exact />
      </Switch>
    </Router>
  );
}

export default App;
