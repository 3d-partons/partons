---
layout: default 
---

# Installation 

---

## Getting the code

The project is composed of three subprojects, which you need to compile one by one in the following order:
* [elementary-utils](https://github.com/3d-partons/elementary-utils) (utility methods)
* [numa](https://github.com/3d-partons/numa) (numerical analysis routines)
* [partons](https://github.com/3d-partons/partons) (the main library)

It is complemented by an additional subproject containing executables with a collection of examples:
* [partons-example](https://github.com/3d-partons/partons-example) 

All files related to the project are stored in the repository hosted on [github](https://github.com/3d-partons). Click on the links above to access the code for the four subprojects. If you are not familiar with git and github, you can check [this tutorial](https://docs.github.com/en/repositories/creating-and-managing-repositories/cloning-a-repository).

## Dependencies

The project is written in C++ using the C++11 standard. It requires the following external libraries, which can easily be found in the repositories of the most popular operating systems:
* [SFML](https://www.sfml-dev.org) (simple and fast multimedia library, in PARTONS used to handle multithreading)
* [CLN](https://www.ginac.de/CLN) (class library for numbers, used for high-precision representation of floats)
* [GSL](https://www.gnu.org/software/gsl/) (GNU scientific library, used for special functions and numerical methods)
* [Libxml2](https://gitlab.gnome.org/GNOME/libxml2) (XML parser)

In addition, the project requires the following libraries used in particle physics, which you may need to install from source:
* [LHAPDF](https://www.lhapdf.org) (tool for evaluating parton distribution functions)
* [Apfel++](https://github.com/vbertone/apfelxx) (code for the evolution of various hadron structure functions)

## Compilation, linking and installation

This step is managed by [cmake](https://cmake.org). Let us demonstrate how to download the development version of [elementary-utils](https://github.com/3d-partons/elementary-utils), prepare the compilation environment, build the library, and (optionally) install it:
```shell
# download the project using the git command-line tool
git clone https://github.com/3d-partons/elementary-utils.git

# go to the build directory
cd elementary-utils/build

# run cmake to set up the compilation
cmake ..

# make the library
make

# (optionally) install the library (admin access rights may be needed)
make install
```

The last step is optional. If you do not want to install the library in your system, make sure to download each subproject into the same directory (so they are at the same level). This will allow for straightforward detection of dependencies.

After compiling [elementary-utils](https://github.com/3d-partons/elementary-utils), [numa](https://github.com/3d-partons/numa) and [partons](https://github.com/3d-partons/partons) subprojects, as well as the [partons-example](https://github.com/3d-partons/partons-example) executable, you can check if it works by running a minumum working example [described here](usage.html).