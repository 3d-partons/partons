# GPDService {#usage_gpd_1}
 
[TOC]

# computeGPDModel task {#usage_gpd_1_lab}

## XML task name {#usage_gpd_1_xml}

```xml
computeGPDModel
```

## C++ function name {#usage_gpd_1_cpp}

```cpp
PARTONS::GPDService::computeGPDModel()
```

## Description {#usage_gpd_1_description}

This task is for the evaluation of GPD model in a single kinematic point.

## XML example {#usage_gpd_1_examplexml}  

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

## C++ example {#usage_gpd_1_examplecpp}  

```cpp
void computeSingleKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Create a GPDKinematic(x, xi, t, MuF, MuR) to compute
    PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

    // Run computation
    PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic,
            pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResult.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

```
