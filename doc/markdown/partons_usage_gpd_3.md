# GPDService {#usage_gpd_3}
 
[TOC]

# printResults task {#usage_gpd_3_lab}

## XML task name {#usage_gpd_3_namexml}

```xml
printResults
```

## C++ function name {#usage_gpd_3_namecpp}

No correspondance in C++, use Logger instead.

## Description {#usage_gpd_3_description}

This task is used to print out results of a GPD computation to the standard output. It should be always preceded by other GPD computational task. If it is preceded by two or more GPD computational tasks, it will only print out the results returned by the last task.

## XML example {#usage_gpd_3_examplexml}

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario starts here -->
<!-- For your convenience and for bookkeeping provide creation date and unique description -->
<scenario date="2017-07-18" description="GPD evaluation for single kinematics example">

   <!-- First task: evaluate GPD model for a single kinematics --> 
   <!-- Indicate service and its methods to be used and indicate if the result should be stored in the database --> 
   <task service="GPDService" method="computeGPDModel" storeInDB="0">

      <!-- Define GPD kinematics -->
      <kinematics type="GPDKinematic">
         <param name="x" value="0.1" />
         <param name="xi" value="0.2" />
         <param name="t" value="-0.1" />
         <param name="MuF2" value="2." />
         <param name="MuR2" value="2." />
      </kinematics>

      <!-- Define physics assumptions -->
      <computation_configuration>

         <!-- Select GPD model -->
         <module type="GPDModule" name="GPDMMS13">
         </module>

      </computation_configuration>

   </task>

   <!-- Second task: print results of the last computation into standard output --> 
   <task service="GPDService" method="printResults">
   </task>

</scenario>
```

