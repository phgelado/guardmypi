import React from 'react';
import {animateScroll as scroll} from 'react-scroll'
import { FaFacebook, FaGithub, FaInstagram, FaTwitter, FaLinkedin } from 'react-icons/fa';
import{
    FooterContainer,
    FooterWrap,
    FooterLink,
    FooterLinksContainer,
    FooterLinksWrapper,
    FooterLinkItems,
    FooterLinkTitle,
    SocialMedia,
    SocialMediaWrap,
    SocialLogo,
    WebsiteRights,
    SocialIcons,
    SocialIconLink
} from './FooterElements';

const Footer = () => {

    const toggleHome = () => {
        scroll.scrollToTop();
    };
    
    return (
        <FooterContainer>
            <FooterWrap>
                
                <SocialMedia>
                <SocialMediaWrap>
                    <SocialLogo to = '/' onClick = {toggleHome}> GuardMyPi </SocialLogo>
                    <WebsiteRights> GuardMyPi © {new Date().getFullYear()} All rights reserved.</WebsiteRights>
                    <SocialIcons>
                    <SocialIconLink href='https://github.com/phgelado/guardmypi' target='_blank' aria-label = 'Github'>
                            <FaGithub/>
                        </SocialIconLink>
                        <SocialIconLink href='https://www.facebook.com/guardmypi/' target='_blank' aria-label = 'Facebook'>
                            <FaFacebook/>
                        </SocialIconLink>
                        <SocialIconLink href='https://www.instagram.com/guardmypi/' target='_blank' aria-label = 'Instagram'>
                            <FaInstagram/>
                        </SocialIconLink>
                        <SocialIconLink href='https://twitter.com/guardmypi' target='_blank' aria-label = 'Twitter'>
                            <FaTwitter/>
                        </SocialIconLink>
                        <SocialIconLink href='https://www.linkedin.com/company/guardmypi/' target='_blank' aria-label = 'Linkedin'>
                            <FaLinkedin/>
                        </SocialIconLink>
                    </SocialIcons>
                </SocialMediaWrap>
            </SocialMedia>
            </FooterWrap>
        </FooterContainer>
    )
}

export default Footer
