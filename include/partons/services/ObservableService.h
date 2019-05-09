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

#include "../beans/automation/Task.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../modules/observable/Observable.h"
#include "../modules/process/ProcessModule.h"
#include "../ServiceObjectTyped.h"
#include "../utils/VectorUtils.h"

namespace PARTONS {

/**
 * @class ObservableService
 *
 * @brief Abstract class for a service to handle and compute observables.
 */
template<typename KinematicType, typename ResultType>
class ObservableService: public ServiceObjectTyped<KinematicType, ResultType> {

public:

    static const std::string OBSERVABLE_SERVICE_COMPUTE_SINGLE_KINEMATIC; ///< Name of the XML task used to compute an Observable at given kinematics.
    static const std::string OBSERVABLE_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used to compute an Observable for a list of kinematics.
    static const std::string OBSERVABLE_SERVICE_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Destructor.
     */
    virtual ~ObservableService() {
    }

    /**
     * See parent class for details.
     */
    virtual void resolveObjectDependencies() {

        ServiceObjectTyped<KinematicType, ResultType>::resolveObjectDependencies();

        try {
            this->m_batchSize = ElemUtils::GenericType(
                    ElemUtils::PropertiesManager::getInstance()->getString(
                            "observable.service.batch.size")).toUInt();
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    e.what());
        }
    }

    virtual void computeTask(Task &task) {

        ServiceObjectTyped<KinematicType, ResultType>::computeTask(task);

        List<ResultType> resultList;

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::OBSERVABLE_SERVICE_COMPUTE_MANY_KINEMATIC)) {
            resultList = computeManyKinematicTask(task);
        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::OBSERVABLE_SERVICE_COMPUTE_SINGLE_KINEMATIC)) {
            resultList.add(computeSingleKinematicTask(task));
        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ObservableService::OBSERVABLE_SERVICE_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        }

        else if (!this->computeGeneralTask(task)) {
            this->errorUnknownMethod(task);
        }

        this->updateResultInfo(resultList, this->m_resultInfo);

        if (task.isStoreInDB()) {

            if (resultList.size() == 0) {
                this->warn(__func__, "No results to be inserted into database");
            } else {
                storeResultListInDatabase(resultList);
            }
        }

        this->m_resultListBuffer = resultList;
    }

    /**
     * Computes an Observable at specific kinematics.
     * @param observableKinematic Kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return ObservableResult.
     */
    ResultType computeSingleKinematic(const KinematicType &observableKinematic,
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType> & gpdTypeList = List<GPDType>()) const {

        //get list of GPD types
        List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(
                pObservable, gpdTypeList);

        //return
        return pObservable->compute(observableKinematic, gpdTypeList);
    }

    /**
     * Computes an Observable for a list of kinematics.
     * @param listOfKinematic List of kinematics.
     * @param pObservable Observable to compute.
     * @param listOfGPDType List of GPDType to compute. Default: all the GPDTypes available with (both) the underlying ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of ObservableResult.
     */
    List<ResultType> computeManyKinematic(
            const List<KinematicType> & listOfKinematic,
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType>& gpdTypeList = List<GPDType>()) {

        //debug information
        this->debug(__func__,
                ElemUtils::Formatter() << listOfKinematic.size()
                        << " Observable kinematic(s) will be computed with "
                        << pObservable->getClassName());

        //initialize
        List<ResultType> results;
        List<ElemUtils::Packet> listOfPacket;
        List<GPDType> finalListOfGPDType = getFinalGPDTypeList(pObservable,
                gpdTypeList);

        //if to be computed
        if (finalListOfGPDType.size() != 0) {

            //init thread
            this->initComputationalThread(pObservable);

            //print info
            this->info(__func__, "Thread(s) running ...");

            //batch feature
            unsigned int i = 0;
            unsigned int j = 0;

            //divide to packets
            while (i != listOfKinematic.size()) {

                listOfPacket.clear();
                j = 0;

                while ((j != this->m_batchSize) && (i != listOfKinematic.size())) {

                    ElemUtils::Packet packet;
                    KinematicType kinematic;
                    kinematic = listOfKinematic[i];
                    packet << kinematic << finalListOfGPDType;
                    listOfPacket.add(packet);
                    i++;
                    j++;
                }

                //add, lunch and sort
                this->addTasks(listOfPacket);
                this->launchAllThreadAndWaitingFor();
                this->sortResultList();

                //print info
                this->info(__func__,
                        ElemUtils::Formatter()
                                << "Kinematic(s) already computed: " << i);

                //update result info
                this->updateResultInfo(this->getResultList(),
                        this->m_resultInfo);

                //add to output
                results.add(this->getResultList());

                //clear buffer
                this->clearResultListBuffer();
            }

            //clear threads
            this->clearAllThread();

        } else {
            this->info(__func__,
                    "Nothing to compute with your computation configuration ; there is no GPDType available");
        }

        return results;
    }

    /**
     * @brief Uses an automation task (XML file) to configure a ProcessModule, e.g.\ a DVCSModule.
     * @param task Automation task.
     * @return Pre-configured ProcessModule.
     */
    virtual ProcessModule<KinematicType, ResultType>* newProcessModuleFromTask(
            const Task &task) const = 0;

    /**
     * Uses an automation task (XML file) to configure an Observable.
     * @param task Automation task.
     * @return Pre-configured Observable.
     */
    virtual Observable<KinematicType, ResultType>* newObservableModuleFromTask(
            const Task &task) const = 0;

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

    /**
     * Store list of results in DB.
     * @param results List of results.
     * @return True is insertion successful.
     */
    virtual void storeResultListInDatabase(
            const List<ResultType>& results) const = 0;

