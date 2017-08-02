# ConvolCoeffFunctionService {#usage_cff_1}
 
[TOC]

# computeWithGPDModel task {#usage_cff_1_lab}

## XML task name {#usage_cff_1_xml}

```xml
computeWithGPDModel
```

## C++ function name {#usage_cff_1_cpp}

```cpp
PARTONS::ConvolCoeffFunctionService::computeForOneCCFModel()
```

## Description {#usage_cff_1_description}

<!--
This task is for the evaluation of DVCS Compton Form Factors (CFF) in a single kinematic point. 
-->

## XML example {#usage_cff_1_examplexml}  

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

## C++ example {#usage_cff_1_examplecpp}  

```cpp
void computeSingleKinematicsForDVCSComptonFormFactor() {

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

    // Create kinematic
    PARTONS::DVCSConvolCoeffFunctionKinematic cffKinematic =
            PARTONS::DVCSConvolCoeffFunctionKinematic(0.01, -0.1, 4., 4., 4.);

    // Run computation
    PARTONS::DVCSConvolCoeffFunctionResult cffResult =
            pDVCSConvolCoeffFunctionService->computeForOneCCFModel(cffKinematic,
                    pDVCSCFFModule);

    // Print results for DVCSCFFModule
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            cffResult.toString());

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
