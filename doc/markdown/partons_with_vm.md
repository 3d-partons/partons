# PARTONS with our provided virtual machine {#vm}

[TOC]

You can use our provided virtual machine with an out-of-the-box PARTONS runtime and development environment. This is the easiest way to start your experience with PARTONS. The virtual machine comes with the latest PARTONS version, Eclipse CDT IDE platform, all needed libraries and a predefined MySQL database. It is based on Debian 8.8 with Xfce desktop environment. 

This wiki page explains how to install the required virtualization software, download our virtual machine, set it up and use it. It also contains a set of useful tips for troubleshooting and further improving your work.

# Virtualization software {#vm_virtualization}

You need VirtualBox ([https://www.virtualbox.org](https://www.virtualbox.org)) on your host machine. For Windows and MacOS download the latest version of VirtualBox from [the official download page](https://www.virtualbox.org/wiki/Downloads) and install it. For Linux you should be able to install VirtualBox through the repositories:
~~~~~~~~~~~~~{.sh}
#on Debian-like (Debian, Ubuntu, etc.) distribution
sudo apt-get install virtualbox

#on Fedora
sudo dnf install virtualbox
~~~~~~~~~~~~~
If this fails, visit [the official download page for Linux](https://www.virtualbox.org/wiki/Linux_Downloads) and follow the instructions to update your repository list or download and install the appropriate package manually.  

# Download and set-up our virtual machine {#vm_download}

The appliance of our virtual machine can be downloaded from [this page](http://to-be-updated). With downloading completed, open VirtualBox and navigate through the menu: `File` > `Import Appliance`. Select the downloaded file, click `Next` and finish the straightforward procedure. Now, you can run the new virtual machine that has appeared in the list on the left side of the VirtualBox window. The previously downloaded file will not be needed anymore. 

# Using our our virtual machine {#vm_usage}

To start our virtual machine double-click on the corresponding entry in the list on the left side of the VirtualBox window. Wait until the machine stops to boot and log in with __login:__ `partons` and __password:__ `partons`. You can now start working with PARTONS! Open the terminal (icon on the desktop and in the dock) and navigate to `git/partons/partons-exe` to run some examples or run the Eclipse platform (icon on the desktop or in the dock), which has been pre-configured for an optimal development environment. See [Examples](@ref examples) and [Eclipse](@ref eclipse) wiki pages for more information.

# Tips and troubleshooting {#vm_tips}

## Keyboard layout {#vm_tips_keybord}

The default keyboard layout is English `QWERTY (US)`. The French layout `AZERTY (FR)` is available as well. To toggle between both layouts use this combination of keys: `Alt` + `Shift`. To set the French layout as the default one, open the terminal and edit the keyboard configuration file (here with the text editor called `mousepad`):
~~~~~~~~~~~~~{.sh}
sudo mousepad /etc/default/keyboard
~~~~~~~~~~~~~
Administrator (root) password is: `partons`. With the file open and editable, change 
~~~~~~~~~~~~~{.sh}
XKBLAYOUT="us,fr"
~~~~~~~~~~~~~
to
~~~~~~~~~~~~~{.sh}
XKBLAYOUT="fr,us"
~~~~~~~~~~~~~
Save and exit the file, then restart the keyboard service by running in the terminal
~~~~~~~~~~~~~{.sh}
sudo service keyboard-setup restart
~~~~~~~~~~~~~

## Additional packages {#vm_tips_packages}

Our virtual machine comes only with those elements that are required by PARTONS runtime and development environment. However, You can install any other package from a rich repository of Debian by yourself and use it in your work. To do this, open the terminal and do as follows (here the example for the installation of `vim` text editor): 
~~~~~~~~~~~~~{.sh}
#update list of packages
sudo apt-get update

#search a given package
apt-cache search vim

#install a given package
apt-get install vim
~~~~~~~~~~~~~
Administrator (root) password is: `partons`.

## Shared folders {#vm_tips_sharedFolder}

A shared folder is the easiest way to transfer files between your host and our virtual machine. This folder can be set to be bidirectional, so any file inserted into such a folder can be accessed, modified and removed by both sides. To establish the shared folder create one at your host machine (e.g. `shared_folder_host`) and other one at the virtual machine (e.g. `shared_folder_vm`). At your host machine, right-click at the entry in the list on the left side of the VirtualBox window corresponding to your virtual machine, select `Settings` > `Shared Folders`. Click on `Add Button`, set `Folder Path` to the path of the previously created folder at the host machine, set `Folder Name` to `shared_folder_host`, check `Make Permanent` and click `OK Button`. At your virtual machine open the terminal and run
~~~~~~~~~~~~~{.sh}
sudo mount -t vboxsf -o rw,uid=1000,gid=1000 shared_folder_host ABSOLUTE_PATH/shared_folder_vm
~~~~~~~~~~~~~ 
where `ABSOLUTE_PATH` is the absolute path to `shared_folder_vm`. Administrator (root) password is: `partons`. The shared folder should work now, try to insert a file there and check if it can be accessed by the other side. Note however, that after rebooting of your virtual machine the shared folder will not work any more - it has to be mounted again. To avoid such trouble, make it permanent by modifying the script run at the boot time (here with the text editor called `mousepad`):
~~~~~~~~~~~~~{.sh}
sudo mousepad /etc/rc.local 
~~~~~~~~~~~~~ 
Just add `mount` command before `exit 0` line:
~~~~~~~~~~~~~{.sh}
mount -t vboxsf -o rw,uid=1000,gid=1000 shared_folder_host ABSOLUTE_PATH/shared_folder_vm
exit 0
~~~~~~~~~~~~~ 

## Dual screen {#vm_tips_dualScreen}

Do you work with two screens and want to use both of them by our virtual machine?  At your host machine, right-click at the entry in the list on the left side of the VirtualBox window corresponding to your virtual machine, select `Settings` > `Display`. Set `Monitor Count` to `2` (to perform this operation the virtual machine must be switched off). At the virtual machine navigate through the Xfce menu: `Applications Menu` > `Settings` > `Display`. Make sure that both screens are active. Open the terminal and play with these two commands:
~~~~~~~~~~~~~{.sh}
xrandr --output VGA-0 --left-of VGA-1
xrandr --output VGA-0 --right-of VGA-1
~~~~~~~~~~~~~ 
which should help you to align both screens. Unfortunately, the effect of this command will not be remembered after the reboot. To run it automatically at the start of Xfce open `Applications Menu` > `Settings` > `Session and Startup` > `Application Autostart` and add your command.

## VT-x is disabled {#vm_tips_Vtx}

Sometimes the processor virtualization support is disabled, so a virtual machine can not be open properly and the following (or similar) error is returned by VirtualBox: `VT-x is disabled in the BIOS`. This may happen in particular if you are using either a laptop or an old desktop computer. To enable the virtualization support open Bios and look for `Intel Vitualization Technology`, `AMD-V` or similar option. Do this operation carefully - you do not want to mess with Bios!
