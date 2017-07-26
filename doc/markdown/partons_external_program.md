# Using the %PARTONS library in your program {#external_program}

[TOC]

It is highly recommended to first read the [tutorial](@ref usage) explaining the %PARTONS library before reading this one. Once you understand how to use the Services and methods in %PARTONS, we can move on to the practical aspects of writing your program.

# PARTONS_example {#partons-example}

For this tutorial, it is useful to have the example project [`partons-example`](https://drf-gitlab.cea.fr/partons/core/partons-example) already set up. It will serve as illustration, and you can base your own program on it for a start.

If you are using our Virtual Machine, the project is already set up in the IDE Eclipse.
To set it up in your own machine, you can follow the Eclipse tutorial [here](@ref eclipse). It is also useful to read the tutorial on how to build and install %PARTONS before, either for [Linux](@ref linux) or [Mac](@ref mac).

This executable program serves as an out of the box tool to run XML scenarios (see the next section). It also features several C++ examples in the file `examples.cpp`.

The following sections dissect the `main.cpp` file of this project in order to explain how to build your own executable project. 

# Use the XML interface in your C++ project {#use_xml_interface}

This section will explain how to use the Automation services of %PARTONS to read the XML scenarios presented [here](@ref usage_xml). The example project `partons-example` already implements this and lets you use a given XML file just by giving its path as an argument to the executable, *e.g*
```sh
./partons-example path/to/XML/file.xml
```
You can also read multiple files at once, by giving multiple arguments in chain.

The relevant corresponding code (adapted to be self-contained) is the following:
```cpp
// Retrieve automation service
AutomationService* pAutomationService =
                pPartons->getServiceObjectRegistry()->getAutomationService();
// Parse XML file
Scenario* pScenario = pAutomationService->parseXMLFile(
                    "path/to/XML/file.xml");
// Play scenario
pAutomationService->playScenario(pScenario);
```
In `partons-example`, it is of course more interesting because the path of the file is defined in the argument, so there is no need to hard-code it and rebuild the project when you change your computation.

To use command-line arguments within Eclipse, you can right-click on the project in the **Project Explorer view**, then **Run as** > **Run Configurations** and then under **C/C++ Application** > **PARTONS_example** (or whatever the name of your project), go the **Arguments** tab and add there the paths to the XML scenarios you want to launch:

![](../images/eclipse_run_arguments.png "Eclipse run configurations - arguments")

[comment]: # (The following was in the old documentation, but it doesn't seem to be implemented... I commented it out for now.)

[comment]: # (Alternatively, you can read an XML file but only consider the configuration of the modules, and then treat the rest within the C++ interface. Here is an example)

# Write your external program {#write_external_program}

Let us examine the `main` function in `partons-example` and keep only the parts which are always needed:

```cpp
int main(int argc, char** argv) {
    QCoreApplication a(argc, argv);
    Partons* pPartons = 0;

    try {
        // Init PARTONS application
        pPartons = Partons::getInstance();
        pPartons->init(argc, argv);

        // Your program goes here
        // ...
        // ...
        // Organize your code as you wish by calling your own methods and classes making use of PARTONS.
       
      // If there is an exception
    } catch (const ElemUtils::CustomException &e) {
        pPartons->getLoggerManager()->error(e);
        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }
    } catch (const std::exception &e) {
        pPartons->getLoggerManager()->error("main", __func__, e.what());
        // Close PARTONS application properly
        if (pPartons) {
            pPartons->close();
        }
    }

    // Close PARTONS application properly
    if (pPartons) {
        pPartons->close();
    }

    return 0;
}
```

Some remarks:
* It is not mandatory but still highly recommended to keep the `try`-`catch` mechanism, in order to catch exceptions and display with the Logger the associated error messages.
* `Partons` is a singleton object that serves to initialize all the other singletons (registry, factory, services, logger, etc) and configure them accordingly. You should have well set up configurations files at this point for this initialization to work. See the [tutorial](@ref config) about the configuration.
* The following includes are necessary for the fraction of code displayed above:
```cpp
#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/logger/LoggerManager.h>
#include <partons/Partons.h>
#include <QtCore/qcoreapplication.h> // or <QCoreApplication>
```
Be sure to include all the needed headers. Eclipse can do it for you automatically with `Ctrl`+`Shift`+`O` if it was configured correctly as in the related [tutorial](@ref eclipse).
* You can organize your code as you wish, with your own classes and functions. If you wish to inherit modules from %PARTONS to use your own physics models, you can read the corresponding [tutorial](@ref newmodule).

