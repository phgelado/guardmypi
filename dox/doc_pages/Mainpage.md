@mainpage GuardMyPi 
@tableofcontents

@section Introduction

GuardMyPi is a home security system centred around the Raspberry Pi 3. It utilises the Pi NoIR %Camera to monitor a room or entrance point in a house. Facial recognition software is used to distinguish between house members (including pets) and intruders. If an intruder is detected, a notification is sent to the user via a web application. 

@authors

Magnus Bell Cochran - 2258776B@student.gla.ac.uk

Pedro Hernandez Gelado - 2262004h@student.gla.ac.uk

Aidan Porteous - 2245628p@student.gla.ac.uk

Project Link: [https://github.com/phgelado/guardmypi](https://github.com/phgelado/guardmypi)

@section Hardware Required Hardware

This project can be completed within a £50 budget (excluding the Raspberry Pi) with the following components:

* Raspberry Pi 3  https://thepihut.com/products/raspberry-pi-3-model-b-plus
* Pi NoIR %Camera V2 https://thepihut.com/products/raspberry-pi-noir-camera-module
* Infrared LED Ring  https://www.amazon.co.uk/Infrared-Illuminator-Board-Security-Camera/dp/B0057DPXI4/ref=sr_1_3?dchild=1&keywords=Infrared+IR+board&qid=1614706403&sr=8-3
* Raspberry Pi 3/4 and %Camera enclosure https://thepihut.com/products/raspberry-pi-4-3-camera-case
* MT3068 DC-DC Voltage Step Up Adjustable Boost Converter Module 2A https://www.ebay.co.uk/itm/MT3608-DC-DC-Voltage-Step-Up-Adjustable-Boost-Converter-Module-2A-UK/193424475011


@section Build Building OpenCV from Source

1. Downloading pre-built OpenCV4
```sh
wget https://github.com/sol-prog/raspberry-pi-opencv/releases/download/opencv4rpi2.1/opencv-4.1.0-armhf.tar.bz2
```
2. Extract the archive
   ```sh
   tar xvf opencv-4.1.0-armhf.tar.bz2
   ```
3. Move the extracted archive to the /opt folder 
   ```sh
   sudo mv opencv-4.1.0 /opt
   ```
4. Remove the archive (optional) 
   ```sh
   rm opencv-4.1.0-armhf.tar.bz2
   ```
5. Install video and image support packages
 ```sh 
sudo apt install libjpeg-dev libtiff-dev libjasper-dev libpng-dev libwebp-dev libopenexr-dev
sudo apt install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev libxvidcore-dev libx264-dev libdc1394-22-dev libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev
```
6. Install packages needed for OpenCV's interface
```sh
sudo apt install libgtk-3-dev libqtgui4 libqtwebkit4 libqt4-test python3-pyqt5
```
7. Install packages for OpenCV to run at a good speed
```sh
sudo apt install libatlas-base-dev liblapacke-dev gfortran
```
8. Add OpenCV to the system path 
```sh
cd ~
echo 'export LD_LIBRARY_PATH=/opt/opencv-4.1.0/lib:$LD_LIBRARY_PATH' >> .bashrc
. .bashrc
```
9. Restart the terminal or log back in to the RPi if connected via SSH

10. Install git if necessary
```sh
sudo apt-get install git
```
11. Clone a config file to use OpenCV for C++
```sh 
git clone https://gist.github.com/sol-prog/ed383474872958081985de733eaf352d opencv_cpp_compile_settings
 cd opencv_cpp_compile_settings/
 sudo cp opencv.pc /usr/lib/arm-linux-gnueabihf/pkgconfig
 cd ~
 rm -rf opencv_cpp_compile_settings/
```
12. Check OpenCV has installed correctly on the RPi 
```sh
pkg-config --modversion opencv
```

@section MJPG Streamer 

1. Ensure you have CMake installed, as well as the dependencies mjpg-streamer uses: 
```
sudo apt-get install libjpeg8-dev imagemagick libv4l-dev
```

2. Clone the mjpg-streamer repository you can find here https://github.com/jacksonliam/mjpg-streamer.
```
git clone https://github.com/jacksonliam/mjpg-streamer
```
4. Access the mjpg-streamer directory. 
```
cd mjpg-streamer
cd mjpg-streamer-experimental
```
4. Build and install mjpg-streamer using CMake.
```
make
sudo make install
```

@section Installing Dataplicity

Dataplicity is a simple to use, safe and free tool for you to access and control your RaspberryPi remotely, allowing you to setup and start your alarm from anywhere, as well as accessing the camera's live video feed. 

Similar to a VPN, but easier to install, it also allows you to access your Pi's local port 80 (http://localhost:80) through a domain, which is where the Pi's live camera feed is streamed to. Installing Dataplicity is very simple! Follow the instructions here: https://www.dataplicity.com/.

@section MotionDetector

The system has to be capable of detecting changes in individual frames. The changes in these frames are what allow computer visual applications to detect motion. However, while consecutive frames are not identical the MotionDetector class applies a Gaussian blur to the data to filter high frequency noise that would irregularly trigger the motion warning and the rest of the system. 

Take a frame from a video feed of an empty room: 

![](background.png)

If a person (or pet) walks into the feed, the detector will find large changes in contours by taking the absolute difference of the two frame's pixel intensities leaving only the regions containing motion:

![](frame_diff.png)

The detector acts as an event-base trigger invokes methods such as facial recognition and human detection. 

![](motiondetected.png)

@see HumanDetector

@section HumanDetector

overview how this class works
@section Acknowledgements

* [README Template](https://github.com/othneildrew/Best-README-Template)
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Img Shields](https://shields.io)
* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Pages](https://pages.github.com)
* [Animate.css](https://daneden.github.io/animate.css)
* [Loaders.css](https://connoratherton.com/loaders)
* [Slick Carousel](https://kenwheeler.github.io/slick)
* [Smooth Scroll](https://github.com/cferdinandi/smooth-scroll)
* [Sticky Kit](http://leafo.net/sticky-kit)
* [JVectorMap](http://jvectormap.com)
* [Font Awesome](https://fontawesome.com)