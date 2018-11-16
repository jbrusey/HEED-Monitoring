# Installation guide
## Clean raspbian image
- get the `Setup.bash` with `wget`, make it executable (`chmod +x Setup.bash` and run it as sudo (`sudo ./Setup.bash`)
- add `nofootfall` parameter (`sudo ./Setup.bash nofootfall`) in order to skip the footfall installation

## After reboot
- change hostname (`/etc/hostname`)
- set the repote port for autossh in `/etc/default/auto-ssh` (`REMOTE_PORT=xxxx`) and write it on cogentee wiki
- generate ssh key (`ssh-keygen`) and put  the content of `/home/pi/.ssh/id_rsa.pub`  on cogentee wiki

## Cloning cards
After you do the [clean image installation](#Clean-raspbian-image), take out the SD card (without doing the 'After reboot' steps), make it an image, flash another one with this image and apply the [after reboot](#After-reboot) steps on it

**Cogentee wiki:** http://cogentee.coventry.ac.uk/mediawiki/index.php/Cogentee_AutoSSH_Ports