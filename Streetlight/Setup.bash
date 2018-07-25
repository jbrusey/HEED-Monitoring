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

hostname="footfall2"
password="pervasive"

############################################################
# PARTITIONING & INITIAL SETUP
############################################################

#echo "Applying new password..."
sudo echo "root:$password" | chpasswd
sudo echo "pi:$password" | chpasswd

echo "Disabling swap..."
sudo swapoff -a
cd /var
sudo dd if=/dev/zero of=swap bs=1M count=100
cd

echo "Starting build script..."
partitionCount=$(fdisk -l /dev/mmcblk0 | grep '/dev/mmcblk0p' | wc --lines)

if [[ $partitionCount -lt 3 ]]; then
	echo "Disk is NOT partitioned"
	
	startSector=$(sudo fdisk -l /dev/mmcblk0 | grep '/dev/mmcblk0p2' | cut -d' ' -f 7)
	OSPartitionSize=8
	
    # We are creating 3 partitions - the root one with OS on it, tmp for temporary files and var for all the other stuff
    # The idea is that root partition should be read only, while tmp and var have read and write permissions
	# WARNING: SIZES SPECIFIED FOR 32GB SD CARD - IF USING DIFFERENT SD SIZE, PLEASE AMEND BELOW SIZES ACCORDINGLY
	echo "Repartitioning the drive..."
	(echo d; echo 2; \
	echo n; echo p; echo ; echo $startSector; echo +$[$OSPartitionSize]g; \
    echo n; echo e; echo ; echo $[$startSector+1024+(2*1024*1024*$OSPartitionSize)]; echo ; \
    echo n; echo l; echo ; echo +6g; \
	echo n; echo l; echo ; echo ; \
	echo w;) | sudo fdisk /dev/mmcblk0
	sudo partprobe /dev/mmcblk0
	sudo resize2fs /dev/mmcblk0p2

    echo "Formatting new partitions.."
	sudo mkfs.ext4 -F /dev/mmcblk0p5
	sudo mkfs.ext4 -F /dev/mmcblk0p6

    echo "Configuring and mounting new partitions to root directories..."
	#
	sudo mkdir /mnt/tmp
	sudo mkdir /mnt/var
	#
	sudo mount /dev/mmcblk0p5 /mnt/tmp
	sudo mount /dev/mmcblk0p6 /mnt/var

	#
	sudo cp /tmp/* /mnt/tmp -rp
	sudo cp /var/* /mnt/var -rp
	#
    sudo mv /tmp /tmp.old
	sudo mv /var /var.old
	#
	sudo mkdir /tmp
	sudo mkdir /var
	#
	sudo umount /dev/mmcblk0p5
	sudo umount /dev/mmcblk0p6
	#
	sudo mount /dev/mmcblk0p5 /tmp
	sudo mount /dev/mmcblk0p6 /var
	#
	echo "Waiting for partitions to mount..."
	sleep 10
	sudo rm -rf /tmp.old
	sudo rm -rf /var.old

    echo "Amending fstab..."
    echo "proc            /proc           proc    defaults						0       0
/dev/mmcblk0p1  /boot           vfat    defaults          				0       2
/dev/mmcblk0p2  /               ext4    rw,defaults,noatime,nodiratime  0       1
/dev/mmcblk0p5	/tmp			ext4	rw,defaults,noatime,nodiratime	0		2
/dev/mmcblk0p6	/var			ext4	rw,defaults,noatime,nodiratime	0		2
# a swapfile is not a swap partition, no line here
#   use  dphys-swapfile swap[on|off]  for that" > /etc/fstab


else
	echo "Disk is already partitioned. Skipping..."
fi

echo "Applying new hostname..."
echo  "$hostname" > /etc/hostname
echo "127.0.0.1       localhost
::1             localhost ip6-localhost ip6-loopback
fe00::0         ip6-localnet
ff00::0         ip6-mcastprefix
ff02::1         ip6-allnodes
ff02::2         ip6-allrouters
127.0.1.1       $hostname" > /etc/hosts

echo "Making sure SSH is enabled..."
sudo raspi-config nonint do_ssh 0

echo "Making sure Pi camera is enabled..."
sudo raspi-config nonint do_camera 1

echo "Performing fixes to Read Only filesystem..."
sudo sed -i -e 's_/run/dhcpcd.pid_/var/run/dhcpcd.pid_g' /etc/systemd/system/dhcpcd5.service

echo "Performing update"
sudo apt-get update
sudo apt-get -y upgrade

echo "Installing necessary packages..."
sudo apt-get -y install supervisor emacs git subversion usbmount python-setuptools python-pip i2c-tools python-smbus autossh ppp screen elinks autossh

sudo pip install pyserial
sudo pip install paho-mqtt

echo "Cleaning..."
sudo apt-get -y autoremove
sudo apt-get -y clean

#Enable usb auto mounting
#Edit /lib/systemd/system/systemd-udevd.service and change the line
#MountFlags=slave
#to
#MountFlags=shared

echo "Creating file structure"
sudo mkdir /opt/HELP
cd /opt/HELP

echo "Setting up Pi Face"
wget https://raw.github.com/piface/PiFace-Real-Time-Clock/master/install-piface-real-time-clock.sh
chmod +x install-piface-real-time-clock.sh
sudo ./install-piface-real-time-clock.sh

cd /etc/rc2.d
sudo ln -s ../init.d/pifacertc S01pifacertc
cd ../rc5.d
sudo ln -s ../init.d/pifacertc S01pifacertc

#sudo reboot
sudo hwclock --systohc

echo "Making changes to /lib/systemd/system/hciuart.service..."
sudo sed -i -e 's|After=dev-serial1.device|After=dev-ttyS0.device|g' /lib/systemd/system/hciuart.service
sudo sed -i -e 's|ExecStart=/usr/bin/btuart|ExecStart=/usr/bin/hciattach /dev/ttyS0 bcm43xx 460800 noflow -|g' /lib/systemd/system/hciuart.service

echo "Making changes to /etc/network/interfaces..."
echo "
auto fona
iface fona inet ppp
	provider fona" >> /etc/network/interfaces

#https://learn.adafruit.com/fona-tethering-to-raspberry-pi-or-beaglebone-black/setup

#install wiringpi
mkdir ~/git
cd ~/git
git clone git://git.drogon.net/wiringPi
cd ./wiringPi
git pull origin
./build

echo "Making changes to /etc/rc.local..."
sudo sed -e "s/exit\s0//g" -i /etc/rc.local
echo "gpio -g mode 17 out 
gpio -g write 17 1

exit 0" >> /etc/rc.local
# Add "usr/bin/tvservice -o" if you wish to disable monitor output

echo "Cloning HELP Repo..."
cd ~/git
git clone https://github.com/rwilkins87/HELP-Monitoring-Interns.git
cd ~/git/HELP-Monitoring-Interns/Streetlight/

sudo  rsync -rv  RPI/opt/ /opt/
sudo  rsync -rv  RPI/etc/ /etc/

cd /opt/HELP
sudo python setup.py develop

sudo chmod 755 /etc/init.d/autossh 
sudo update-rc.d autossh defaults
sudo update-rc.d autossh enable

sudo chmod 777 /opt/HELP/*
cd Footfall
sudo chmod +x Footfall


echo "Installing Open Framework dependencies..."
cd /tmp/

#wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_dependencies.sh
#chmod +x install_dependencies.sh
#sudo ./install_dependencies.sh # Warning: this isn't automated - you have to press Y to confirm installation

#wget https://raw.githubusercontent.com/openframeworks/openFrameworks/master/scripts/linux/debian/install_codecs.sh
#chmod +x install_codecs.sh
#sudo ./install_codecs.sh # Warning: this isn't automated - you have to press Y to confirm installation


# That's it. You can now run Footfall with: cd /opt/HELP/Footfall/; sudo ./Footfall

# cd /home/pi/openFrameworks/apps/Footfall/Footfall/bin/; sudo ./Footfall

############################################################
# OPTIMIZATION (OPTIONAL)
############################################################

echo "Enabling SSH access as Root..."
echo "
PermitRootLogin yes" >> /etc/ssh/sshd_config

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

sudo systemctl stop hciuart.service
sudo systemctl disable hciuart.service
sudo rm -f /etc/systemd/system/hciuart.service

#sudo systemctl stop bluetooth.service
#sudo systemctl disable bluetooth.service
#sudo rm -f /etc/systemd/system/bluetooth.service

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

