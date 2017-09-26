# %PARTONS on GNU/Linux {#linux}

[TOC]

# Introduction {#linux_intro}

This tutorial is for building %PARTONS on Linux. It describes the procedure for a Debian-like distribution (Ubuntu, etc). For RPM-based distributions, it should not be too different.

<hr>

# Setup {#linux_setup}

## Installing the external libraries {#linux_libraries}

%All the external libraries should be available through the package manager:

```sh
sudo apt-get install libeigen3-dev libcln-dev libsfml-dev libqt4-dev libqt4-sql-mysql
```

You can use any of your favorite dpkg front-ends (apt, aptitude, apt-get...), but be sure that it installs the recommended packages as dependencies. On Debian, you might want to use `su` to switch to `root` first, instead of using `sudo` directly from your current user. For an old Debian, it might be necessary to build libraries from their sources, as *e.g.* the version of SFML from Debian Wheezy does not work.

## CMake {#linux_cmake}

CMake is the main tool to achieve multi-platform compilation. Install it with your package manager:
```sh
sudo apt-get install cmake
```

## PARTONS source code {#linux_source}

Visit [this page](@ref download) to download %PARTONS' source code. To achieve this, you may need the Git command line tool. It can be installed in the following way:
```sh
sudo apt-get install git
```

# Building the source code {#linux_build}

This section is for building the projects without using any Integrated Development Environment (IDE). Skip it and see [this tutorial](@ref eclipse) if you want to use Eclipse. 

We will detail the procedure in the case of [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils). For other %PARTONS sub-projects, the procedure is exactly the same.

Go to the `build` directory and call CMake in the following way:
```sh
cd elementary-utils/build/
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug
```

You can replace `-DCMAKE_BUILD_TYPE=Debug` with `-DCMAKE_BUILD_TYPE=Release` to speed up the executable execution, if you do not need to use a debugger.

If everything went fine, you can build the project with
```sh
make
```

You need to repeat the procedure for the other projects, that is for [numa](https://drf-gitlab.cea.fr/partons/core/numa), [partons](https://drf-gitlab.cea.fr/partons/core/partons) and [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example). Always do the compilation in the order that has been specified here, i.e. [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils) first and [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) last, as the projects depend on each other. 

<hr>

# Installing the PARTONS libraries {#linux_install}

You can keep the generated shared libraries `libElementaryUtils.so`, `libNumA++.so` and `libPARTONS.so` in their folders (`lib` in each sub-project), and use them from there. Alternatively, you can install these libraries to your system with this command:
```sh
make install
```
which should be executed from the `build` directory as described above for `make`.

By default, CMake chooses system folders for the installation (such as `/usr` or `/usr/local`). You need root privileges to install to these folders - use `sudo` or `su` to grant the access if you have it. If you wish to install to local user folders (for example somewhere in `/home`), which does not require root privileges, you need to change the CMake variable `CMAKE_INSTALL_PREFIX`, e.g.:
```sh
cmake -G"Unix Makefiles" ../ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="/home/youruser/somefolder"
```
instead of what was specified [above](@ref linux_build).

This will allow you to install the headers to `/home/youruser/somefolder/include/PARTONS` and the shared libraries to `/home/youruser/somefolder/lib/PARTONS` instead of *e.g.* `/usr/local/include/PARTONS` and `/usr/local/lib/PARTONS`.

<hr>

# Building your own project using PARTONS {#linux_buildperso}

The project [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) is meant to serve as an example for projects making use of %PARTONS libraries. You can use it as a template for your own projects.

Except the main function encoded in `main.cpp`, you may be also interested in the following components of [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example), that you can adapt to your needs: 
* `CMakeLists.txt`: CMake configuration file;
* `cmake/Modules/FindXXXX.cmake`: CMake modules for finding external libraries;
* `partons.properties`, `logger.properties` and `environment_configuration.dat`: %PARTONS configuration files (for more details see [this tutorial](@ref config)).

