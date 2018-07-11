#!/bin/bash
# Footfall build script
# Run only if you need to rebuild Footfall from scratch (ie. if you need to edit its core code)

echo "Downloading required packages..."
sudo apt-get install libraspberrypi-dev raspberrypi-kernel-headers

echo "Downloading Open Frameworks repo..."
cd ~
wget https://openframeworks.cc/versions/v0.9.8/of_v0.9.8_linuxarmv6l_release.tar.gz --no-check-certificate
mkdir openFrameworks
tar vxfz of_v0.9.8_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1

echo "Installing Open Framework dependencies..."
cd /home/pi/openFrameworks/scripts/linux/debian
sudo ./install_dependencies.sh
sudo ./install_codecs.sh

echo "Building Open Frameworks (takes time)..."
make Release -C /home/pi/openFrameworks/libs/openFrameworksCompiled/project

echo "Downloading Footfall repo..."
cd /home/pi/openFrameworks/apps/
git clone https://github.com/WatershedArts/Footfall.git

echo "Downloading footfall-required addons..."
cd /home/pi/openFrameworks/addons
wget https://raw.githubusercontent.com/WatershedArts/Footfall/master/getrepos.sh
chmod 777 getrepos.sh
./getrepos.sh

echo "Fix - Getting proper ofxCv commit version..."
cd /home/pi/openFrameworks/addons/ofxCv
git reset --hard f2e0f06668573cf41d4e700ede457e169fdf67c3

echo "Fix - Getting proper ofxHttpUtils commit version..."
cd /home/pi/openFrameworks/addons/ofxHttpUtils
git reset --hard 2cbab7c99136066828f0399984fcf4637678ace8

echo "Fix - Removing incorrect MMAL libraries from ofxCvPiCam..."
cd /home/pi/openFrameworks/addons/ofxCvPiCam
mv libs libs-old

# Make any changes necessary to Footfall HERE, before compiling

# Compile Footfall repo
cd /home/pi/openFrameworks/apps/Footfall/Footfall/
make clean
make

# /home/pi/openFrameworks/apps/Footfall/Footfall/bin/ 
# now contains your compiled Footfall. Copy it over wherever you want
