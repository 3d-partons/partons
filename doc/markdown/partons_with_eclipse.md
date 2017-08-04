# Using Eclipse as IDE for %PARTONS {#eclipse}

[TOC]

# Introduction {#eclipse_intro}

This tutorial demonstrates how to use Eclipse CDT IDE in your work with %PARTONS. Eclipse is a very powerful tool that significantly improves the efficiency of code development. Due to the possibility of using plugins, Eclipse may be also used as a support for e.g. code commenting and revision controlling. 

<hr>

# Preparation of Eclipse to work with PARTONS {#eclipse_preparation}

If you are using [our virtual machine](@ref vm), you may skip this section, as Eclipse provided there is fully configured to work with %PARTONS. However, if you want to use your own installation, without the virtual machine, the tutorial found in this section will help you to configure Eclipse properly. 

## Installation {#eclipse_preparation_installation}

You will need Eclipse (platform base), Eclipse CDT (support for C++) and EGit (plugin for Git revision control, if you want to clone the repositories directly from within Eclipse). At Linux systems the installation of those component can be achieved by using the repositories:
~~~~~~~~~~~~~{.sh}
# on Debian-like (Debian, Ubuntu, etc.) distribution
sudo apt-get install eclipse-cdt eclipse-egit 
~~~~~~~~~~~~~
For other Linux distributions and operating systems some adjustments may be needed. You will need also all libraries required by the compilation of %PARTONS. This subject is however covered in the wiki pages explaining the installation of %PARTONS on [Linux](@ref linux) or [Mac](@ref mac). 

## Get PARTONS code {#eclipse_preparation_git}

I order to get %PARTONS you can either download the code manually and import the related projects in Eclipse or you can use Eclipse EGit plugin. 

### Download code manually

To download the code manually visit [this page](@ref download). 

All %PARTONS-related projects are equipped with Eclipse configuration files, so you can directly import them in Eclipse. To do so, use **File** > **Open Projects from File System** or **File** > **Import** > **General** > **File System** and choose the parent location of the projects. Select one or more entries from the list of detected projects and hit **Finish**.

If you intend to work only on your own program, it is not necessary to import all the projects in Eclipse. Instead, you can install %PARTONS libraries in your system by following the procedure described in the installation guide ([Linux](@ref linux_install) or [Mac](@ref max_install)). In that case, you need to only configure Eclipse to understand the includes of the headers, by indicating where they have been installed. See [Troubleshooting](@ref eclipse_troubleshooting) section for more information. Note however, that we always recommend to import `partons-example` in Eclipse, so that you can adapt it to your needs and use it as a start for your own program.

### Use EGit plugin

The %PARTONS project uses the Git version control system. For users who wish to clone our Git repositories but do not have developer accounts, the authorization to our GitLab server on CEA is provided exclusively via HTTPS protocol. However, you need to set up EGit (Eclipse Git plugin) to ignore SSL certificates, as that issued by CEA is often not recognized properly. To do this, navigate through the Eclipse menu: **Window** > **Preferences** > **Team** > **Git** > **Configuration** > **User Setting**. Click on **Add Entry** button that will call a new window and set `http.https://drf-gitlab.cea.fr/.sslVerify` to `false`:

![](../images/eclipse_ssl.png "Force EGit to ignore SSL certificates")

Click on **OK** button to close the new window, then hit **Apply** and again click on **OK** button to finish. 

With EGit, it is useful to use the **Git Repositories** view. To enable it, navigate through the Eclipse menu: **Window** > **Show View** > **Git Repositories** (the last one may be hidden in **Other**).

Once you have this **Git Repositories** view, click on **Clone a Git Repository** icon. In the new window, set URI to `%https://drf-gitlab.cea.fr/partons/core/partons.git`. %All other crucial elements should be filled automatically: 

![](../images/eclipse_git.png "Add new Git repository")

Click on **Next** button. You should be able to select now the branch that you want to download, most probably the one corresponding to the latest release of %PARTONS. With the branch selected, click on **Next** button. Make sure that **Import all existing Eclipse projects after clone finishes** option is selected. Hit **Finish**. You should see now the imported project in the **Project Explorer** view. Repeat the procedure for all other %PARTONS projects:
* `%https://drf-gitlab.cea.fr/partons/core/elementary-utils.git`
* `%https://drf-gitlab.cea.fr/partons/core/numa.git`
* `%https://drf-gitlab.cea.fr/partons/core/partons-example.git`

Again, it is not necessary to actually import the libraries as Eclipse projects. See the remark in the previous subsection.

## Set development environment {#eclipse_preparation_libs}

This step allows Eclipse to localize library headers used by %PARTONS. Without this step, Eclipse will mark any code that is using an unrecognized library as a wrong one. Auto-completion of includes - a very useful tool - also will not work properly. 

Navigate through the Eclipse menu: **Window** > **Preferences** > **C/C++** > **Build** > **Environment** and add the variables indicated in the following screenshot. Note, that the paths to the libraries may be different on your system. 

![](../images/eclipse_env.png "Set development environment")

The variables are used by the configuration of PARTONS-related projects, as you can see by examining their Eclipse properties. Strictly speaking, not all variables are needed by each project, e.g. [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) needs only `QT_INCLUDE_DIR`.

