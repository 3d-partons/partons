# ConvolCoeffFunctionService {#usage_cff_3}
 
[TOC]

# printResults task {#usage_cff_3_lab}

## XML task name {#usage_cff_3_namexml}

```xml
printResults
```

## C++ function name {#usage_cff_3_namecpp}

No correspondance in C++, use Logger instead.

## Description {#usage_cff_3_description}

This task is used to print out results of a CFF computation to the standard output. It should be always preceded by other CFF computational task. If it is preceded by two or more CFF computational tasks, it will only print out the results returned by the last task.

## XML example {#usage_cff_3_examplexml}

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario starts here -->
<!-- For your convenience and for bookkeeping provide creation date and unique description -->
<scenario date="2017-07-18" description="DVCS CFF evaluation for single kinematics example">

   <!-- First task: evaluate DVCS CFF for a single kinematics --> 
   <!-- Indicate service and its methods to be used and indicate if the result should be stored in the database --> 
   <task service="ConvolCoeffFunctionService" method="computeWithGPDModel" storeInDB="0">

      <!-- Define DVCS CFF kinematics -->
      <kinematics type="DVCSConvolCoeffFunctionKinematic">
         <param name="xi" value="0.01" />
         <param name="t" value="-0.1" />
         <param name="Q2" value="4." />
         <param name="MuF2" value="4." />
         <param name="MuR2" value="4." />
      </kinematics>

      <!-- Define physics assumptions -->
      <computation_configuration>

         <!-- Select DVCS CFF model -->
         <module type="ConvolCoeffFunctionModule" name="DVCSCFFStandard">

            <!-- Indicate pQCD order of calculation -->
            <param name="qcd_order_type" value="LO" />

            <!-- Select GPD model -->
            <module type="GPDModule" name="GPDMMS13">
            </module>

         </module>

      </computation_configuration>

   </task>

   <!-- Second task: print results of the last computation into standard output --> 
   <task service="ConvolCoeffFunctionService" method="printResults">
   </task>

</scenario>
```

