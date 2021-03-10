import React, {useState} from 'react'
import HeroSection from '../components/HeroSection';
import { homeObjOne,
        homeObjTwo,
        homeObjThree} from '../components/InfoSection/Data';
import Navbar from '../components/Navbar';
import Sidebar from '../components/Sidebar';
import InfoSection from '../components/InfoSection';
import Services from '../components/Services';
import Footer from '../components/Footer';

const Home = () => {
    const [isOpen, setIsOpen] = useState(false)

    const toggle = () =>{
        setIsOpen(!isOpen)
    };

    return (
        <>
        <Sidebar isOpen={isOpen} toggle={toggle}/>
        <Navbar isOpen={isOpen} toggle={toggle}/>
        <HeroSection />
        <InfoSection {...homeObjOne} />
        <Services />
        <InfoSection {...homeObjTwo} />
        <InfoSection {...homeObjThree} />
        <Footer />
        </>
    );
};

export default Home;
