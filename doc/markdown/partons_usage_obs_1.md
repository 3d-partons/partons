# ObservableService {#usage_obs_1}
 
[TOC]

# computeObservable task {#usage_obs_1_lab}

## XML task name {#usage_obs_1_xml}

```xml
computeObservable
```

## C++ function name {#usage_obs_1_cpp}

```cpp
PARTONS::ObservableService::computeObservable()
```

## Description {#usage_obs_1_description}

This task is for the evaluation of DVCS observable in a single kinematic point.

## XML example {#usage_obs_1_examplexml}  

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

## C++ example {#usage_obs_1_examplecpp}  

```cpp
void computeSingleKinematicsForDVCSObservable() {

    // Retrieve Observable service
    PARTONS::ObservableService* pObservableService =
            PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

    // Create Observable
    PARTONS::Observable* pObservable =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newObservable(
                    PARTONS::DVCSAllMinus::classId);

    // Create ProcessModule
    PARTONS::DVCSProcessModule* pProcessModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                    PARTONS::DVCSProcessGV08::classId);

    // Create ScalesModule
    PARTONS::ScalesModule* pScalesModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newScalesModule(
                    PARTONS::ScalesQ2Multiplier::classId);

    // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
    pScalesModule->configure(
            ElemUtils::Parameter(
                    PARTONS::ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA, 1.));

    // Create XiConverterModule
    PARTONS::XiConverterModule* pXiConverterModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(
                    PARTONS::XiConverterXBToXi::classId);

    // Create CFF module
    PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                    PARTONS::DVCSCFFStandard::classId);

    // Set its PerturbativeQCDOrder
    pDVCSCFFModel->configure(
            ElemUtils::Parameter(
                    PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE,
                    PARTONS::PerturbativeQCDOrderType::NLO));

    // Create GPDModule
    PARTONS::GPDModule* pGPDModule =
            PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(
                    PARTONS::GPDMMS13::classId);

    // Link module to each other
    pObservable->setProcessModule(pProcessModule);
    pProcessModule->setScaleModule(pScalesModule);
    pProcessModule->setXiConverterModule(pXiConverterModule);
    pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
    pDVCSCFFModel->setGPDModule(pGPDModule);

    // Load list of kinematics from file
    PARTONS::ObservableKinematic observableKinematic =
            PARTONS::ObservableKinematic(0.2, -0.1, 2., 6.);

    // Create kinematic
    PARTONS::ObservableResult observableResult =
            pObservableService->computeObservable(observableKinematic,
                    pObservable);

    // Print results
    PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__,
            observableResult.toString());

    // Remove Observable pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pObservable, 0);
    pObservable = 0;

    // Remove ProcessModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pProcessModule, 0);
    pProcessModule = 0;

    // Remove ScalesModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pScalesModule, 0);
    pScalesModule = 0;

    // Remove XiConverterModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pXiConverterModule, 0);
    pXiConverterModule = 0;

    // Remove DVCSCFFModel pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pDVCSCFFModel, 0);
    pDVCSCFFModel = 0;

    // Remove GPDModule pointer reference
    PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(
            pGPDModule, 0);
    pGPDModule = 0;
}

```
