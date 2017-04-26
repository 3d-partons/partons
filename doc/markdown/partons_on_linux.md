# PARTONS on GNU/Linux          {#linux}

This tutorial is for building PARTONS on Linux. It describes the procedure for a Debian-like distribution (Ubuntu, etc). For RPM-based distros, it shouldn't be too different.

[TOC]

# Installing the external libraries # {#linux_libraries}

%All the needed libraries can be accessed through the package manager:

~~~~~~~~~~~~~{.sh}
sudo apt-get install libeigen3-dev libcln-dev libsfml-dev libqt4-dev libqt4-sql-mysql root-system
~~~~~~~~~~~~~

You can use any of your favourite dpkg front-ends (apt, aptitude, apt-get...), but be sure that it installs recommended packages as dependencies. In particular, we need the MathMore module of ROOT (package `libroot-math-mathmore-dev`).

On Debian, you might want to use `su -` to switch to `root` first, instead of the ubuntu way of using `sudo` directly from your current user.

For an old Debian, it might be necessary to build libraries from source (a recent SFML version is needed, the one from Debian Wheezy doesn't work).

If you do build libraries yourself from sources, be sure to use the default paths for the installation. Otherwise, the CMake scripts might not work.

# CMake and SVN # {#linux_cmake_svn}

CMake is the main tool to achieve multi-platform compilation. Install it with your package manager:

~~~~~~~~~~~~~{.sh}
sudo apt-get install cmake
~~~~~~~~~~~~~

You obviously need SVN (Subversion) too in order to retrieve the sources. So install it too if not already available:

~~~~~~~~~~~~~{.sh}
sudo apt-get install subversion
~~~~~~~~~~~~~

# Building the source code #  {#linux_build}

This section is for building the projects without IDE. Skip it and go to [the last section](@ref linux_eclipse) if you want to use Eclipse.

We will detail the procedure in the case of NumA++. For the other projects, it's exactly the same.

First, you need to `checkout` the project (or `update` it if already available):

~~~~~~~~~~~~~{.sh}
cd /whatever/directory/you/like
svn checkout --username#your_SVN_username https://dsm-trac.cea.fr/svn/prophet/DEVELOPMENT/NumA++
~~~~~~~~~~~~~

Now, go the subdirectory `build/` and call the following cmake command:

~~~~~~~~~~~~~{.sh}
cd NumA++/build/
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug
~~~~~~~~~~~~~

You can replace `-DCMAKE_BUILD_TYPE=Debug` by `-DCMAKE_BUILD_TYPE=Release` to speed up the execution, if you don't need the debugger.

If everything went fine, you can build the project:

~~~~~~~~~~~~~{.sh}
make
~~~~~~~~~~~~~

You can repeat this for all the projects such as Elementary\_Utils, PARTONS or PARTONS_release.

# Installing the PARTONS libraries {#linux_install}

You can keep the generated shared libraries Elementary\_Utils, NumA++ and PARTONS as it is in their source folder (the commande `make` will create if not present a folder `lib/` with the resulting shared library), and use them from there, or you can install these libraries if you wish, with the command:

~~~~~~~~~~~~~{.sh}
make install
~~~~~~~~~~~~~

done in the same build directory as before. But this is optional. By default, cmake chooses system folders for this installation (such as `/usr/` or `/usr/local/`). You will need root privileges to install to these folders (use `sudo make install` or `su` to grant access if you have it). If you wish to install to local user folders (somewhere in `/home` for example) without root access, you will need to change the cmake variable `CMAKE_INSTALL_PREFIX` to do so. For example, when running cmake (in the example [above](@ref linux_build)):

~~~~~~~~~~~~~{.sh}
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="/home/youruser/somefolder"
~~~~~~~~~~~~~

This will allow you to install the headers to `/home/youruser/somefolder/include/PARTONS` and the shared libraries to `/home/youruser/somefolder/lib/PARTONS` instead of probably `/usr/local/include/PARTONS` and `/usr/local/lib/PARTONS`, once you use the command `make install`.

# Building your own project using PARTONS {#linux_buildperso}

PARTONS_release is meant as an example of project making use of the PARTONS libraries. You can use it as a template for your own projects.

In particular, you can adapt the CMakeLists.txt already present in PARTONS_release, and use the same FindXXXX.cmake scripts found in the folder `cmake/Modules`.

# Using Eclipse # {#linux_eclipse}

You can install the IDE Eclipse CDT (and some useful plugins) with:

~~~~~~~~~~~~~{.sh}
sudo apt-get install eclipse-cdt eclipse-subclipse eclipse-eclox
~~~~~~~~~~~~~

You can see the [Eclipse](@ref eclipse) tutorial for how to configure Eclipse for PARTONS and use it as IDE.