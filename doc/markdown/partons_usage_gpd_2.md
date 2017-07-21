# GPD service task {#usage_gpd_2}
 
[TOC]

# Task name {#usage_gpd_2_name}

~~~~~~~~~~~~~{.xml}
computeManyKinematicOneModel
~~~~~~~~~~~~~

# Description {#usage_gpd_2_description}

This task is used to evaluate a given GPD model with kinematics defined in a text file. 

The text file may contain one or more lines, with each one corresponding to a single set of GPD kinematics described by the format:
~~~~~~~~~~~~~{.xml} 
x|xi|t|muF2|muR2
~~~~~~~~~~~~~
that is by the values of \f$x\f$, \f$\xi\f$, \f$t\f$, \f$\mu_{F}^{2}\f$ and \f$\mu_{R}^{2}\f$ variables separated by the pipe symbol `|`.

# Example {#usage_gpd_2_example} 

* Input XML scenario:
~~~~~~~~~~~~~{.xml}
<!-- XML prolog -->
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario: date and description -->
<scenario date="2017-06-15" description="Example: service/GPD, task/computeManyKinematicOneModel">

   <!-- Task: service name, method name and switch to store results in the database -->
   <task service="GPDService" method="computeManyKinematicOneModel" storeInDB="0">

      <!-- Kinematics: type -->
      <kinematics type="GPDKinematic">

         <!-- Parameter indicating path to the file defining input kinematics -->
         <param name="file" value="input_kinematics.dat" />
      </kinematics>

      <!-- Computation configuration -->
      <computation_configuration>

         <!-- Module: type and name -->
         <module type="GPDModule" name="VGGModel">
         </module>
      </computation_configuration>
   </task>
</scenario>
~~~~~~~~~~~~~

* Content of `input_kinematics.dat` file:
~~~~~~~~~~~~~{.xml}
0.1|0.2|-0.1|2.|2.
0.2|0.2|-0.1|2.|2.
~~~~~~~~~~~~~