protected:

    /**
     * Default constructor.
     */
    ObservableService(const std::string &className) :
            ServiceObjectTyped<KinematicType, ResultType>(className) {
    }

private:

    /**
     * Method used in the automated interface to compute an Observable.
     * @param task Automated XML task.
     * @return ObservableResult object.
     */
    ResultType computeSingleKinematicTask(Task &task) {

        //create a kinematic and init it with a list of parameters
        KinematicType kinematic = newKinematicFromTask(task);

        //get GPD types
        List<GPDType> gpdTypeList = this->getGPDTypeListFromTask(task);

        //get configured observable module
        Observable<KinematicType, ResultType>* pObservable =
                newObservableModuleFromTask(task);

        //make computation
        ResultType result = computeSingleKinematic(kinematic, pObservable,
                gpdTypeList);

        //remove reference to pConvolCoeffFunctionModule pointer.
        this->m_pModuleObjectFactory->updateModulePointerReference(pObservable,
                0);
        pObservable = 0;

        return result;
    }

    /**
     * Method used in the automated interface to compute an Observable for a list of kinematics.
     * @param task Automated XML task.
     * @return List of ObservableResult.
     */
    List<ResultType> computeManyKinematicTask(Task &task) {

        //get kinematics
        List<KinematicType> listOfKinematic = newListOfKinematicFromTask(task);

        //get GPD types
        List<GPDType> gpdTypeList = this->getGPDTypeListFromTask(task);

        //get observable module
        Observable<KinematicType, ResultType>* pObservable =
                newObservableModuleFromTask(task);

        //make computation
        List<ResultType> results = computeManyKinematic(listOfKinematic,
                pObservable, gpdTypeList);

        //remove reference to pConvolCoeffFunctionModule pointer
        this->m_pModuleObjectFactory->updateModulePointerReference(pObservable,
                0);
        pObservable = 0;

        //return
        return results;
    }

    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    virtual void generatePlotFileTask(Task &task) = 0;

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pConvolCoeffFunctionModule ConvolCoeffFunctionModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(
            Observable<KinematicType, ResultType>* pObservable,
            const List<GPDType> &gpdTypeList) const {

        //initialize
        List<GPDType> restrictedByGPDTypeListFinal = gpdTypeList;

        //get list of GPD types available
        restrictedByGPDTypeListFinal =
                pObservable->getListOfAvailableGPDTypeForComputation();

        //intersection between available GPDType and GPDType asked
        if (!gpdTypeList.isEmpty()) {
            restrictedByGPDTypeListFinal = VectorUtils::intersection(
                    restrictedByGPDTypeListFinal, gpdTypeList);
        }

        //debug info
        this->debug(__func__,
                ElemUtils::Formatter() << restrictedByGPDTypeListFinal.size()
                        << " GPDType will be computed");

        //return
        return restrictedByGPDTypeListFinal;
    }
};

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::OBSERVABLE_SERVICE_COMPUTE_SINGLE_KINEMATIC =
        "computeSingleKinematic";

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::OBSERVABLE_SERVICE_COMPUTE_MANY_KINEMATIC =
        "computeManyKinematic";

template<typename KinematicType, typename ResultType>
const std::string ObservableService<KinematicType, ResultType>::OBSERVABLE_SERVICE_GENERATE_PLOT_FILE =
        "generatePlotFile";

} /* namespace PARTONS */

#endif /* OBSERVABLE_SERVICE_H */
