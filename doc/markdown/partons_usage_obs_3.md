# ObservableService {#usage_obs_3}
 
[TOC]

# printResults task {#usage_obs_3_lab}

## XML task name {#usage_obs_3_namexml}

```xml
printResults
```

## C++ function name {#usage_obs_3_namecpp}

No correspondance in C++, use Logger instead.

## Description {#usage_obs_3_description}

This task is used to print out results of an observable computation to the standard output. It should be always preceded by other observable computational task. If it is preceded by two or more observable computational tasks, it will only print out the results returned by the last task.

## XML example {#usage_obs_3_examplexml}

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario starts here -->
<!-- For your convenience and for bookkeeping provide creation date and unique description -->
<scenario date="2017-07-18" description="DVCS observable evaluation for single kinematics example">

   <!-- First task: evaluate DVCS observable for a single kinematics --> 
   <!-- Indicate service and its methods to be used and indicate if the result should be stored in the database --> 
   <task service="ObservableService" method="computeObservable" storeInDB="0">

      <!-- Define DVCS observable kinematics -->
      <kinematics type="ObservableKinematic">
         <param name="xB" value="0.2" />
         <param name="t" value="-0.1" />
         <param name="Q2" value="2." />
         <param name="E" value="6." />
      </kinematics>

      <!-- Define physics assumptions -->
      <computation_configuration>

         <!-- Select DVCS observable -->
         <module type="Observable" name="DVCSAllMinus">

            <!-- Select DVCS process model -->
            <module type="ProcessModule" name="DVCSProcessGV08">

               <!-- Select scales module -->
               <!-- (it is used to evaluate factorization and renormalization scales out of kinematics) -->
               <module type="ScalesModule" name="ScalesQ2Multiplier">

                  <!-- Configure this module -->
                  <param name="lambda" value="1." />
               </module>

               <!-- Select xi-converter module -->
               <!-- (it is used to evaluate GPD variable xi out of kinematics) -->
               <module type="XiConverterModule" name="XiConverterXBToXi">
               </module>

               <!-- Select DVCS CFF model -->
               <module type="ConvolCoeffFunctionModule" name="DVCSCFFStandard">

                  <!-- Indicate pQCD order of calculation -->
                  <param name="qcd_order_type" value="NLO" />

                  <!-- Select GPD model -->
                  <module type="GPDModule" name="GPDMMS13">
                  </module>

               </module>

            </module>

         </module>

      </computation_configuration>

   </task>

   <!-- Second task: print results of the last computation into standard output --> 
   <task service="ObservableService" method="printResults">
   </task>

</scenario>
```

