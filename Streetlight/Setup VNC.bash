#!/bin/bash

echo "Downloading required packages..."
sudo apt-get install g++ libvncserver-dev libconfig++-dev

echo "Downloading vnc server repo..."
sudo git clone https://github.com/patrikolausson/dispmanx_vnc.git
cd dispmanx_vnc
sudo make

add to /boot/config.txt
hdmi_force_hotplug=1
hdmi_cvt=320 240 60 1 0 0 0
hdmi_group=2
hdmi_mode=87

sudo ./dispmanx_vncserver -s 0 -d -m -t=1