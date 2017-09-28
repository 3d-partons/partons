# Quick quide {#quick}

[TOC]

# Introduction {#quick_intro}

The purpose of this tutorial is to demonstrate how to run %PARTONS with no questions asked. The full tutorial for %PARTONS usage can be found [here](@ref usage). We assume that a %PARTONS installation is available to you, *i.e.* you use either [our virtual machine](@ref vm) or your own installation on [Linux](@ref linux) or [Mac](@ref mac). 

For the demonstration we will use [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) project. You can run the executable either from a terminal (the most straightforward way) or from the Eclipse IDE.

<hr>

# Run partons-example in terminal {#quick_console}

* Open the terminal.
* Go to the root directory of [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) project:
```sh
# on our virtual machine
cd /home/partons/git/partons-example
```
* Run one of the examples:
```sh
./bin/PARTONS_example data/examples/gpd/computeSingleKinematicsForGPD.xml
```
* At this point you should see the output of the played scenario: values of GPDs, including singlet and non-singlet combinations, for a single kinematics and for GPD types and partons available in the chosen model. Study the example by using, e.g. `mousepad` text editor:
```sh
mousepad data/examples/gpd/computeSingleKinematicsForGPD.xml
```
You will find more examples of XML scenarios to play with in the `data/examples` directory. You can use multiple scenarios with one command, *e.g.*
```sh
./bin/PARTONS_example "scenario1.xml" "scenario2.xml"
```
`partons-example` will play them consecutively.

<hr>

# Run partons-example in Eclipse {#quick_eclipse}

* Open Eclipse.
* Specify the example XML scenario to play: right-click on [partons-example](https://drf-gitlab.cea.fr/partons/core/partons-example) project in **Project Explorer** view, then **Run as** > **Run Configurations** and then under **C/C++ Application** > **PARTONS_example**, go to the **Arguments** tab and add there a path to the XML scenario you want to play (you may also indicate several scenarios to be played consecutively).
![](../images/eclipse_run_arguments.png "Eclipse run configurations - arguments")
* At this point you should see the output of the played scenario in the **Console** view: values of GPDs, including singlet and non-singlet combinations, for a single kinematics and for GPD types and partons available in the chosen model. Study the example by opening it via the **Project Explorer** view. You will find more examples of XML scenarios to play with in the `data/examples` directory.

