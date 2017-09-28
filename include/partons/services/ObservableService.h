#ifndef OBSERVABLE_SERVICE_H
#define OBSERVABLE_SERVICE_H

/**
 * @file ObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/observable/ObservableChannel.h"
#include "../beans/observable/ObservableKinematic.h"
#include "../beans/observable/ObservableResult.h"
#include "../ServiceObjectTyped.h"

namespace PARTONS {

class ConvolCoeffFunctionModule;
class Observable;
class ProcessModule;
class Task;

/**
 * @class ObservableService
 *
 * @brief Singleton used to handle and compute some pre-configured Observables.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 *
 * Please find below some examples on how to use the different functions provided by this service.
 *
 * For now, only DVCS observables are available, including charge and spin asymmetries, both \f$\phi\f$-dependent and \f$\phi\f$-integrated (Fourier coefficients), and some cross-sections.
 *
 * 1. Compute an observable (*e.g.* here the longitudinally polarized beam and target [asymmetry](@ref PARTONS::DVCSAllMinus) for negative beam charge, \f$ A_{LL} \f$) at specific kinematics (\f$x_B\f$, t, \f$Q^{2}\f$, E, \f$\phi\f$) using the GPD model `MyFavoriteGPDModel` and the standard CFF module, with *e.g.* the [Guichon-Vanderhaeghen](@ref PARTONS::DVCSProcessGV08) set of formulas for the DVCS cross-section:
 * \code{.cpp}
 // Retrieve Observable service
 PARTONS::ObservableService* pObservableService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getObservableService();

 // Create Observable
 PARTONS::Observable* pObservable = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newObservable(PARTONS::DVCSAllMinus::classId);

 // Create ProcessModule
 PARTONS::DVCSProcessModule* pProcessModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(PARTONS::DVCSProcessGV08::classId);

 // Create ScalesModule
 PARTONS::ScalesModule* pScalesModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newScalesModule(PARTONS::ScalesQ2Multiplier::classId);

 // Set its lambda parameter, so MuF2 = MuR2 = lambda * Q2
 pScalesModule->configure(ElemUtils::Parameter(PARTONS::ScalesQ2Multiplier::PARAMETER_NAME_LAMBDA, 1.));

 // Create XiConverterModule
 PARTONS::XiConverterModule* pXiConverterModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newXiConverterModule(PARTONS::XiConverterXBToXi::classId);

 // Create CFF module
 PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(PARTONS::DVCSCFFStandard::classId);

 // Set its PerturbativeQCDOrder
 pDVCSCFFModel->configure(ElemUtils::Parameter(PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PARTONS::PerturbativeQCDOrderType::NLO));

 // Create GPDModule
 PARTONS::GPDModule* pGPDModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteGPDModel::classId);

 // Link module to each other
 pObservable->setProcessModule(pProcessModule);
 pProcessModule->setScaleModule(pScalesModule);
 pProcessModule->setXiConverterModule(pXiConverterModule);
 pProcessModule->setConvolCoeffFunctionModule(pDVCSCFFModel);
 pDVCSCFFModel->setGPDModule(pGPDModule);

 // Load list of kinematics from file
 PARTONS::ObservableKinematic observableKinematic = PARTONS::ObservableKinematic(0.2, -0.1, 2., 6.);

 // Create kinematic
 PARTONS::ObservableResult observableResult = pObservableService->computeObservable(observableKinematic, pObservable);

 // Print results
 PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, observableResult.toString());

 // Remove Observable pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pObservable, 0);
 pObservable = 0;

 // Remove ProcessModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pProcessModule, 0);
 pProcessModule = 0;

 // Remove ScalesModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pScalesModule, 0);
 pScalesModule = 0;

 // Remove XiConverterModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pXiConverterModule, 0);
 pXiConverterModule = 0;

 // Remove DVCSCFFModel pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pDVCSCFFModel, 0);
 pDVCSCFFModel = 0;

 // Remove GPDModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModule, 0);
 pGPDModule = 0;
 * \endcode
 *
 * 2. The same thing can be done when dealing with many kinematics, by adapting the code with the following lines:
 \code{.cpp}
 // Load list of kinematics from file
 PARTONS::List<PARTONS::ObservableKinematic> observableKinematicList = PARTONS::KinematicUtils().getObservableKinematicFromFile("/path/to/kinematics_dvcs_observable.csv");

 // Run computation
 PARTONS::List<PARTONS::ObservableResult> observableResultList = pObservableService->computeManyKinematicOneModel(observableKinematicList, pObservable);
 \endcode
 * In the file `kinematics_dvcs_observable.csv`, kinematic points are encoded in separate lines using the following format: "xB|t|Q2|E|phi".
 *
 * 3. You can use the same methods for Fourier-type observable (*e.g.* the \f$\cos\left(\phi\right)\f$ [moment](@ref PARTONS::DVCSAllMinusCos1Phi) of the previous asymmetry). The only difference is that there is no need to define \f$ \phi \f$ in [ObservableKinematic](@ref PARTONS::ObservableKinematic).
 */

