# GPD service task {#usage_gpd_1}
 
[TOC]

# Task name {#usage_gpd_1_name}

~~~~~~~~~~~~~{.xml} 
computeGPDModel
~~~~~~~~~~~~~

# Description {#usage_gpd_1_description}

This task is used to evaluate a given GPD model with defined kinematics. You can define any number of kinematic points, just replicate as many `<kinematics type="GPDKinematic"></kinematics>` blocks as you want. 

# Example {#usage_gpd_1_example}  

* Input XML scenario:
~~~~~~~~~~~~~{.xml}
<!-- XML prolog -->
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario: date and description -->
<scenario date="2017-06-15" description="How to compute GPD">

   <!-- Task: service name, method name and switch to store results in the database -->
   <task service="GPDService" method="computeGPDModel" storeInDB="0">

      <!-- Kinematics: type -->
      <kinematics type="GPDKinematic">

         <!-- Parameter indicating values of input kinematics -->
         <param name="x" value="0.15" />
         <param name="xi" value="0.1" />
         <param name="t" value="-0.1" />
         <param name="MuF2" value="2." />
         <param name="MuR2" value="2." />
      </kinematics>

      <!-- Computation configuration -->
      <computation_configuration>

         <!-- Module: type and name -->
         <module type="GPDModule" name="GK11Model">
         </module>
      </computation_configuration>
   </task>
</scenario>
~~~~~~~~~~~~~
