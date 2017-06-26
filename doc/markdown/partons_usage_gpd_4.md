# GPD service task {#usage_gpd_4}
 
[TOC]

# Task name {#usage_gpd_4_name}

~~~~~~~~~~~~~{.xml}
generatePlotFile
~~~~~~~~~~~~~

# Description {#usage_gpd_4_description}

This task is used to create a file to be used in 1D plotting of GPD results with the data taken from the database. Two independant variables, acting as 'x' and 'y' in the plotting, are selected via `<task_param type="select"></task_param>` block. A set of requirerment, which can be used to select e.g. GPD type, quark flavor, computation id, etc., is defined via `<task_param type="where"></task_param>` block. The output file path is selected via `<task_param type="output"></task_param>` block. 

The most important variables to be selected in the plotting and to be used to restrict the selected data are:
| Name | Description | Type | Possible values |
| :---- | :---- | :---- | :---- |
| `x`                        | GPD kienematics \f$x\f$ 			| double       |  |
| `xi`                       | GPD kienematics \f$\xi\f$ 		| double       |  |
| `t`                        | GPD kienematics \f$t\f$ 			| double       |  |
| `MuF2`                     | GPD kienematics \f$\mu_{F}^{2}\f$ 	| double       |  |
| `MuR2`                     | GPD kienematics \f$\mu_{R}^{2}\f$ 	| double       |  |
|||||
| `quark_distribution`       | GPD value for specific quark 		| double       |  |
| `quark_distribution_plus`  | GPD singlet value for specific quark 	| double       |  |
| `quark_distribution_minus` | GPD non-singlet value for specific quark	| double       |  |
| `gluon_distribution_value` | GPD value for gluons 			| double       |  |
|||||
| `computation_id`           | Unique id of related computation		| integer      |  |
| `computation_module_name`  | Name of GPD module used in computation	| char string  |  |
| `gpd_type_short_name`      | GPD type 				| char string  | `H`, `E`, etc. see GPDType::Type |
| `quark_flavor_short_name`  | Quark flavor				| char string  | `u`, `d`, etc. see QuarkFlavor::Type |

# Example {#usage_gpd_4_example} 

* Input XML scenario to fill the database with some data (for the explanation of this task see its documentation): 
~~~~~~~~~~~~~{.xml} 
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<scenario date="2017-06-15" description="How to compute GPD">
   <task service="GPDService" method="computeGPDModel" storeInDB="1">
      <kinematics type="GPDKinematic">
         <param name="x" value="0.15" />
         <param name="xi" value="0.1" />
         <param name="t" value="-0.1" />
         <param name="MuF2" value="2." />
         <param name="MuR2" value="2." />
      </kinematics>
      <computation_configuration>
         <module type="GPDModule" name="GK11Model">
         </module>
      </computation_configuration>
   </task>
</scenario>
~~~~~~~~~~~~~
* Standard output:
~~~~~~~~~~~~~{.xml} 
~~~~~~~~~~~~~

* Input XML scenario:
~~~~~~~~~~~~~{.xml} 
<!-- XML prolog -->
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario: date and description -->
<scenario date="2017-06-15" description="How to compute GPD">

   <!-- Task: service name and method name -->
   <task service="GPDService" method="generatePlotFile">

      <!-- Variables selected to be stored in the output file -->
      <task_param type="select">
         <param name="xPlot" value="x" />
         <param name="yPlot" value="quark_distribution" />
      </task_param>

      <!-- Applied requirements -->
      <task_param type="where">
         <param name="computation_id" value="1" />
         <param name="gpd_type_short_name" value="H" />
         <param name="quark_flavor_short_name" value="u" />
      </task_param>

      <!-- Path to the output fille -->
      <task_param type="output">
         <param name="filePath" value="output.dat" />
      </task_param>
   </task>
</scenario>
~~~~~~~~~~~~~

* Content of `output.dat` file:
~~~~~~~~~~~~~{.xml} 
~~~~~~~~~~~~~
