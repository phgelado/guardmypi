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
                    <ServicesP>GuardMyPi is an open-source, free to use security software tool. Your only cost is the hardware you want to pair with it!</ServicesP>
                </ServicesCard>
                <ServicesCard>
                    <ServicesIcon src ={Icon2}/>
                    <ServicesH2>Remote streaming</ServicesH2>
                    <ServicesP>You can access your GuardMyPi stream from anywhere in the world!</ServicesP>
                </ServicesCard>
                <ServicesCard>
                    <ServicesIcon src ={Icon3}/>
                    <ServicesH2>Smart security</ServicesH2>
                    <ServicesP>With our ML algorithms, this is to be filled. <br /><br /><br /></ServicesP>
                </ServicesCard>
            </ServicesWrapper>
        </ServicesContainer>
    )
}

export default Services
