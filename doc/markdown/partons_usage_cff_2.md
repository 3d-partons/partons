# ConvolCoeffFunctionService {#usage_cff_2}
 
[TOC]

# computeManyKinematicOneModel task {#usage_cff_2_lab}

## XML task name {#usage_cff_2_namexml}

```xml
computeManyKinematicOneModel
```

## C++ function name {#usage_cff_2_namecpp}

```cpp
PARTONS::ConvolCoeffFunctionService::computeForOneCCFModelAndManyKinematics()
```

## Description {#usage_cff_2_description}

This task is for the evaluation of DVCS Compton Form Factors (CFF) for kinematics defined in a text file. In this file kinematic points are encoded in separate lines using the following format: "xi|t|MuF2|MuR2|Q2".

## XML example {#usage_cff_2_examplexml} 

```xml
<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>

<!-- Scenario starts here -->
<!-- For your convenience and for bookkeeping provide creation date and unique description -->
<scenario date="2017-07-18" description="DVCS CFF evaluation for many kinematics example">

   <!-- First task: evaluate DVCS CFF for a single kinematics --> 
   <!-- Indicate service and its methods to be used and indicate if the result should be stored in the database --> 
   <task service="ConvolCoeffFunctionService" method="computeManyKinematicOneModel" storeInDB="0">

      <!-- Define DVCS CFF kinematics -->
      <kinematics type="DVCSConvolCoeffFunctionKinematic">

         <!-- Path to file defining kinematics -->
         <param name="file" value="/home/partons/git/partons-example/data/examples/cff/kinematics_dvcs_cff.csv" />
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

## C++ example {#usage_cff_2_examplecpp} 

```cpp
void computeManyKinematicsForDVCSComptonFormFactor() {

    // Retrieve service
    PARTONS::ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

    // Create GPD module with the BaseModuleFactory
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Create CFF module with the BaseModuleFactory
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
    ElemUtils::Parameters parameters(
            PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
            PARTONS::PerturbativeQCDOrderType::LO);

    // Configure DVCSCFFModule with previous parameters.
    pDVCSCFFModule->configure(parameters);

    // Link GPDModule to DVCSCFFModule
    pDVCSCFFModule->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionKinematic> cffKinematicList =
            PARTONS::KinematicUtils().getCCFKinematicFromFile(
                    "/home/partons/git/partons-example/bin/examples/kinematics_dvcs_cff.csv");

    // Run computation
    PARTONS::List<PARTONS::DVCSConvolCoeffFunctionResult> cffResultList =
            pDVCSConvolCoeffFunctionService->computeForOneCCFModelAndManyKinematics(
                    cffKinematicList, pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResultList.toString());

    // Remove DVCSCFFModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModule, 0);
    pDVCSCFFModule = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}
```

## Content of kinematics_dvcs_cff.csv file {#usage_cff_2_content} 
```xml
0.2|-0.1|2.|2.|2.
0.3|-0.1|2.|2.|2.
```
