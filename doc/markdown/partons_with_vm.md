# %PARTONS with our provided virtual machine {#vm}

[TOC]

# Introduction {#vm_intro}

You can use our provided virtual machine with an out-of-the-box %PARTONS runtime and development environment. This is the easiest way to start your experience with %PARTONS. The virtual machine comes with the latest %PARTONS version, Eclipse CDT IDE platform, all needed libraries and a predefined MySQL database. It is based on Debian 8.8 with Xfce desktop environment. 

This page explains how to install the required virtualization software, download our virtual machine, set it up and use it. It contains also a set of useful tips for troubleshooting and further improving your work.

<hr>

# Virtualization software {#vm_virtualization}

You need VirtualBox ([https://www.virtualbox.org](https://www.virtualbox.org)) on your host machine. For Windows and MacOS download the latest version of VirtualBox from [the official download page](https://www.virtualbox.org/wiki/Downloads) and install it. For Linux you should be able to install VirtualBox through the repositories:
~~~~~~~~~~~~~{.sh}
# on Debian-like (Debian, Ubuntu, etc.) distribution
sudo apt-get install virtualbox
~~~~~~~~~~~~~
For other Linux distributions and operating systems some adjustments to the installation receipt may be needed. If the installation via the repositories fails, visit [the official download page for Linux](https://www.virtualbox.org/wiki/Linux_Downloads) and follow the instruction to update your repository list or download and install the appropriate package manually.  

<hr>

# Download and set-up our virtual machine {#vm_download}

The image of our virtual machine can be downloaded from [this page](@ref download_vm). Whe the download is completed, open VirtualBox and navigate through the menu: `File` > `Import Appliance`. Select the downloaded file, click on `Next` button and finish the straightforward procedure. Now, you can run the new virtual machine that has appeared in the list on the left side of the VirtualBox window. The previously downloaded file will not be needed anymore. 

<hr>

# Using our virtual machine {#vm_usage}

To start our virtual machine double-click on the corresponding entry in the list on the left side of the VirtualBox window. Wait until the machine finishes to boot and log in with __login:__ `partons` and __password:__ `partons`. You can now start working with %PARTONS! Open the terminal (icon on the desktop and in the dock) and navigate to `git/partons-examples` to run some examples or run the Eclipse platform (icon on the desktop or in the dock), which has been pre-configured for an optimal development environment. See [Usage](@ref usage) and [Eclipse](@ref eclipse) pages for more information.

<hr>

# Tips and troubleshooting {#vm_tips}

## Keyboard layout {#vm_tips_keyboard}

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

Our virtual machine comes only with those elements that are required by either %PARTONS runtime or the development environment. However, You can install any other package from the rich repository of Debian by yourself and use it in your work. To do this, open the terminal and do as follows (here is an example for the installation of `gnuplot`): 
~~~~~~~~~~~~~{.sh}
# update list of packages
sudo apt-get update

# search a given package
apt-cache search gnuplot 

# install a given package
sudo apt-get install gnuplot 
~~~~~~~~~~~~~
Administrator (root) password is: `partons`.

## Shared folders {#vm_tips_sharedFolder}

A shared folder is the easiest way to transfer files between your host and our virtual machine. This folder can be set to be bidirectional, so any file inserted into such a folder can be accessed, modified and removed by both sides. To establish a shared folder, create a folder on your host machine (e.g. `shared_folder_host`) and an other one on the guest machine (e.g. `shared_folder_vm`). At your host machine, right-click on the entry in the list on the left side of the VirtualBox window corresponding to our virtual machine, select **Settings** > **Shared Folders**. Click on **Add Button**, set **Folder Path** to the aforementioned `shared_folder_host`, set **Folder Name** to `shared_folder_host`, check **Make Permanent** and click on **OK** button. On the guest machine, open the terminal and run:
~~~~~~~~~~~~~{.sh}
sudo mount -t vboxsf -o rw,uid=1000,gid=1000 shared_folder_host ABSOLUTE_PATH/shared_folder_vm
~~~~~~~~~~~~~ 
where `ABSOLUTE_PATH` is the absolute path to `shared_folder_vm`. Administrator (root) password is: `partons`. The shared folder should work now, try to insert a file there and check if it can be accessed by the other side. Note however, that after rebooting the virtual machine the shared folder will not work any more - it has to be mounted again. To avoid such trouble, make it permanent by modifying the script run at boot time (here with the text editor called `mousepad`):
~~~~~~~~~~~~~{.sh}
sudo mousepad /etc/rc.local 
~~~~~~~~~~~~~ 
Just add the `mount` command before the line `exit 0`:
~~~~~~~~~~~~~{.sh}
mount -t vboxsf -o rw,uid=1000,gid=1000 shared_folder_host ABSOLUTE_PATH/shared_folder_vm
exit 0
~~~~~~~~~~~~~ 

## Dual screen {#vm_tips_dualScreen}

Do you work with two screens and want the guest machine to use both of them?  At your host machine, right-click on the entry in the list on the left side of the VirtualBox window corresponding to our virtual machine, select **Settings** > **Display**. Set **Monitor Count** to `2` (to perform this operation the virtual machine must be switched off). On the guest machine, navigate through the Xfce menu: **Applications Menu** > **Settings** > **Display**. Make sure that both screens are active. Open the terminal and play with these two commands:
~~~~~~~~~~~~~{.sh}
xrandr --output VGA-0 --left-of VGA-1
xrandr --output VGA-0 --right-of VGA-1
~~~~~~~~~~~~~ 
which should help you align both screens. Unfortunately, the effect of this command will not be remembered after the reboot. To run it automatically at the start of Xfce open **Applications Menu** > **Settings** > **Session and Startup** > **Application Autostart** and add your command.

## VT-x is disabled {#vm_tips_Vtx}

Sometimes the processor virtualization support is disabled, so a virtual machine can not be open properly and the following (or similar) error is returned by VirtualBox: `VT-x is disabled in the BIOS`. This may happen in particular if you are using either a laptop or an old desktop computer. To enable the virtualization support open Bios and look for `Intel Vitualization Technology`, `AMD-V` or a similar option. Do this operation carefully - you do not want to mess with Bios!

## No network connection {#vm_tips_noNet}

After booting on the virtual machine, the network should be set up automatically (assuming that it is available at the host machine, of course). If this is not the case, check if VirtualBox has connected the network adapter. Navigate through the menu that should be visible in the window used by the running virtual machine (you may need to exit the full-screen): **Devices** > **Network**. Check if **Connect Network Adapter** option is selected. If the network has not been set up at this moment, try to reboot the virtual machine.  

## Exit full-screen mode {#vm_tips_exitFS}

You need to know your `Host` key - it should be the right `Ctrl`. Press `Host`+`F` to toggle between the full screen mode. The menu is available with the combination `Host` + `Home`.
