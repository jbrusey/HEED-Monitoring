#!/bin/bash
# Streetlight installation Script

############################################################
# PREPARATION
############################################################

# 1. Disable resize on first boot, enable SSH and Wifi by default
#   a) Place a file named “ssh” (without any extension) onto the boot partition of the SD card
#   b) Delete "init=/usr/lib/raspi-config/init_resize.sh" from cmdline.txt
#   c) Create a file called "wpa_supplicant.conf" with:
#       ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
#       update_config=1
#       network={
#           ssid="....."
#           psk="....."
#           key_mgmt=WPA-PSK
#       }
#   d) Insert the SD card into the Pi
#   e) Boot it up
#   f) login with default "pi/raspberry" credentials
#   g) Copy this script onto the pi and run it with root priviligies

############################################################
# VARIABLES
############################################################

hostname="streetlight1"

############################################################
# PARTITIONING & INITIAL SETUP
############################################################
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

echo "Applying new password..."
echo "Insert the new passowrd: "
read password

sudo echo "root:$password" | chpasswd
sudo echo "pi:$password" | chpasswd

echo "Disabling swap..."
sudo swapoff -a
sudo dd if=/dev/zero of=/var/swap bs=1M count=100

echo "Starting build script..."

echo "Applying new hostname..."
echo  "$hostname" | sudo tee /etc/hostname
sudo tee /etc/hosts << EOF
127.0.0.1       localhost
::1             localhost ip6-localhost ip6-loopback
fe00::0         ip6-localnet
ff00::0         ip6-mcastprefix
ff02::1         ip6-allnodes
ff02::2         ip6-allrouters
127.0.1.1       $hostname
EOF

echo "Making sure SSH is enabled..."
sudo raspi-config nonint do_ssh 0

echo "Making sure Pi camera is enabled..."
sudo raspi-config nonint do_camera 0

echo "Making sure I2C is enabled..."
sudo raspi-config nonint do_i2c 0

echo "Disable serial login console"
sudo raspi-config nonint do_serial 1

echo "Performing update"
sudo apt-get update
sudo apt-get -y upgrade

echo "Installing necessary packages..."
sudo apt-get -y install supervisor git usbmount python-setuptools python-pip i2c-tools python-smbus autossh ppp vim picocom

echo "Setting up Pi Face"
cd /tmp
wget https://raw.github.com/piface/PiFace-Real-Time-Clock/master/install-piface-real-time-clock.sh
chmod +x install-piface-real-time-clock.sh
sudo ./install-piface-real-time-clock.sh

cd /etc/rc2.d
sudo ln -s ../init.d/pifacertc S01pifacertc
cd ../rc5.d
sudo ln -s ../init.d/pifacertc S01pifacertc

# sudo /etc/init.d/pifacertc start
# sudo hwclock --systohc

echo "Making changes to /etc/network/interfaces..."
sudo tee -a /etc/network/interfaces << EOF
auto fona
iface fona inet ppp
	provider fona
EOF

#https://learn.adafruit.com/fona-tethering-to-raspberry-pi-or-beaglebone-black/setup

#install wiringpi
echo "Installing WiringPi"
cd /tmp
git clone git://git.drogon.net/wiringPi
cd ./wiringPi
./build

if [[ "$@" != "nofootfall" ]]
then
echo "Installing Open Framework dependencies..."
cd /tmp

wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_dependencies.sh
chmod +x install_dependencies.sh
yes | sudo ./install_dependencies.sh # Warning: this isn't automated - you have to press Y to confirm installation

wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_codecs.sh
chmod +x install_codecs.sh
yes | sudo ./install_codecs.sh # Warning: this isn't automated - you have to press Y to confirm installation
fi

echo "Cloning HEED Repo..."
cd /tmp
git clone https://github.com/jbrusey/HEED-Monitoring.git
cd HEED-Monitoring/Streetlight/

sudo rsync -rv RPI/opt/ /opt/
sudo rsync -rv RPI/etc/ /etc/
sudo cp RPI/auto-ssh /etc/init.d/auto-ssh

cd /opt/HEED
sudo mkdir Data
sudo python setup.py develop

cd Footfall
sudo chmod +x Footfall

echo "Creating link to Footfall..."
sudo ln -s /opt/HEED/Footfall/Footfall /usr/bin/Footfall

echo "Making changes to /etc/rc.local..."
sudo sed -e "s/exit\s0//g" -i /etc/rc.local
sudo tee -a /etc/rc.local << EOF
gpio -g mode 17 out
gpio -g write 17 1
exit 0
EOF

echo "Prepare dispmaxn_vnc"
cd /home/pi
sudo apt-get -y install g++-4.8 libvncserver-dev libconfig++-dev
echo "Downloading vnc server repo..."
git clone https://github.com/patrikolausson/dispmanx_vnc.git
cd dispmanx_vnc
make

sudo mkdir /opt/vnc
sudo cp dispmanx_vncserver /opt/vnc/dispmanx_vncserver
sudo ln -s /opt/vnc/dispmanx_vncserver /usr/bin/dispmanx_vncserver

echo "Cleaning..."
sudo apt-get -y autoremove
sudo apt-get -y clean

rm -rf /tmp/*
# That's it. You can now run Footfall with: cd /opt/HEED/Footfall/; sudo ./Footfall
# cd /home/pi/openFrameworks/apps/Footfall/Footfall/bin/; sudo ./Footfall

# Add "usr/bin/tvservice -o" if you wish to disable monitor output

############################################################
# OPTIMIZATION
############################################################

echo "Removing wait for network on boot (speeds up boot to 5sec, can be changed in raspi-config)..."
sudo rm -f /etc/systemd/system/dhcpcd.service.d/wait.conf

echo "Disabling additional services..."

# Remove auto update services

sudo systemctl stop apt-daily.timer
sudo systemctl disable apt-daily.timer
sudo systemctl mask apt-daily.service

sudo systemctl stop apt-daily-upgrade.timer
sudo systemctl disable apt-daily-upgrade.timer
sudo systemctl mask apt-daily-upgrade.service

sudo systemctl stop remote-fs.target
sudo systemctl disable remote-fs.target

sudo systemctl stop wifi-country.service
sudo systemctl disable wifi-country.service

sudo systemctl stop bluetooth.service
sudo systemctl disable bluetooth.service

sudo systemctl stop console-setup.service
sudo systemctl disable console-setup.service

##############

#Add to /boot/config.txt

sudo tee -a /boot/config.txt << EOF

# turn wifi and bluetooth off
dtoverlay=pi3-disable-wifi
dtoverlay=pi3-disable-bt

# enable uart
enable_uart=1
EOF

############################################################
# DONE
############################################################

echo "Everything has been setup"

for i in `seq 1 10`;
do
        echo -ne "Rebooting in $((11-$i))... (press CTRL+C if you wish to abort, or R to reboot now) "\\r
        read -t 1 -n 1 -s key
        case $key in
            'r') break;;
        esac
done
echo
echo "Rebooting..."
sudo reboot
