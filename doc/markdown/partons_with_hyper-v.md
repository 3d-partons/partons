# Running %PARTONS with Hyper-V on Windows OS {#hyper-v}

[TOC]

# Introduction {#Hyper-V_intro}


%PARTONS team recommends to use VirtualBox as a software for virtual machine.
In [this tutorial](@ref vm) one can find a detail instruction of the installation.

Before running %PARTONS with Hyper-V make sure that you enable it on Windows, see [the official Microsoft tutorial](https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows) for details.

# Conversion of OVA into VHD {#Hyper-V_conversion}

In order to use %PARTONS in Hyper-V, which were configured for VirtualBox the conversion to suitable formats is necessary.
First, download the image of %PARTONS' virtual machine from [this page](@ref download_vm).
The image of the virtual machine is in `.ova` format and it has to be converted into different format in order to be used in Hyper-V.
The `.ova` file is a tar-archive which can be decompressed into two files: `.vmdk` and `.ovf`.

- The `.ovf` file stores information about the machine settings (vCPUs, Memory, NIC etc.).
The `.ovf` file is a .xml-file. You can launch the file in any browser/xml-parser. In our case this file will not be needed.

- The `.vmdk` file is a disk image used by a VirtualBox and it has to be converted into a native Hyper-V diskimage (`.vhd` or `.vhdx`).
The VirtualBox provides continent tool to make neccesery conversion. Even though you are not goint to use VirtualBox later, it is good to install this softwere to make the conversion easily. After installing VirtualBox (see [this tutorial](@ref vm) for instruction), open the commandline and go to the directory where the `.vmdk` file is located and type:

~~~~~~~~~~~~~{.sh}
"c:\Program Files\Oracle\VirtualBox\VBoxManage.exe" clonemedium --format vhd infile.vmdk outfile.vhd
~~~~~~~~~~~~~

Change `infile.vmdk` into the file name of a diskimage that you obtain from extraction.
You can change `outfile.vhd` into more applicable name. 

If you installed the VirtualBox not in the default place, change `c:\Program Files\Oracle\VirtualBox\VBoxManage.exe` into the path to your `VBoxManage.exe` program.

The more detailed instruction about conversion of Virtual Box file into Hyper-V file can be found on [this website](http://blog.worldofjani.com/?p=991).

# Running PARTONS {#Hyper-V_run}

Now, open the Hyper-V Manager. Right-click on your computer name in the left panel of the Hyper-V and choose `New -> Virtual Machine…` .

<img src="../images/Hyper-V_stepA.png" title="Create New Virtual Machine" width="80%"/>


Choose the default options in the wizard up to the `Connect Virtual Hard Disk` tab.

<img src="../images/Hyper-V_stepB.png" title="Connect Virtual Hard Disk" width="50%"/>


In this tab, select `Use an existing virtual hard disk` and point the `.vhd` file that was created before, then click `Finish`.

In this moment you should be able to run the %PARTONS virtual machine in Hyper-V.

# Network in Hyper-V {#Hyper-V_network}

In order to get the VM connected to the internet you have to tie it to the Ethernet/Wireless NIC Card. In order to do this you have to create a Virtual Switch.

1. Open your Hyper-V Manager.

2. Select `Action -> Virtual Switch Manager`.

   <img src="../images/Hyper-V_step2.png" title="Select Virtual Switch Manager" width="80%"/>

3. Select `External` and then `Create Virtual Switch`.

   <img src="../images/Hyper-V_step3.png" title="Create Virtual Switch" width="50%"/>

4. Give the switch a name and then select the `External network` you use to connect to the internet (Ethernet, or Wireless, etc,.)

   <img src="../images/Hyper-V_step4.png" title="Select External network" width="50%"/>

   Note: You must select the way that you are actually connecting to the Internet, *i.e.* if your computer is set up to use Wifi or Ethernet, but is currently using Wifi, you must use Wifi - the Ethernet connection will not work because the host machine isn't currently using it.

5. Select `Apply` and then `OK`.

6. Right-click on the Virtual Machine you want to connect and select `Settings`.

   <img src="../images/Hyper-V_step6.png" title="Select Settings" width="80%"/>

7. Under the `Network Adapter` setting select the newly created Virtual Switch

   <img src="../images/Hyper-V_step7.png" title="Select the newly created Virtual Switch" width="50%"/>

This should enable the connection you use to connect to the Internet to be accessible to the Hyper-V.

The more detailed instruction can be found in [this tutorial](https://superuser.com/questions/469806/windows-8-hyper-v-how-to-give-vm-internet-access).

# Tips and troubleshooting {#Hyper-V_tips}

If after connecting the virtual switch there is still no Internet in the virtual machine make sure that following files looks like that:

~~~~~~~~~~~~~{.sh}
partons@partonsVM_DEV:~$ cat /etc/hostname
partonsVM_DEV
~~~~~~~~~~~~~

~~~~~~~~~~~~~{.sh}
partons@partonsVM_DEV:~$ cat /etc/hosts
127.0.0.1	localhost
127.0.1.1	partonsVM_DEV

# The following lines are desirable for IPv6 capable hosts
::1	localhost ip6-localhost ip6-loopback
ff02::1	ip6-allnodes
ff02::2	ip6-allrouters
~~~~~~~~~~~~~

~~~~~~~~~~~~~{.sh}
partons@partonsVM_DEV:~$ cat /etc/network/interfaces
# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

source /etc/network/interfaces.d/*

# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
auto eth0
iface eth0 inet dhcp
~~~~~~~~~~~~~

Any of this file can be edit using command `sudo nano path_to_file`.
After making necessary changes use command

~~~~~~~~~~~~~{.sh}
sudo /etc/init.d/networking restart
~~~~~~~~~~~~~

to restart Internet connection. 