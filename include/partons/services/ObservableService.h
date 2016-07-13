#ifndef OBSERVABLE_SERVICE_H
#define OBSERVABLE_SERVICE_H

/**
 * @file ObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/GenericType.h>
#include <string>
#include <vector>

#include "../beans/List.h"
#include "../beans/observable/ObservableChannel.h"
#include "../beans/observable/ObservableKinematic.h"
#include "../beans/observable/ObservableResult.h"
#include "../ServiceObjectTyped.h"

class ConvolCoeffFunctionModule;
class GPDModule;
class Observable;
class ProcessModule;
class Task;

/**
 * @class ObservableService
 *
 * @brief \<singleton\> Use for handle and compute some pre-configured CFF modules.
 */

class ObservableService: public ServiceObjectTyped<ObservableKinematic,
        ObservableResult> {
public:
    static const std::string FUNCTION_NAME_COMPUTE_OBSERVABLE;
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL;
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    ObservableService(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~ObservableService();

    List<ObservableResult> computeManyKinematicOneModel(
            const List<ObservableKinematic> & listOfKinematic,
            Observable* pObservable);

    virtual void computeTask(Task &task);

    ObservableChannel::Type getObservableChannel(
            const std::string &observableClassName) const;

    ObservableResult computeObservable(
            const ObservableKinematic &observableKinematic,
            Observable* pObservable) const;

    void generatePlotFile(const std::string &filePath,
            std::vector<std::string> &selectParams,
            std::vector<ElemUtils::GenericType> &whereParams) const;

    Observable* configureObservable(Observable* pObservable,
            ProcessModule* pProcessModule,
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const;

    ProcessModule* configureProcessModule(ProcessModule* pProcessModule,
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule) const;

    Observable* newObservableModuleFromTask(const Task &task) const;
    ProcessModule* newProcessModuleFromTask(const Task &task) const;

private:
    //TODO improve object copy
    ObservableResult computeObservableTask(Task &task);
    List<ObservableResult> computeManyKinematicOneModelTask(Task &task);
    void generatePlotFileTask(Task &task);

};

#endif /* OBSERVABLE_SERVICE_H */
