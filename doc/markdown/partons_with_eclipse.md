# Using Eclipse as IDE for PARTONS {#eclipse}

[TOC]

This tutorial explains how to handle Eclipse CDT IDE during your work with PARTONS. Eclipse is a very powerful tool that significantly improves the efficiency of code development. Due to the possibility of using plugins, Eclipse may be also used as a support for e.g. code commenting and revision controlling. 

# Preparation of Eclipse to work with PARTONS {#eclipse_preparation}

If you are using [our virtual machine](@ref vm), you may skip this section, as Eclipse provided there is fully configured to work with PARTONS. However, if you want to use your own installation, presumptively without using the virtualization technology, tips found here will help you to configure Eclipse properly. 

## Installation {#eclipse_preparation_instalation}

You will need Eclipse (platform base), Eclipse CDT (support for C++) and EGit (plugin for Git revision control). At Linux systems the installation of those component can be achieved by using repositories:
~~~~~~~~~~~~~{.sh}
#on Debian-like (Debian, Ubuntu, etc.) distribution
sudo apt-get install eclipse-cdt eclipse-egit 

#on Fedora
sudo dnf install eclipse-cdt eclipse-egit 
~~~~~~~~~~~~~
You will need also all libraries required by the compilation of PARTONS. This subject is however covered in the wiki pages explaining the installation of PARTONS on [Linux](@ref linux) or [Mac](@ref mac). 

## Get PARTONS code {#eclipse_preparation_git} 

PARTONS project uses the revision control based on Git. For users the authorization to our Git server is provided exclusively via HTTPS protocol. However, you need to set up EGit to ignore SSL certificates, as that issued by CEA is often not recognized. To do this, navigate through the Eclipse menu: `Windows` > `Preferences` > `Team` > `Git` > `Configuration` > `User Setting`. Click on `Add Entry` button that will call a new window and set:

![](../images/eclipse_ssl.png "Force EGit to ignore SSL certificates")

Click on `OK` button to close the new window, then hit `Apply` and again click on `OK` button to finish. 

To download PARTONS code open `Git Repositories` view by navigating through the Eclipse menu: `Window` > `Show View` > `Git Repositories` (the last one may be hidden in `Other`). Click on `Clone a Git Repository` button. In the new window set URI to `%https://drf-gitlab.cea.fr/partons/core/partons.git`. All other crucial elements should be filled automatically: 

![](../images/eclipse_git.png "Add new Git repository")

Click on `Next` button. Now, you should be able to set the branch that you want to download, most probably that related to the latest release of PARTONS. With the branch selected, click on `Next` button. Make sure that `Import all existing Eclipse project after clone finishes` option is selected. Click on `Finish` button. You should see the imported project in `Project Explorer` view. Repeat the procedure for all other PARTONS projects:
* `%https://drf-gitlab.cea.fr/partons/core/elementary-utils.git`
* `%https://drf-gitlab.cea.fr/partons/core/numa.git`
* `%https://drf-gitlab.cea.fr/partons/core/partons-exe.git`

## Set development environment {#eclipse_preparation_libs}

This step allows Eclipse to localize library headers used by PARTONS. Without this step, Eclipse will mark any code that is using an unrecognized library as a wrong one. Auto-completion of includes - a very useful tool - also will not work properly. 

Navigate through the Eclipse menu: `Window` > `Preferences` > `C/C++` > `Build` > `Environment` and add the variables indicated in the following screenshot. Note, that the paths to the libraries may be different on your system. 

![](../images/eclipse_env.png "Set development environment")

## Build projects {#eclipse_preparation_build}

The build is based on CMake, which is a cross-platform make system. Make sure to have it installed at your computer (command `cmake` should be available in the terminal). If CMake is missing, we refer to the wiki pages explaining the installation of PARTONS on [Linux](@ref linux) or [Mac](@ref mac).  

The build is automatized by Ant scripts, which are small xml files to chain the call of CMake. Those scripts are called `build.xml` and they can be found in each PARTONS project. Open Eclipse `Ant` view by navigating through the Eclipse menu: `Window` > `Show View` > `Ant` (the last one may be hidden in `Other`). Click on `Add Buildfiles` button, select and expand the list for the first PARTONS project, select `build.xml` and finish with `OK` button. Repeat this procedure for all other PARTONS projects. 

Now you can build each PARTONS project separately. Just remember to do this in the following order: 1) `elementary-utils`, 2) `numa`, 3) `partons`, 4) `partons-exe`. In `Ant` view expand a given entry, and double click to perform one of available operations:

![](../images/eclipse_ant.png "Ant")

## Run configuration {#eclipse_preparation_run}

You can run PARTONS within Eclipse, which is useful if you prefer to not use the console. To achieve that, right-click in `Project Explorer` view on the name of PARTONS execution project (`partons-exe`) and select: `Run As` > `Run Configurations`. A new window will appear. Select `C/C++ Application` in the list of the left side of this window and press on `New lunch configuration` button. Set `C/C++ Application` to `PARTONS_exe` executable file:

![](../images/eclipse_run.png "Add new run configuration")

Click on `Apply` button and `Run` if you want to run PARTONS immediately. Latter, use `Run` icon in Eclipse menu whenever you want to run PARTONS (you may need to customize the toolbar to make this icon visible). 

# Tips of using Eclipse {#eclipse_tips}

These shortcuts are essential and can be used each time you modify a file:
* `Ctrl`+`Shift`+`F`: Eclipse handles indentation and spacing automatically.
* `Ctrl`+`Shift`+`O`: Eclipse handles includes automatically (yes, it generates the includes for you!).
* `Ctrl`+`Space`: Auto-completion (very handy!).

The last two work correctly, only if the indexer of Eclipse is up-to-date. You can update it by right-clicking on a given project, then choosing: `Index` > `Rebuild`. This can be also useful when Eclipse signals some errors in the code, due to recent changes in other projects.
