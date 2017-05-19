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

class ConvolCoeffFunctionModule;
class Observable;
class ProcessModule;
class Task;

/**
 * @class ObservableService
 *
 * @brief \<singleton\> Used to handle and compute some pre-configured Observables.
 *
 * See also the general tutorial on [Computation Services](@ref services_computation).
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

#endif /* OBSERVABLE_SERVICE_H */