<hr>

# Build projects {#eclipse_preparation_build}

The build is based on CMake, which is a cross-platform make system. Make sure to have it installed on your computer (command `cmake` should be available in the terminal). If CMake is missing, we refer to the wiki pages explaining the installation of %PARTONS on [Linux](@ref linux) or [Mac](@ref mac).  

The build is automatized by Ant scripts, which are small XML files scripting the call to `cmake`, `make`, etc. Those scripts are called `build.xml` and they can be found in each %PARTONS project. Open Eclipse `Ant` view by navigating through the Eclipse menu: **Window** > **Show View** > **Ant** (the last one may be hidden in **Other**). Click on **Add Buildfiles** button, select and expand the list for the first %PARTONS project, select `build.xml` and finish with **OK** button. Alternatively, you can also drag and drop `build.xml` file from the Project explorer view to Ant view. Repeat this procedure for all other %PARTONS projects. 

You can build now each of %PARTONS projects separately. Just remember to do this in the following order: 1) `elementary-utils`, 2) `numa`, 3) `partons`, 4) `partons-example`. In `Ant` view, expand a given entry, and double click to perform one of the available operations:

![](../images/eclipse_ant.png "Ant")

# Run configuration {#eclipse_preparation_run}

If you prefer to not use the console, you can run %PARTONS within Eclipse. To achieve this, right-click in **Project Explorer** view on the name of your %PARTONS executable project (`PARTONS_example` or your own project) and select: **Run As** > **Run Configurations**. A new window will appear. Select **C/C++ Application** in the list on the left side of this window and press on **New lunch configuration** button. Set **C/C++ Application** to `PARTONS_example` executable file in `bin` folder:

![](../images/eclipse_run.png "Add new run configuration")

In **Arguments** tab you can specify argument passed to the executable, e.g. path to XML scenario that you want to launch. Click on **Apply** button and **Run** if you want to run %PARTONS immediately. Latter, use **Run** icon in Eclipse menu whenever you want to run %PARTONS (you may need to customize the toolbar to make this icon visible). 

<hr>

# Troubleshooting {#eclipse_troubleshooting}

* If Eclipse does not recognize your includes of %PARTONS headers (because for instance you did not import %PARTONS libraries as projects in Eclipse, but instead you only have installed them in your system), you need to inform Eclipse on their location. Right-click on your executable project in **Project Explorer** view, go to **Properties** > **C/C++ General** > **Paths and Symbols** > **Includes** tab > **GNU C**++ and add a link to your system installed headers (*e.g.* `/usr/local/include/PARTONS`, if you used the default installation location). The default links that can be seen in the screenshot below use the Eclipse workspace projects and therefore are not useful with the system installation. You can remove them, but do not remove the global variables such as `${QT_INCLUDE_DIR}`!.

![](../images/eclipse_paths.png "Eclipse project properties - paths and symbols")

* If the Ant view is not available within Eclipse, install the Eclipse Java Development Tools: either with **Help** > **Eclipse Marketplace** or **Help** > **Install New Software**.

* If you are using Mac you may encounter this error when using `cmake` via *Ant* scripts: 
~~~~~~~~~~~~~{.sh}
Execute failed: java.io.IOException: Cannot run program "cmake" (in directory "..."): error=13, Permission denied
~~~~~~~~~~~~~
which appears as Eclipse is unable to localize CMake executable. To solve this problem right-click on one of entries in `Ant` view and navigate through the menu: **Run us** > **External Tools Configurations**. In the new window select **Environment** tab. Click on **Select** button that will call a new window. Select **PATH** variable and click on **OK** button. Modify value by adding a path to `cmake` executable, most likely `/usr/local/bin`. Unless CMake is installed in a custom place, you can get this path by running
~~~~~~~~~~~~~{.sh}
which cmake 
~~~~~~~~~~~~~
in your terminal. With `PATH` variable modified, click on **Apply** button and **Close** one to finish.
![](../images/eclipse_ant_mac_trouble.png "Ant trouble")

* If you are developing in %PARTONS and if Eclipse formatter (activated after pressing `Ctrl`+`Shift`+`F`), modifies a file by no reason, it probably changes the way how the white spaces are taken into account. Navigate through the Eclipse menu: `Windows` > `Preferences` > `C/C++` > `Code Style` > `Formatter`. Click on `Edit` button and make sure that `Tab policy` is set to `Spaces only`. If this is not the case, change it (you may need to create a new profile). 

<hr>

# Tips of using Eclipse {#eclipse_tips}

These shortcuts are essential and can be used each time you modify a file:
* `Ctrl`+`Shift`+`F`: Eclipse handles indentation and spacing automatically.
* `Crtl`+`/`: comment/uncomment selected block of code
* `Shift`+`Alt`+`R`: change name of class, variable, etc. (Eclipse will propagate your change in the whole project) 
* `Ctrl`+`Shift`+`O`: Eclipse handles includes automatically (yes, it generates the includes for you!).
* `Ctrl`+`Space`: auto-completion (very handy!).

The last two work correctly, only if the indexer of Eclipse is up-to-date. You can update it by right-clicking on a given project, then choosing: **Index** > **Rebuild**. This can be also useful when Eclipse signals some errors in the code, due to a recent change in one of dependent projects.
