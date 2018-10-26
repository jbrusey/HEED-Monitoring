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
password="ENTER password HERE"

############################################################
# PARTITIONING & INITIAL SETUP
############################################################
if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 
   exit 1
fi

echo "Applying new password..."
sudo echo "root:$password" | chpasswd
sudo echo "pi:$password" | chpasswd

echo "Disabling swap..."
sudo swapoff -a
sudo dd if=/dev/zero of=/var/swap bs=1M count=100

echo "Starting build script..."

echo "Applying new hostname..."
echo  "$hostname" > sudo /etc/hostname
echo "127.0.0.1       localhost \
::1             localhost ip6-localhost ip6-loopback \
fe00::0         ip6-localnet \
ff00::0         ip6-mcastprefix \
ff02::1         ip6-allnodes \
ff02::2         ip6-allrouters \
127.0.1.1       $hostname" > sudo /etc/hosts

echo "Making sure SSH is enabled..."
sudo raspi-config nonint do_ssh 0

echo "Making sure Pi camera is enabled..."
sudo raspi-config nonint do_camera 0

echo "Making sure I2C is enabled..."
sudo raspi-config nonint do_i2c 0

echo "Performing update"
sudo apt-get update
sudo apt-get -y upgrade

echo "Installing necessary packages..."
sudo apt-get -y install supervisor git usbmount python-setuptools python-pip i2c-tools python-smbus autossh ppp autossh vim

echo "Setting up Pi Face"
cd /tmp
wget https://raw.github.com/piface/PiFace-Real-Time-Clock/master/install-piface-real-time-clock.sh
chmod +x install-piface-real-time-clock.sh
sudo ./install-piface-real-time-clock.sh

cd /etc/rc2.d
sudo ln -s ../init.d/pifacertc S01pifacertc
cd ../rc5.d
sudo ln -s ../init.d/pifacertc S01pifacertc

sudo /etc/init.d/pifacertc start
sudo hwclock --systohc

echo "Making changes to /etc/network/interfaces..."
sudo tee -a /etc/netowrk/interfaces << EOF
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

echo "Installing Open Framework dependencies..."
cd /tmp

wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_dependencies.sh
chmod +x install_dependencies.sh
yes | sudo ./install_dependencies.sh # Warning: this isn't automated - you have to press Y to confirm installation

wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_codecs.sh
chmod +x install_codecs.sh
yes | sudo ./install_codecs.sh # Warning: this isn't automated - you have to press Y to confirm installation

echo "Cloning HEED Repo..."
cd /tmp
git clone https://github.com/jbrusey/HEED-Monitoring.git
cd HEED-Monitoring/Streetlight/

sudo rsync -rv RPI/opt/ /opt/
sudo rsync -rv RPI/etc/ /etc/

cd /opt/HEED
sudo mkdir Data
sudo python setup.py develop

cd Footfall
sudo chmod +x Footfall

echo "Creating link to Footfall..."
sudo ln -s /opt/HEED/Footfall/Footfall /usr/bin/Footfall

echo "Cleaning..."
sudo apt-get -y autoremove
sudo apt-get -y clean