class ObservableService: public ServiceObjectTyped<ObservableKinematic,
        ObservableResult> {
public:
    static const std::string FUNCTION_NAME_COMPUTE_OBSERVABLE; ///< Name of the XML task used to compute an Observable at given kinematics.
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL; ///< Name of the XML task used to compute an Observable for a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor.
     */
    ObservableService(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~ObservableService();

    /**
     * See parent class for details.
     */
    void resolveObjectDependencies();

    /**
     * Computes an Observable at specific kinematics.
     * @param observableKinematic Kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return ObservableResult.
     */
    ObservableResult computeObservable(
            const ObservableKinematic &observableKinematic,
            Observable* pObservable,
            const List<GPDType> & listOfGPDType = List<GPDType>()) const;

    /**
     * Computes an Observable for a list of kinematics.
     * @param listOfKinematic List of kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of ObservableResult.
     */
    List<ObservableResult> computeManyKinematicOneModel(
            const List<ObservableKinematic> & listOfKinematic,
            Observable* pObservable,
            const List<GPDType> & listOfGPDType = List<GPDType>(),
            const bool storeInDB = false);

    /**
     * Method used in automation to compute given tasks.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task);

    /**
     * @brief Returns the channel of a given Observable.\ E.g.\ DVCS.
     * @param observableClassName Class name of the Observable.
     * @return ObservableChannel.
     */
    ObservableChannel::Type getObservableChannel(
            const std::string &observableClassName) const;

//    Observable* configureObservable(Observable* pObservable,
//            ProcessModule* pProcessModule) const;
//
//    ProcessModule* configureProcessModule(ProcessModule* pProcessModule,
//            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const;

    /**
     * Uses an automation task (XML file) to set specific kinematics.
     * @param task
     * @return Observable kinematics.
     */
    ObservableKinematic newKinematicFromTask(const Task &task) const;
    /**
     * Uses an automation task (XML file) to set a list of kinematics.
     * @param task
     * @return List of Observable kinematics.
     */
    List<ObservableKinematic> newListOfKinematicFromTask(
            const Task &task) const;

    /**
     * Uses an automation task (XML file) to configure an Observable.
     * @param task
     * @return Pre-configured Observable.
     */
    Observable* newObservableModuleFromTask(const Task &task) const;
    /**
     * @brief Uses an automation task (XML file) to configure a ProcessModule, e.g.\ a DVCSModule.
     * @param task
     * @return Pre-configured ProcessModule.
     */
    ProcessModule* newProcessModuleFromTask(const Task &task) const;

private:
    //TODO improve object copy
    /**
     * Method used in the automated interface to compute an Observable.
     * @param task Automated XML task.
     * @return ObservableResult object.
     */
    ObservableResult computeObservableTask(Task &task);
    /**
     * Method used in the automated interface to compute an Observable for a list of kinematics.
     * @param task Automated XML task.
     * @return List of ObservableResult.
     */
    List<ObservableResult> computeManyKinematicOneModelTask(Task &task);
    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

};

} /* namespace PARTONS */

#endif /* OBSERVABLE_SERVICE_H */
