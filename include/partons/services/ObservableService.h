#ifndef OBSERVABLE_SERVICE_H
#define OBSERVABLE_SERVICE_H

/**
 * @file ObservableService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date November 20, 2014
 * @version 1.0
 */

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/GenericType.h>
#include <ElementaryUtils/PropertiesManager.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <ElementaryUtils/string_utils/StringUtils.h>
#include <ElementaryUtils/thread/Packet.h>
#include <string>

#include "../beans/automation/BaseObjectData.h"
#include "../beans/automation/Task.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../beans/observable/ObservableResult.h"
#include "../modules/observable/DVCS/DVCSObservable.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/DVCS/DVCSProcessModule.h"
#include "../modules/process/ProcessModule.h"
#include "../ModuleObjectFactory.h"
#include "../Partons.h"
#include "../ServiceObjectTyped.h"

namespace PARTONS {

/**
 * @class ObservableService
 *
 * @brief Abstract class for a service to handle and compute observables.
 */
template<typename KinematicType, typename ResultType>
class ObservableService: public ServiceObjectTyped<KinematicType, ResultType> {

public:

    static const std::string FUNCTION_NAME_COMPUTE_OBSERVABLE; ///< Name of the XML task used to compute an Observable at given kinematics.
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL; ///< Name of the XML task used to compute an Observable for a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Default constructor.
     */
    ObservableService(const std::string &className) :
            ServiceObjectTyped<KinematicType, ResultType>(className) {
    }
    /**
     * Destructor.
     */
    virtual ~ObservableService();

    /**
     * See parent class for details.
     */
    void resolveObjectDependencies() {

        ServiceObject::resolveObjectDependencies();

        try {
            ServiceObjectTyped<KinematicType, ResultType>::m_batchSize =
                    ElemUtils::GenericType(
                            ElemUtils::PropertiesManager::getInstance()->getString(
                                    "observable.service.batch.size")).toUInt();
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    e.what());
        }
    }

