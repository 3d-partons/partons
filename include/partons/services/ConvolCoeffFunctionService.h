#ifndef CONVOL_COEFF_FUNCTION_SERVICE_H
#define CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file ConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
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
#include "../modules/convol_coeff_function/ConvolCoeffFunctionModule.h"
#include "../modules/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionModule.h"
#include "../ModuleObjectFactory.h"
#include "../Partons.h"
#include "../ServiceObjectTyped.h"
#include "../utils/VectorUtils.h"

namespace PARTONS {
class GPDService;
} /* namespace PARTONS */

namespace PARTONS {

/**
 * @class ConvolCoeffFunctionService
 *
 * @brief Abstract class for a service to handle and compute pre-configured CCF modules.
 */
template<typename KinematicType, typename ResultType> class ConvolCoeffFunctionService: public ServiceObjectTyped<
        KinematicType, ResultType> {

public:

    static const std::string FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL; ///< Name of the XML task used for computing a CCF.
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL; ///< Name of the XML task used for computing CCFs with a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionService(const std::string &className) :
            ServiceObjectTyped<KinematicType, ResultType>(className) {
    }

    /**
     * Default destructor.
     */
    virtual ~ConvolCoeffFunctionService() {
    }

    /**
     * See parent class for details.
     */
    void resolveObjectDependencies() {

        ServiceObject::resolveObjectDependencies();

        try {
            ServiceObjectTyped<KinematicType, ResultType>::m_batchSize =
                    ElemUtils::GenericType(
                            ElemUtils::PropertiesManager::getInstance()->getString(
                                    "ccf.service.batch.size")).toUInt();
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(
                    ServiceObjectTyped<KinematicType, ResultType>::getClassName(),
                    __func__, e.what());
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
                ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL)) {
            resultList = computeManyKinematicOneModelTask(task);
        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL)) {
            resultList.add(computeWithGPDModelTask(task));

        } else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::FUNCTION_NAME_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        } else if (!ServiceObjectTyped<KinematicType, ResultType>::computeGeneralTask(
                task)) {
            ServiceObjectTyped<KinematicType, ResultType>::errorUnknownMethod(
                    task);
        }

        updateResultInfo(resultList,
                ServiceObjectTyped<KinematicType, ResultType>::m_resultInfo);