rm -rf /tmp/*
# That's it. You can now run Footfall with: cd /opt/HEED/Footfall/; sudo ./Footfall
# cd /home/pi/openFrameworks/apps/Footfall/Footfall/bin/; sudo ./Footfall

echo "Making changes to /etc/rc.local..."
sudo sed -e "s/exit\s0//g" -i /etc/rc.local
sudo tee -a /etc/rc.local << EOF
gpio -g mode 17 out
gpio -g write 17 1
exit 0
EOF
# Add "usr/bin/tvservice -o" if you wish to disable monitor output

exit 0
############################################################
# OPTIMIZATION (OPTIONAL)
############################################################

echo "Enabling SSH access as Root..."
echo "PermitRootLogin yes" >> /etc/ssh/sshd_config

echo "Removing all references of 'console='' in /boot/cmdline.txt..."
sudo sed -e s/console=serial0,115200//g -i /boot/cmdline.txt
sudo sed -e s/console=tty1//g -i /boot/cmdline.txt

echo "Disabling rainbow splash at startup..."
echo "disable_splash=1" >> /boot/config.txt
echo "boot_delay=0" >> /boot/config.txt
echo "avoid_safe_mode=1" >> /boot/config.txt

echo "Disabling raspberries logo"
sed -i 's/$/ loglevel=0 logo.nologo /' /boot/cmdline.txt

echo "Removing wait for network on boot (speeds up boot to 5sec, can be changed in raspi-config)..."
sudo rm -f /etc/systemd/system/dhcpcd.service.d/wait.conf

echo "Disabling additional services..."

sudo systemctl stop display-manager.service
sudo systemctl disable display-manager.service
sudo rm -f /etc/systemd/system/display-manager.service

#sudo systemctl stop autologin@.service
#sudo systemctl disable autologin@.service
#sudo rm -f /etc/systemd/system/autologin@.service

sudo systemctl stop autovt@.service
sudo systemctl disable autovt@.service
sudo rm -f /etc/systemd/system/autovt@.service

sudo systemctl stop rpi-display-backlight.service
sudo systemctl disable rpi-display-backlight.service
sudo rm -f /etc/systemd/system/rpi-display-backlight.service

# Remove auto update services

sudo systemctl stop apt-daily.timer
sudo systemctl disable apt-daily.timer
sudo systemctl mask apt-daily.service

sudo systemctl stop apt-daily-upgrade.timer
sudo systemctl disable apt-daily-upgrade.timer
sudo systemctl mask apt-daily-upgrade.service

sudo systemctl stop rpi-display-backlight.service
sudo systemctl disable rpi-display-backlight.service
sudo rm -f /etc/systemd/system/rpi-display-backlight.service

sudo systemctl stop remote-fs.target
sudo systemctl disable remote-fs.target
sudo rm -f "/etc/systemd/system/disable remote-fs.target"

sudo systemctl stop wifi-country.service
sudo systemctl disable wifi-country.service
sudo rm -f /etc/systemd/system/wifi-country.service

#sudo systemctl stop hciuart.service
#sudo systemctl disable hciuart.service
#sudo rm -f /etc/systemd/system/hciuart.service

sudo systemctl stop bluetooth.service
sudo systemctl disable bluetooth.service
sudo rm -f /etc/systemd/system/bluetooth.service

sudo systemctl stop avahi-daemon.service
sudo systemctl disable avahi-daemon.service
sudo rm -f /etc/systemd/system/avahi-daemon.service

sudo systemctl stop nfs-client.target
sudo systemctl disable nfs-client.target
sudo rm -f /etc/systemd/system/nfs-client.target

sudo systemctl stop console-setup.service
sudo systemctl disable console-setup.service
sudo rm -f /etc/systemd/system/console-setup.service

sudo systemctl stop keyboard-setup.service
sudo systemctl disable keyboard-setup.service
sudo rm -f /etc/systemd/system/keyboard-setup.service

sudo systemctl stop triggerhappy.service
sudo systemctl disable triggerhappy.service
sudo rm -f /etc/systemd/system/triggerhappy.service

sudo systemctl stop triggerhappy.socket
sudo systemctl disable triggerhappy.socket
sudo rm -f /etc/systemd/system/triggerhappy.socket

sudo systemctl disable dphys-swapfile
sudo systemctl disable dphys-swapfile
sudo rm -f /etc/systemd/system/dphys-swapfile

sudo systemctl daemon-reload

sudo insserv -r plymouth
sudo insserv -r plymouth-log
sudo insserv -r nfs-common
sudo insserv -r avahi-daemon
#sudo insserv -r bluetooth
sudo insserv -r console-setup.sh
sudo insserv -r keyboard-setup.sh
sudo insserv -r triggerhappy
sudo insserv -r raspi-config
sudo insserv -r dphys-swapfile

sudo rm -f /etc/init.d/plymouth
sudo rm -f /etc/init.d/plymouth-log
sudo rm -f /etc/init.d/nfs-common
sudo rm -f /etc/init.d/avahi-daemon
#sudo rm -f /etc/init.d/bluetooth
sudo rm -f /etc/init.d/console-setup.sh
sudo rm -f /etc/init.d/keyboard-setup.sh
sudo rm -f /etc/init.d/triggerhappy
sudo rm -f /etc/init.d/raspi-config
sudo rm -f /etc/init.d/dphys-swapfile

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

##############


#Generate ssh key and add to pi user on cogentee
ssh-keygen -t rsa
#check you can ssh to cogentee

#echo "Disabling TV series (Run only when all setup is sorted)..."
#sed -e s/exit 0//g -i /etc/rc.local
#echo "/usr/bin/tvservice -o
#exit 0" >> /etc/rc.local # Change -o to -p to re-enablew

#Disable ACT LED & WiFi & BT & Audio (only when all setup is sorted)

#Add to /boot/config.txt

# disable audio (loads snd_bcm2835)
#dtparam=audio=off
#gpu_mem=16

# Disable the ACT LED.
#dtparam=act_led_trigger=none
#dtparam=act_led_activelow=off

# Disable the PWR LED.
#dtparam=pwr_led_trigger=none
#dtparam=pwr_led_activelow=off

# turn and wifi bluetooth off
#dtoverlay=pi3-disable-wifi
#dtoverlay=pi3-disable-bt


#Reboot
#check:
#1. ppp0 comes up
#ifconfig ppp0
#2. IAM code is running
#ps -ef | grep IAM
#3. Serial forwarder has started
#ps -ef | grep sf
#3. flat looger is running
#ps -ef | grep Flat

#poweroff
#plug into laptop

#fdisk /dev/disk2
#dd if=/dev/disk2 bs=512 count=7494037 of=f.img

#Edit already created

#Set hostname
#Expand partion
#disable SPI
#sudo emacs /etc/default/auto-ssh

#reboot
#ssh-keygen -t rsa
#sudo apt-get update
#sudo apt-get -y upgrade
#sudo apt -y autoremove
#sudo rpi-update
#sudo reboot

