# Main Page                          {#mainpage}

[TOC]

# What is PARTONS? {#mainpage_intro}

<img src='../images/logo_partons.jpg' alt='PARTONS' style='float:right; max-width:20%; height:auto; margin:2pt 10pt' />

%PARTONS is a C++ software framework dedicated to the phenomenology of Generalized Parton Distributions (GPDs). GPDs provide a comprehensive description of the partonic structure of the nucleon and contain a wealth of new information. In particular, GPDs provide a description of the nucleon as an extended object, referred to as 3-dimensional nucleon tomography, and give an access to the orbital angular momentum of quarks.

%PARTONS provides a necessary bridge between models of GPDs and experimental data measured in various exclusive channels, like Deeply Virtual Compton Scattering (DVCS) and Hard Exclusive Meson Production (HEMP). The experimental programme devoted to study GPDs has been carrying out by several experiments, like HERMES at DESY (closed), COMPASS at CERN, Hall-A and CLAS at JLab. GPD subject will be also a key component of the physics case for the expected Electron Ion Collider (EIC).

%PARTONS is useful to theorists to develop new models, phenomenologists to interpret existing measurements and to experimentalists to design new experiments. A detailed description of the project can be found [here](https://arxiv.org/abs/1512.06174).

<hr>

# Get PARTONS {#mainpage_get}

Here you can learn how to get your own version of %PARTONS. We offer two ways.

You can use our provided virtual machine with an out-of-the-box %PARTONS runtime and development environment. This is the easiest way to start your experience with %PARTONS.

[Using PARTONS with our provided Virtual Machine](@ref vm)

You can also build %PARTONS by your own on either GNU/Linux or Mac OS X. This is useful if you want to have %PARTONS on your computer without using the virtualization technology or if you want to use %PARTONS on computing farms. 

[Using PARTONS on GNU/Linux](@ref linux)

[Using PARTONS on Mac OS X](@ref mac)

<hr>

# Configure PARTONS {#mainpage_conf} 

If you are using [our virtual machine](@ref vm), you will find all configuration files set up and ready to be used. However, if you want to tune the configuration or if you have installed %PARTONS by your own, this tutorial will be helpful for you.

[Manual configuration](@ref config)

<hr>

# How to use PARTONS {#mainpage_howtouse}

At this point you should have your own version of %PARTONS available and ready to be used. We provide two ways of using %PARTONS: by executing _scenarios_ encoded in simple XML files, or by using %PARTONS library and the corresponding headers to write your own stand-alone program. The first way allows to use %PARTONS without writing a line of code in C++ and rebuilding the project, while the second way is suitable for the most complex tasks. 

[Quick guide](@ref quick)

[Using PARTONS library](@ref usage)

%PARTONS benefits from a modular structure. If you want to extend %PARTONS, you can create your own module, as for instance a new GPD model, plug it in and use all functionalities offered by the project. This tutorial will also be helpful for any development work.

[Writing your own module](@ref newmodule)

When working with %PARTONS you may want to configure and use an integrated development environment (IDE), such as Eclipse. It is almost mandatory to use IDE whenever you use %PARTONS to write your own program, or if you develop its library, e.g. by adding a new module. Note, that if your are using [our virtual machine](@ref vm), you will find Eclipse ready to be used there. 

[Using PARTONS with Eclipse CDT](@ref eclipse)

When working with %PARTONS, one may want to use also a SQL database, e.g. to keep track of results obtained in the past, to avoid repeating time-consuming computations or to store experimental results. Learn how to do this with this tutorial.

[Using PARTONS with database](@ref database)

<hr>

# Publications and talks {#mainpage_talks}

Here you can find our publications and the list of talks given on behalf of %PARTONS team.

[PARTONS publications and talks](@ref tandp)

<hr>

# Acknowledgments {#mainpage_acknow}

The work on %PARTONS has been supported by the following grants: 
* French National Research Agency, Commissariat l’Energie Atomique et aux Energies Alternatives, grant no. 12-MONU-0008-01
* U.S. Department of Energy, Office of Science, Office of Nuclear Physics, contract no. DE-AC02-06CH11357

<hr>

# License {#mainpage_licences}

The following licenses apply: [elementary-utils](https://drf-gitlab.cea.fr/partons/core/elementary-utils), [numa](https://drf-gitlab.cea.fr/partons/core/numa) and [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) sub-projects are distributed under [XXX](www.???.com) license, while [partons](https://drf-gitlab.cea.fr/partons/core/partons) sub-project is distributed under [XXX](www.???.com) license. Please, respect the licensing! 

<hr>

# Contact and newsletter {#mainpage_contact}

Description of the %PARTONS developement team and contacts can be found [here](@ref contact). If you would like to be informed on the new versions of %PARTONS, sign up for the [newsletter](@ref contact_newsletter).
