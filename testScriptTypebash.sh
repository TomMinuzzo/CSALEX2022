#!/bin/sh
sudo apt-get update -y
sudo apt install build-essential -y
sudo apt install git -y
sudo apt install cmake -y
sudo apt install gcc-arm-linux-gnueabihf -y
sudo apt install g++-arm-linux-gnueabihf -y
cd Desktop
git clone https://github.com/TomMinuzzo/CSALEX2022.git
cd CSALEX2022
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../armhf.cmake ..
make