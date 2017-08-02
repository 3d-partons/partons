# GPDService {#usage_gpd_2}
 
[TOC]

# computeManyKinematicOneModel task {#usage_gpd_2_lab}

## XML task name {#usage_gpd_2_namexml}

```xml
computeManyKinematicOneModel
```

## C++ function name {#usage_gpd_2_namecpp}

```cpp
PARTONS::GPDService::computeManyKinematicOneModel()
```

## Description {#usage_gpd_2_description}

This task is for the evaluation of GPD model for kinematics defined in a text file. In this file kinematic points are encoded in separate lines using the following format: "x|xi|t|MuF2|MuR2". 

## XML example {#usage_gpd_2_examplexml} 

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario starts here -->
<!-- For your convenience and for bookkeeping provide creation date and unique description -->
<scenario date="2017-07-18" description="GPD evaluation for many kinematics example">

   <!-- First task: evaluate GPD model for a single kinematics --> 
   <!-- Indicate service and its methods to be used and indicate if the result should be stored in the database --> 
   <task service="GPDService" method="computeManyKinematicOneModel" storeInDB="0">

      <!-- Define GPD kinematics -->
      <kinematics type="GPDKinematic">

         <!-- Path to file defining kinematics -->
         <param name="file" value="/home/partons/git/partons-example/data/examples/gpd/kinematics_gpd.csv" />
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

## C++ example {#usage_gpd_2_examplecpp} 

```cpp
void computeManyKinematicsForGPD() {

    // Retrieve GPD service
    PARTONS::GPDService* pGPDService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::GPDKinematic> gpdKinematicList =
            PARTONS::KinematicUtils().getGPDKinematicFromFile(
                    "/home/partons/git/partons-example/bin/examples/kinematics_gpd.csv");

    // Run computation
    PARTONS::List<PARTONS::GPDResult> gpdResultList =
            pGPDService->computeManyKinematicOneModel(gpdKinematicList,
                    pGPDModel);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            gpdResultList.toString());

    // Remove pointer reference ; Module pointers are managed by PARTONS.
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModel, 0);
    pGPDModel = 0;
}

```

## Content of kinematics_gpd.csv file {#usage_gpd_2_content} 
```xml
0.1|0.2|-0.1|2.|2.
0.2|0.2|-0.1|2.|2.
```
