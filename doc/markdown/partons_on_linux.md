# PARTONS on GNU/Linux          {#linux}

This tutorial is for building PARTONS on Linux. It describes the procedure for a Debian-like distribution (Ubuntu, etc). For RPM-based distros, it shouldn't be too different.

[TOC]

# Installing the libraries # {#linux_libraries}

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

# Building the projects #  {#linux_build}

This section is for building the projects without IDE. Skip it and go [below](@ref linux_eclipse) if you want to use Eclipse.

We will detail the procedure in the case of NumA++. For the other projects, it's exactly the same.

First, you need to `checkout` the project (or `update` it if already available):

~~~~~~~~~~~~~{.sh}
cd /whatever/directory/you/like
svn checkout --username#your_SVN_username https://dsm-trac.cea.fr/svn/prophet/DEVELOPMENT/NumA++
~~~~~~~~~~~~~

Now, go the subdirectory ` and call the following cmake command:

~~~~~~~~~~~~~{.sh}
cd NumA++/build/
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE#Debug
~~~~~~~~~~~~~

You can replace `-DCMAKE_BUILD_TYPE=Debug` by `-DCMAKE_BUILD_TYPE=Release` to speed up the execution, if you don't need the debugger.

If everything went fine, you can build the project:

~~~~~~~~~~~~~{.sh}
make
~~~~~~~~~~~~~

Bravo, you won! If not, sorry, you'll have to work harder.

You can repeat this for all the projects such as PARTONS, PARTONS_exe, etc.

# Using Eclipse # {#linux_eclipse}

You can install the IDE Eclipse CDT (and some useful plugins) with:

~~~~~~~~~~~~~{.sh}
sudo apt-get install eclipse-cdt eclipse-subclipse eclipse-eclox
~~~~~~~~~~~~~

You can see the [Eclipse](@ref eclipse) tutorial for how to configure Eclipse for PARTONS and use it as IDE.