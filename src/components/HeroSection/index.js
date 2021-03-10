import React, {useState} from 'react';
import Video from '../../videos/video.mp4';
import {Button, ButtonRouter} from '../ButtonElement';
import {HeroContainer,
HeroBg,
VideoBg,
HeroContent,
HeroH1,
HeroP,
HeroBtnWrapper,
ArrowForward,
ArrowRight} from './HeroElements';

const HeroSection = () => {
    const[hover, setHover] = useState(false)

    const onHover = () => {
        setHover(!hover)
    }

    return (
        <HeroContainer id='home'>
            <HeroBg>
                <VideoBg autoPlay loop muted src={Video} type='video/mp4'/>
            </HeroBg>
            <HeroContent>
                <HeroH1>The ultimate open-source security tool</HeroH1>
                <HeroP>
                     Protect your berries from unauthorised tampering.
                </HeroP>
                <HeroBtnWrapper>
                    <ButtonRouter to='/interest' onMouseEnter={onHover}
                    onMouseLeave={onHover} 
                    primary = 'true'
                    dark = 'true' smooth={true} duration={500} spy={true} exact='true' offset={-80}>
                        Register your interest {hover ? <ArrowForward /> : <ArrowRight/>}
                    </ButtonRouter>
                </HeroBtnWrapper>
            </HeroContent>
        </HeroContainer>
    )
}

export default HeroSection
