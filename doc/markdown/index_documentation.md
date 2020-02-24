# Main Page                          {#mainpage}

# What is PARTONS? {#mainpage_intro}

<img src='../images/logo_partons_strong.png' alt='PARTONS' style='float:right; max-width:17%; height:auto; margin:2pt 10pt' />

%PARTONS is a software framework dedicated to the phenomenology of 3D hadron structure, in particular Generalized Parton Distributions (GPDs) and Tranverse Momentum Dependent (TMDs) parton distribution functions.

%PARTONS provides a necessary bridge between models of 3D hadron structure and experimental data measured in various channels, like for example Deeply Virtual Compton Scattering (DVCS) and Hard Exclusive Meson Production (HEMP).

The experimental program devoted to study GPDs and TMDs has been carrying out by experiments in several facilities, like CERN, DESY, Fermilab, Jefferson Lab and BNL. The 3D structure of hadrons will be also a key component of the physics case for the future Electron Ion Collider (EIC). %PARTONS is useful to theorists to develop new models, phenomenologists to interpret existing measurements and to experimentalists to design new experiments.

The virtual access infrastructure 3DPartons has received funding from the European Union’s Horizon 2020 research and innovation programme under grant agreement No 824093.

<hr>

# Get PARTONS {#mainpage_get}

Here you can learn how to get your own version of %PARTONS. We offer two ways. You can use our provided virtual machine with an out-of-the-box %PARTONS runtime and development environment. This is the easiest way to start your experience with %PARTONS. Alternatively, you can also build %PARTONS by your own on <!-- either --> GNU/Linux<!--  or Mac OS X -->. This is useful if you want to have %PARTONS on your computer without using the virtualization technology or if you want to use %PARTONS on computing farms. 

* [Download page](@ref download).

Tutorials:
* [Using PARTONS with our provided Virtual Machine](@ref vm).
* [Using PARTONS on GNU/Linux](@ref linux). <!-- 
* [Using PARTONS on Mac OS X](@ref mac).-->

<hr>

# Configure PARTONS {#mainpage_conf} 

If you are using [our virtual machine](@ref vm), you will find all configuration files set up and ready to be used. However, if you want to tune the configuration or if you have installed %PARTONS by your own, this tutorial will be helpful for you:

* [Manual configuration](@ref config).

<hr>

# How to use PARTONS {#mainpage_howtouse}

At this point you should have your own version of %PARTONS available and ready to be used. We provide two ways of using %PARTONS: by executing _scenarios_ encoded in simple XML files, or by using the %PARTONS library and its headers to write your own stand-alone program. The first way allows to use %PARTONS without writing a line of code in C++ and rebuilding the project, while the second way is suitable for the most complex tasks. 

%PARTONS benefits from a modular structure. If you want to extend %PARTONS, you can create your own module, as for instance a new GPD model, plug it in and use all functionalities offered by the project.

When working with %PARTONS you may want to configure and use an integrated development environment (IDE), such as Eclipse. Note, that if your are using [our virtual machine](@ref vm), you will find Eclipse already configured there and ready to be used. 

When working with %PARTONS, one may also want to use also an SQL database, e.g. to keep track of results obtained in the past, to avoid repeating time-consuming computations or to store experimental results.

The following tutorials deal with all these matters:
* [Quick guide on how to use PARTONS](@ref quick).
* [Using the PARTONS library](@ref usage).
* [Writing your own module](@ref newmodule).
* [Using PARTONS with the Eclipse CDT IDE](@ref eclipse).
* [Using PARTONS with a database](@ref database).

<hr>

# License and reference {#mainpage_licences}

The following licenses apply: [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils) sub-project is under the [Apache](https://www.apache.org/licenses/) license, while [numa](https://drf-gitlab.cea.fr/partons/core/numa), [partons](https://drf-gitlab.cea.fr/partons/core/partons) and [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) are distributed under the [GPL](https://www.gnu.org/licenses/gpl.html) license. Please, respect the licensing! 

To reference %PARTONS please use this publication:<br>
* B. Berthou *et al.*, *%PARTONS: PARtonic Tomography Of Nucleon Software: A computing platform for the phenomenology of Generalized Parton Distributions*,<br>Eur. Phys. J. C78 (2018), 478, [DOI: 10.1140/epjc/s10052-018-5948-0](https://doi.org/10.1140/epjc/s10052-018-5948-0)

<hr>

# Contact and newsletter {#mainpage_contact}

Description of the %PARTONS developement team and contacts can be found [here](@ref contact). If you would like to be informed on the new versions of %PARTONS, sign up for the [newsletter](@ref contact_newsletter).
