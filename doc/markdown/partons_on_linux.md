# PARTONS on GNU/Linux {#linux}

[TOC]

# Introduction {#linux_intro}

This tutorial is for building PARTONS on Linux. It describes the procedure for a Debian-like distribution (Ubuntu, etc). For RPM-based distros, it shouldn't be too different.

<hr>

# Setup {#linux_setup}

## Installing the external libraries {#linux_libraries}

%All the needed libraries can be accessed through the package manager:

~~~~~~~~~~~~~{.sh}
sudo apt-get install libeigen3-dev libcln-dev libsfml-dev libqt4-dev libqt4-sql-mysql root-system
~~~~~~~~~~~~~

You can use any of your favorite dpkg front-ends (apt, aptitude, apt-get...), but be sure that it installs recommended packages as dependencies. In particular, we need the MathMore module of ROOT (package `libroot-math-mathmore-dev`).

On Debian, you might want to use `su -` to switch to `root` first, instead of the ubuntu way of using `sudo` directly from your current user.

For an old Debian, it might be necessary to build libraries from source (a recent SFML version is needed, the one from Debian Wheezy doesn't work).

If you do build libraries yourself from sources, be sure to use the default paths for the installation. Otherwise, the CMake scripts might not work.

## CMake {#linux_cmake}

CMake is the main tool to achieve multi-platform compilation. Install it with your package manager:

~~~~~~~~~~~~~{.sh}
sudo apt-get install cmake
~~~~~~~~~~~~~

## Source code {#linux_source}

If you want to retrieve directly the sources from our GitLab repository, you obviously need Git. So install it too if not already available:

~~~~~~~~~~~~~{.sh}
sudo apt-get install git
~~~~~~~~~~~~~

You can then go to a folder of your liking and clone the repository of **PARTONS** (and of its needed dependencies ; **ElementaryUtils** and **NumA++**):

~~~~~~~~~~~~~{.sh}
cd /path/to/some/directory
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/elementary-utils.git --branch v1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/numa.git --branch v1
GIT_SSL_NO_VERIFY=true git clone https://drf-gitlab.cea.fr/partons/core/partons.git --branch v1
~~~~~~~~~~~~~

The option `--branch` is needed to checkout the *release* branch (in that case the first version). If you want to checkout `master`, don't use the option.

The option `GIT_SSL_NO_VERIFY=true` is needed because the CEA certificate is often not recognized.

You can also just download the source code of those projects from [GitLab](https://drf-gitlab.cea.fr/partons/core/) or our website. **TODO: add link to website when available.**

<hr>

# Building the source code {#linux_build}

This section is for building the projects without IDE. Skip it and go to [the last section](@ref linux_eclipse) if you want to use Eclipse.

We will detail the procedure in the case of ElementaryUtils. For the other projects, it's exactly the same.

Go to the subdirectory `build/` of the project and call the following cmake command:

~~~~~~~~~~~~~{.sh}
cd elementary-utils/build/
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug
~~~~~~~~~~~~~

You can replace `-DCMAKE_BUILD_TYPE=Debug` by `-DCMAKE_BUILD_TYPE=Release` to speed up the execution, if you don't need the debugger.

If everything went fine, you can build the project:

~~~~~~~~~~~~~{.sh}
make
~~~~~~~~~~~~~

You can repeat this for the other projects NumA++ and PARTONS. The dependencies follow this order.

Note that if you intend to use the installation described in the following section, you may want to install ElementaryUtils before building NumA++ (and installing NumA++ before building PARTONS) so that each subsequent library uses the installed headers and shared library instead of the temporary ones located in the source folder and that may be removed after the installation.

<hr>

# Installing the PARTONS libraries {#linux_install}

You can keep the generated shared libraries ElementaryUtils, NumA++ and PARTONS as it is in their source folder (the command `make` will create if not present a folder `lib/` with the resulting shared library), and use them from there, or you can install these libraries if you wish, with the command:

~~~~~~~~~~~~~{.sh}
make install
~~~~~~~~~~~~~

done in the same build directory as before. But this is optional. By default, cmake chooses system folders for this installation (such as `/usr/` or `/usr/local/`). You will need root privileges to install to these folders (use `sudo make install` or `su` to grant access if you have it). If you wish to install to local user folders (somewhere in `/home` for example) without root access, you will need to change the cmake variable `CMAKE_INSTALL_PREFIX` to do so. For example, when running cmake (in the example [above](@ref linux_build)):

~~~~~~~~~~~~~{.sh}
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="/home/youruser/somefolder"
~~~~~~~~~~~~~

This will allow you to install the headers to `/home/youruser/somefolder/include/PARTONS` and the shared libraries to `/home/youruser/somefolder/lib/PARTONS` instead of probably `/usr/local/include/PARTONS` and `/usr/local/lib/PARTONS`, once you use the command `make install`.

<hr>

# Building your own project using PARTONS {#linux_buildperso}

The project [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) is meant as an example of project making use of the PARTONS libraries. You can use it as a template for your own projects.

In particular, you can adapt the CMakeLists.txt already present in PARTONS_release, and use the same FindXXXX.cmake scripts found in the folder `cmake/Modules`.

You should also find the configuration files `partons.properties` and `logger.cfg` that you will have to adapt with your paths. See the [tutorial](@ref config) about the configuration.

<hr>

# Using Eclipse {#linux_eclipse}

You can install the IDE Eclipse CDT (and some useful plugins) with:

~~~~~~~~~~~~~{.sh}
sudo apt-get install eclipse-cdt eclipse-egit eclipse-eclox
~~~~~~~~~~~~~

You can see the [Eclipse](@ref eclipse) tutorial for how to configure Eclipse for PARTONS and use it as IDE.