    /**
     * Method used in automation to compute given tasks.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task) {

        ServiceObjectTyped<KinematicType, ResultType>::computeTask(task);

        List<ResultType> resultList;

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
            resultList = computeManyKinematicOneModelTask(task);
        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::FUNCTION_NAME_COMPUTE_OBSERVABLE)) {
            resultList.add(computeObservableTask(task));
        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        }

        else if (!ServiceObjectTyped<KinematicType, ResultType>::computeGeneralTask(
                task)) {
            ServiceObjectTyped<KinematicType, ResultType>::errorUnknownMethod(
                    task);
        }

        ServiceObjectTyped<KinematicType, ResultType>::updateResultInfo(resultList,
                ServiceObjectTyped<KinematicType, ResultType>::m_resultInfo);

//        if (task.isStoreInDB()) {
//            ObservableResultDaoService observableResultDaoService;
//            int computationId = observableResultDaoService.insert(resultList);
//
//            if (computationId != -1) {
//                this->info(__func__,
//                        ElemUtils::Formatter()
//                                << "ObservableResultList object has been stored in database with computation_id = "
//                                << computationId);
//            } else {
//                throw ElemUtils::CustomException(getClassName(), __func__,
//                        "ObservableResultList object : insertion into database failed");
//            }
//        }

        ServiceObjectTyped<KinematicType, ResultType>::m_resultListBuffer =
                resultList;
    }

    /**
     * Computes an Observable for a list of kinematics.
     * @param listOfKinematic List of kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of ObservableResult.
     */
    List<ResultType> computeManyKinematicOneModel(
            const List<KinematicType> & listOfKinematic,
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType> & listOfGPDType = List<GPDType>(),
            const bool storeInDB = false) {

        //print information
        this->info(__func__,
                ElemUtils::Formatter() << listOfKinematic.size()
                        << " Observable kinematic(s) will be computed with "
                        << pObservable->getClassName());

        //initialize
        List<ResultType> results;
        List<ElemUtils::Packet> listOfPacket;
        List<GPDType> finalListOfGPDType = listOfGPDType;

        //TODO implement getFinalGPDTypeList() check CCFService
        // if (finalListOfGPDType.size() != 0) {

        //init thread
        ServiceObjectTyped<KinematicType, ResultType>::initComputationalThread(
                pObservable);

        //print info
        this->info(__func__, "Thread(s) running ...");

        //batch feature
        unsigned int i = 0;
        unsigned int j = 0;

        //divide to packets
        while (i != listOfKinematic.size()) {

            listOfPacket.clear();
            j = 0;

            while ((j
                    != ServiceObjectTyped<KinematicType, ResultType>::m_batchSize)
                    && (i != listOfKinematic.size())) {

                ElemUtils::Packet packet;
                KinematicType kinematic;
                kinematic = listOfKinematic[i];
                packet << kinematic << finalListOfGPDType;
                listOfPacket.add(packet);
                i++;
                j++;
            }

            //add lunch and sort
            ServiceObjectTyped<KinematicType, ResultType>::addTasks(
                    listOfPacket);
            ServiceObjectTyped<KinematicType, ResultType>::launchAllThreadAndWaitingFor();
            ServiceObjectTyped<KinematicType, ResultType>::sortResultList();

            //print info
            this->info(__func__,
                    ElemUtils::Formatter() << "Kinematic(s) already computed: "
                            << i);

            //update result info
            ServiceObjectTyped<KinematicType, ResultType>::updateResultInfo(
                    ServiceObjectTyped<KinematicType, ResultType>::getResultList(),
                    ServiceObjectTyped<KinematicType, ResultType>::m_resultInfo);

            //add to output
            results.add(
                    ServiceObjectTyped<KinematicType, ResultType>::getResultList());

            //clear buffer
            ServiceObjectTyped<KinematicType, ResultType>::clearResultListBuffer();
        }

        //clear threads
        ServiceObjectTyped<KinematicType, ResultType>::clearAllThread();

        /*
         } else {
         this->info(__func__,
         "Nothing to compute with your computation configuration ; there is no GPDType available");
         }
         */

        return results;
    }

    /**
     * Computes an Observable at specific kinematics.
     * @param observableKinematic Kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return ObservableResult.
     */
    ResultType computeObservable(const KinematicType &observableKinematic,
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType> & listOfGPDType = List<GPDType>()) const {
        return pObservable->compute(observableKinematic, listOfGPDType);
    }

    /**
     * Uses an automation task (XML file) to configure an Observable.
     * @param task
     * @return Pre-configured Observable.
     */
    Observable<KinematicType, ResultType>* newObservableModuleFromTask(
            const Task &task) const {

        //initialize
        Observable<KinematicType, ResultType>* pObservable = 0;

        //check if available
        //TODO remove hardcoded string
        if (ElemUtils::StringUtils::equals(
                task.getModuleComputationConfiguration().getModuleType(),
                "Observable")) {

            //configure
            //TODO DVCS HERE!!!!!!!!!!!!!!!!!!!!!!
            pObservable =
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSObservable(
                            task.getModuleComputationConfiguration().getModuleClassName());

            pObservable->configure(
                    task.getModuleComputationConfiguration().getParameters());

            pObservable->prepareSubModules(
                    task.getModuleComputationConfiguration().getSubModules());
        } else {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "You have not provided any Observable");
        }