//        if (task.isStoreInDB()) {
//
//            ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
//
//            int computationId = convolCoeffFunctionResultDaoService.insert(
//                    resultList);
//
//            if (computationId != -1) {
//                info(__func__,
//                        ElemUtils::Formatter()
//                                << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
//                                << computationId);
//            } else {
//                throw ElemUtils::CustomException(getClassName(), __func__,
//                        ElemUtils::Formatter()
//                                << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
//            }
//        }

        ServiceObjectTyped<KinematicType, ResultType>::m_resultListBuffer =
                resultList;
    }

    /**
     * Computes a CCF Model for a list of kinematics.
     * @param kinematics List of kinematics.
     * @param pConvolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of results.
     */
    List<ResultType> computeForOneCCFModelAndManyKinematics(
            List<KinematicType> &kinematics,
            ConvolCoeffFunctionModule<KinematicType>* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList = List<GPDType>(),
            const bool storeInDB = 0) {

        //print information
        info(__func__,
                ElemUtils::Formatter() << kinematics.size()
                        << " CCF kinematic(s) will be computed with "
                        << pConvolCoeffFunctionModule->getClassName());

        //initialize
        List<ResultType> results;
        List<ElemUtils::Packet> listOfPacket;
        List<GPDType> finalGPDTypeList = getFinalGPDTypeList(
                pConvolCoeffFunctionModule, gpdTypeList);

        //if to be computed
        if (finalGPDTypeList.size() != 0) {

            //init thread
            ServiceObjectTyped<KinematicType, ResultType>::initComputationalThread(
                    pConvolCoeffFunctionModule);

            //print info
            info(__func__, "Thread(s) running ...");

            //batch feature
            unsigned int i = 0;
            unsigned int j = 0;

            //devide to packets
            while (i != kinematics.size()) {

                listOfPacket.clear();
                j = 0;

                while ((j
                        != ServiceObjectTyped<KinematicType, ResultType>::m_batchSize)
                        && (i != kinematics.size())) {

                    ElemUtils::Packet packet;
                    KinematicType kinematic;
                    kinematic = kinematics[i];
                    packet << kinematic << finalGPDTypeList;
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
                info(__func__,
                        ElemUtils::Formatter()
                                << "Kinematic(s) already computed : " << i);

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

        } else {
            info(__func__,
                    "Nothing to compute with your computation configuration ; there is no GPDType available");
        }

        return results;
    }

    /**
     * Computes a ConvolCoeffFunctionModule at specific kinematics.
     * @param kinematic CCF Kinematics.
     * @param pConvolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return Result.
     */
    ResultType computeForOneCCFModel(const KinematicType &kinematic,
            ConvolCoeffFunctionModule<KinematicType>* pConvolCoeffFunctionModule,
            const List<GPDType> & gpdTypeList = List<GPDType>()) const {

        //get GPD types to be computed
        List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(
                pConvolCoeffFunctionModule, gpdTypeList);

        //initialize
        ResultType result;

        //loop over GPD types
        for (unsigned int i = 0; i != restrictedByGPDTypeListFinal.size();
                i++) {
            result.add(restrictedByGPDTypeListFinal[i],
                    pConvolCoeffFunctionModule->compute(kinematic,
                            restrictedByGPDTypeListFinal[i]));
        }

        //set kinematics
        result.setKinematic(kinematic);

        //set computation module name
        result.setComputationModuleName(
                pConvolCoeffFunctionModule->getClassName());

        //return
        return result;
    }

    /**
     * Uses an automation task (XML file) to configure a ConvolCoeffFunctionModule.
     * @param task
     * @return Pre-configured ConvolCoeffFunctionModule.
     */
    ConvolCoeffFunctionModule<KinematicType>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const {

        //initialize
        ConvolCoeffFunctionModule<KinematicType>* pConvolCoeffFunctionModule = 0;

        //check if availible
        //TODO remove hardcoded string
        if (ElemUtils::StringUtils::equals(
                task.getModuleComputationConfiguration().getModuleType(),
                "ConvolCoeffFunctionModule")) {
//TODO DVCS HERE!!!!!!!!!!!!!!!!!!!!!!
            //configure
            pConvolCoeffFunctionModule =
                    Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(
                            task.getModuleComputationConfiguration().getModuleClassName());

            pConvolCoeffFunctionModule->configure(
                    task.getModuleComputationConfiguration().getParameters());

            pConvolCoeffFunctionModule->prepareSubModules(
                    task.getModuleComputationConfiguration().getSubModules());
        } else {
            throw ElemUtils::CustomException(
                    ServiceObjectTyped<KinematicType, ResultType>::getClassName(),
                    __func__,
                    ElemUtils::Formatter()
                            << "You have not provided any ConvolCoeffFunctionModule");
        }

        //return
        return pConvolCoeffFunctionModule;
    }

    /**
     * Uses an automation task (XML file) to set specific kinematics.
     * @param task
     * @return CCF kinematics.
     */
    virtual KinematicType newKinematicFromTask(const Task &task) const = 0;
    /**
     * Uses an automation task (XML file) to set a list of kinematics.
     * @param task
     * @return List of CCF kinematics.
     */
    virtual List<KinematicType> newListOfKinematicFromTask(
            const Task &task) const = 0;

private:

    /**
     * Method used in the automated interface to compute CCF.
     * @param task Automated XML task.
     * @return Result.
     */
    ResultType computeWithGPDModelTask(Task &task) const {

        //create a kinematic and init it with a list of parameters
        KinematicType kinematic = newKinematicFromTask(task);

        //get configured ccf module
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
                newConvolCoeffFunctionModuleFromTask(task);

        //make computation
        ResultType result = computeForOneCCFModel(kinematic,
                pConvolCoeffFunctionModule);

        //remove reference to pConvolCoeffFunctionModule pointer.
        ServiceObjectTyped<KinematicType, ResultType>::m_pModuleObjectFactory->updateModulePointerReference(
                pConvolCoeffFunctionModule, 0);
        pConvolCoeffFunctionModule = 0;

        //return
        return result;
    }

    /**
     * Method used in the automated interface to compute CCFs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of results.
     */
    List<ResultType> computeManyKinematicOneModelTask(Task& task) {

        //get kinematics
        List<KinematicType> listOfKinematic = newListOfKinematicFromTask(task);

        //get GPD types
        List<GPDType> gpdTypeList =
                ServiceObjectTyped<KinematicType, ResultType>::getGPDTypeListFromTask(
                        task);

        //get CCF module
        ConvolCoeffFunctionModule* pConvolCoeffFunctionModule =
                newConvolCoeffFunctionModuleFromTask(task);

        //make computation
        List<ResultType> results = computeForOneCCFModelAndManyKinematics(
                listOfKinematic, pConvolCoeffFunctionModule, gpdTypeList,
                task.isStoreInDB());

        //remove reference to pConvolCoeffFunctionModule pointer.
        ServiceObjectTyped<KinematicType, ResultType>::m_pModuleObjectFactory->updateModulePointerReference(
                pConvolCoeffFunctionModule, 0);
        pConvolCoeffFunctionModule = 0;

        //return
        return results;
    }

    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task) {
//        generatePlotFile(getOutputFilePathForPlotFileTask(task),
//                    generateSQLQueryForPlotFileTask(task, "ccf_plot_2d_view"), ' ');
    }

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pConvolCoeffFunctionModule ConvolCoeffFunctionModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(
            ConvolCoeffFunctionModule<KinematicType>* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList) const {

        //initialize
        List<GPDType> availableGPDTypeForCCFModel = gpdTypeList;

        //get list available in ccf module
        availableGPDTypeForCCFModel =
                pConvolCoeffFunctionModule->getListOfAvailableGPDTypeForComputation();

        // intersection between available GPDType for this CCF model and GPDType asked
        if (!gpdTypeList.isEmpty()) {
            availableGPDTypeForCCFModel = VectorUtils::intersection(
                    availableGPDTypeForCCFModel, gpdTypeList);
        }

        // if this CCF model is GPD model dependent we need to perform another intersection with GPDType available for this GPD model
        if (pConvolCoeffFunctionModule->isGPDModuleDependent()) {
            availableGPDTypeForCCFModel =
                    VectorUtils::intersection(availableGPDTypeForCCFModel,
                            pConvolCoeffFunctionModule->getGPDModule()->getListOfAvailableGPDTypeForComputation());
        }

        //print info
        info(__func__,
                ElemUtils::Formatter() << availableGPDTypeForCCFModel.size()
                        << " GPDType will be computed");

        //return
        return availableGPDTypeForCCFModel;
    }
};

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL =
        "computeWithGPDModel";

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL =
        "computeManyKinematicOneModel";

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::FUNCTION_NAME_GENERATE_PLOT_FILE =
        "generatePlotFile";

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_SERVICE_H */
