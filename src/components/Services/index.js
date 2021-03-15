import React from 'react'
import Icon1 from '../../images/svg-1.svg'
import Icon2 from '../../images/svg-2.svg'
import Icon3 from '../../images/svg-3.svg'
import {ServicesContainer,
ServicesH1,
ServicesWrapper,
ServicesCard,
ServicesIcon,
ServicesH2,
ServicesP} from './ServicesElements';

/* To keep cards to the same size add line breaks <br />*/

const Services = () => {
    return (
        <ServicesContainer id='services'>
            <ServicesH1>Our Services</ServicesH1>
            <ServicesWrapper>
                <ServicesCard>
                    <ServicesIcon src ={Icon1}/>
                    <ServicesH2>Reduce expenses</ServicesH2>
                    <ServicesP>GuardMyPi is and will always be free and open-source. Your only cost is the hardware you want to pair it with!</ServicesP>
                </ServicesCard>
                <ServicesCard>
                    <ServicesIcon src ={Icon2}/>
                    <ServicesH2>Remote streaming</ServicesH2>
                    <ServicesP>Access your GuardMyPi stream from anywhere in the world!</ServicesP>
                </ServicesCard>
                <ServicesCard>
                    <ServicesIcon src ={Icon3}/>
                    <ServicesH2>Smart security</ServicesH2>
                    <ServicesP>GuardMyPi is a smart, transparent and customisable, what you see and develop is what you get.<br /><br /><br /></ServicesP>
                </ServicesCard>
            </ServicesWrapper>
        </ServicesContainer>
    )
}

export default Services