        //return
        return pObservable;
    }

    /**
     * @brief Uses an automation task (XML file) to configure a ProcessModule, e.g.\ a DVCSModule.
     * @param task
     * @return Pre-configured ProcessModule.
     */
    ProcessModule<KinematicType>* newProcessModuleFromTask(
            const Task &task) const {

        //initialize
        ProcessModule<KinematicType>* pProcessModule = 0;

        //check if available
        //TODO remove hardcoded string
        if (ElemUtils::StringUtils::equals(
                task.getModuleComputationConfiguration().getModuleType(),
                "ProcessModule")) {

            //configure
            //TODO DVCS HERE!!!!!!!!!!!!!!!!!!!!!!
            pProcessModule =
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSProcessModule(
                            task.getModuleComputationConfiguration().getModuleClassName());

            pProcessModule->configure(
                    task.getModuleComputationConfiguration().getParameters());

            pProcessModule->prepareSubModules(
                    task.getModuleComputationConfiguration().getSubModules());
        } else {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    ElemUtils::Formatter()
                            << "You have not provided any ProcessModule ; Or check case in your XML file");
        }

        //return
        return pProcessModule;
    }

    /**
     * Uses an automation task (XML file) to set specific kinematics.
     * @param task
     * @return Observable kinematics.
     */
    virtual KinematicType newKinematicFromTask(const Task &task) const = 0;

    /**
     * Uses an automation task (XML file) to set a list of kinematics.
     * @param task
     * @return List of Observable kinematics.
     */
    virtual List<KinematicType> newListOfKinematicFromTask(
            const Task &task) const = 0;

private:

    //TODO improve object copy
    /**
     * Method used in the automated interface to compute an Observable.
     * @param task Automated XML task.
     * @return ObservableResult object.
     */
    ResultType computeObservableTask(Task &task) {

        //create a kinematic and init it with a list of parameters
        KinematicType kinematic = newKinematicFromTask(task);

        //get GPD types
        //TODO DOES IT WORK?
        List<GPDType> listOfGPDType;
        try {
            listOfGPDType =
                    ServiceObjectTyped<KinematicType, ResultType>::getGPDTypeListFromTask(
                            task);
        } catch (const ElemUtils::CustomException &e) {
            //TODO why ?
            //Nothing to do
        }

        //get configured observable module
        Observable<KinematicType, ResultType>* pObservable =
                newObservableModuleFromTask(task);

        //make computation
        ResultType result = computeObservable(kinematic, pObservable,
                listOfGPDType);

        //remove reference to pConvolCoeffFunctionModule pointer.
        ServiceObjectTyped<KinematicType, ResultType>::m_pModuleObjectFactory->updateModulePointerReference(
                pObservable, 0);
        pObservable = 0;

        return result;
    }

    /**
     * Method used in the automated interface to compute an Observable for a list of kinematics.
     * @param task Automated XML task.
     * @return List of ObservableResult.
     */
    List<ResultType> computeManyKinematicOneModelTask(Task &task) {

        //get kinematics
        List<KinematicType> listOfKinematic = newListOfKinematicFromTask(task);

        //get GPD types
        //TODO DOES IT WORK?
        List<GPDType> listOfGPDType;
        try {
            listOfGPDType =
                    ServiceObjectTyped<KinematicType, ResultType>::getGPDTypeListFromTask(
                            task);
        } catch (const ElemUtils::CustomException &e) {
            //Nothing to do
        }

        //get observable module
        Observable<KinematicType, ResultType>* pObservable =
                newObservableModuleFromTask(task);

        //make computation
        List<ResultType> results = computeManyKinematicOneModel(listOfKinematic,
                pObservable, listOfGPDType, task.isStoreInDB());

        //remove reference to pConvolCoeffFunctionModule pointer
        ServiceObjectTyped<KinematicType, ResultType>::m_pModuleObjectFactory->updateModulePointerReference(
                pObservable, 0);
        pObservable = 0;

        //return
        return results;
    }

    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task) {
//        generatePlotFile(getOutputFilePathForPlotFileTask(task),
//                generateSQLQueryForPlotFileTask(task, "observable_plot_2d_view"),
//                ' ');
    }

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pConvolCoeffFunctionModule ConvolCoeffFunctionModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType> &gpdTypeList) const {
        //TODO
        return List<GPDType>();
    }
};

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::FUNCTION_NAME_COMPUTE_OBSERVABLE =
        "computeObservable";

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::FUNCTION_NAME_GENERATE_PLOT_FILE =
        "generatePlotFile";

} /* namespace PARTONS */

#endif /* OBSERVABLE_SERVICE_H */
