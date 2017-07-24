# Main Page                          {#mainpage}

[TOC]

# What is PARTONS {#mainpage_intro}

<img src='../images/logo_partons.jpg' alt='PARTONS' style='float:right;max-width:15%;height:auto' />

%PARTONS is a C++ software framework, dedicated to the phenomenology of Generalized Parton Distributions. %PARTONS provides a necessary bridge between models of Generalized Parton Distributions and experimental data measured in various exclusive channels. Detailed description of the project can be found here: [arxiv:1512.06174](https://arxiv.org/abs/1512.06174) @cite Berthou:2015oaw.

This framework should be useful not only for theorists to develop new models but also to interpret existing measurements and even design new experiments. For an example of the use of %PARTONS to fit the high precision DVCS data, look [here](@ref talks_publ_dis2017).

<hr>

# Get PARTONS {#mainpage_get}

This section helps you set up a functioning %PARTONS environment, while the following sections are needed to understand how to use and develop with %PARTONS. 

## With a virtual machine {#mainpage_get_vm} 

You can use our provided virtual machine with an out-of-the-box %PARTONS runtime and development environment. This is the easiest way to start your experience with %PARTONS.

[Using PARTONS with our provided Virtual Machine](@ref vm).

## On your own machine {#mainpage_get_indep} 

If you want to build %PARTONS on your own machine then you should use one of following tutorials:

[Using PARTONS on GNU/Linux](@ref linux).

[Using PARTONS on Mac OS X](@ref mac).

## Configuration {#mainpage_get_conf} 

If you are using [our virtual machine](@ref vm), all configuration files will been set up and you will be able to run %PARTONS as it was out-of-the-box there. However, if you have installed %PARTONS at your own machine, you may need to set up these files manually:

[Manual configuration](@ref config).

## Development environment {#mainpage_eclipse}

Likewise, if you use %PARTONS on your own machine, you may want to configure an IDE such as Eclipse (already set up on the VM) for an optimal development with %PARTONS:

[Using Eclipse CDT with %PARTONS](@ref eclipse).

<hr>

# How to use PARTONS {#mainpage_howtouse}

At this point you should have your own version of %PARTONS available. You can read this short tutorial below on how to use %PARTONS. We provide two methods: using simple [input XML files](@ref usage_xml), or using the %PARTONS library and its headers to write a stand-alone [program](@ref usage_cpp):

[Using PARTONS library](@ref usage).

For the practical aspects of running your program, this tutorial uses an example project to guide you through it:

[Writing you own program using PARTONS](@ref external_program).

When working with %PARTONS, one may want to use a SQL database. Learn how to do this with the following tutorial:

[Database tutorial](@ref database).

If you want to extend %PARTONS for your own purposes, you can create your own modules (for example a new GPD model, new hard scattering kernel or new observable):

[Writing you own modules](@ref newmodule).

Simple example of %PARTONS usage with XML files - XXXX. How to get the text file with a numbers.

<hr>

# Talks and publications {#mainpage_talks}

You can find the list of the talks and publications on %PARTONS [here](@ref talks_publ).

<hr>

# Acknowledgments {#mainpage_acknow}

This work on %PARTONS was supported in part by the Commissariat l’Energie Atomique et aux Energies Alternatives, by the French National Research Agency (ANR) grant ANR- 12-MONU-0008-01 and U.S. Department of Energy, Office of Science, Office of Nuclear Physics, under contract no. DE-AC02-06CH11357

<hr>

# Licences {#mainpage_licences}

??!!

<hr>

# Contact and newsletter {#mainpage_contact}

Description of the %PARTONS developement team and contacts can be found [here](@ref contact). If you would like to be informed on the new versions of %PARTONS, sign up for the [newsletter](@ref contact_newsletter).
