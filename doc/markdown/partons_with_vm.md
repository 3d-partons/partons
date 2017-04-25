# PARTONS with our provided virtual machine          {#vm}

[TOC]

You can use a provided Virtual Machine with an out-of-the-box PARTONS development environment.

# VirtualBox       {#virtualbox}

First, install [VirtualBox](https://www.virtualbox.org/) on your host machine.

On a Debian-like (ubuntu, etc) distribution (it shouldn't be too different on Arch or a RPM-based distro):

~~~~~~~~~~~~~{.sh}
sudo apt install virtualbox
~~~~~~~~~~~~~

For other systems, see the [Download](https://www.virtualbox.org/wiki/Downloads) page.

# Guest machine      {#guest_machine}

Get the already configured guest machine on the SVN (`TODO`: find an other place to store the VM! SVN is bad!): https://dsm-trac.cea.fr/svn/prophet/DEVELOPMENT/PARTONS_VM/

Run [VirtualBox](@ref virtualbox) and go to `File` > `Import`. Select the previously downloaded file called `Debian7_64_en_[date].ova`.

# Using the VM          {#usingvm}

You can run the new VM that appears in the list on the left.

The login is `debian`, password: `debian`.

You can now start working on PARTONS! Just run the Eclipse IDE (icon on the desktop or in the dock), which is already pre-configured for an optimal development environment. See [Eclipse](@ref eclipse) for a tutorial.

To log as `root` in a terminal:

~~~~~~~~~~~~~{.sh}
su -
password : partons
~~~~~~~~~~~~~