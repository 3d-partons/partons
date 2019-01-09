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

    static const std::string CCF_SERVICE_COMPUTE_SINGLE_KINEMATIC; ///< Name of the XML task used for computing a CCF.
    static const std::string CCF_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used for computing CCFs with a list of kinematics.
    static const std::string CCF_SERVICE_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Destructor.
     */
    virtual ~ConvolCoeffFunctionService() {
    }

    virtual void resolveObjectDependencies() {

        ServiceObjectTyped<KinematicType, ResultType>::resolveObjectDependencies();

        try {
            this->m_batchSize = ElemUtils::GenericType(
                    ElemUtils::PropertiesManager::getInstance()->getString(
                            "ccf.service.batch.size")).toUInt();
        } catch (const std::exception &e) {
            throw ElemUtils::CustomException(this->getClassName(), __func__,
                    e.what());
        }
    }

    virtual void computeTask(Task &task) {

        ServiceObjectTyped<KinematicType, ResultType>::computeTask(task);

        List<ResultType> resultList;

        if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::CCF_SERVICE_COMPUTE_MANY_KINEMATIC)) {
            resultList = computeManyKinematicTask(task);
        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::CCF_SERVICE_COMPUTE_SINGLE_KINEMATIC)) {
            resultList.add(computeSingleKinematicTask(task));

        }

        else if (ElemUtils::StringUtils::equals(task.getFunctionName(),
                ConvolCoeffFunctionService::CCF_SERVICE_GENERATE_PLOT_FILE)) {
            generatePlotFileTask(task);
        }

        else if (!this->computeGeneralTask(task)) {
            this->errorUnknownMethod(task);
        }

        this->updateResultInfo(resultList, this->m_resultInfo);

//        if (task.isStoreInDB()) {
//
//            ConvolCoeffFunctionResultDaoService convolCoeffFunctionResultDaoService;
//
//            int computationId = convolCoeffFunctionResultDaoService.insert(
//                    resultList);
//
//            if (computationId != -1) {
//                this->info(__func__,
//                        ElemUtils::Formatter()
//                                << "DVCSConvolCoeffFunctionResultList object has been stored in database with computation_id = "
//                                << computationId);
//            } else {
//                throw ElemUtils::CustomException(getClassName(), __func__,
//                        ElemUtils::Formatter()
//                                << "DVCSConvolCoeffFunctionResultList object : insertion into database failed");
//            }
//        }

        this->m_resultListBuffer = resultList;
    }

    /**
     * Computes a ConvolCoeffFunctionModule at specific kinematics.
     * @param kinematic CCF Kinematics.
     * @param pConvolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return Result.
     */
    ResultType computeSingleKinematic(const KinematicType &kinematic,
            ConvolCoeffFunctionModule<KinematicType, ResultType>* pConvolCoeffFunctionModule,
            const List<GPDType>& gpdTypeList = List<GPDType>()) const {

        //get list of GPD types
        List<GPDType> restrictedByGPDTypeListFinal = getFinalGPDTypeList(
                pConvolCoeffFunctionModule, gpdTypeList);

        //return
        return pConvolCoeffFunctionModule->compute(kinematic,
                restrictedByGPDTypeListFinal);
    }

    /**
     * Computes a CCF Model for a list of kinematics.
     * @param kinematics List of kinematics.
     * @param pConvolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of results.
     */
    List<ResultType> computeManyKinematic(List<KinematicType> &kinematics,
            ConvolCoeffFunctionModule<KinematicType, ResultType>* pConvolCoeffFunctionModule,
            const List<GPDType>& gpdTypeList = List<GPDType>()) {

        //print information
        this->info(__func__,
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
            this->initComputationalThread(pConvolCoeffFunctionModule);

            //print info
            this->info(__func__, "Thread(s) running ...");

            //batch feature
            unsigned int i = 0;
            unsigned int j = 0;

            //divide to packets
            while (i != kinematics.size()) {

                listOfPacket.clear();
                j = 0;

                while ((j != this->m_batchSize) && (i != kinematics.size())) {

                    ElemUtils::Packet packet;
                    KinematicType kinematic;
                    kinematic = kinematics[i];
                    packet << kinematic << finalGPDTypeList;
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
     * Uses an automation task (XML file) to configure a ConvolCoeffFunctionModule.
     * @param task Automation task.
     * @return Pre-configured ConvolCoeffFunctionModule.
     */
    virtual ConvolCoeffFunctionModule<KinematicType, ResultType>* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const = 0;

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

protected:

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionService(const std::string &className) :
            ServiceObjectTyped<KinematicType, ResultType>(className) {
    }

private:

    /**
     * Method used in the automated interface to compute CCF.
     * @param task Automated XML task.
     * @return Result.
     */
    ResultType computeSingleKinematicTask(Task &task) const {

        //create a kinematic and init it with a list of parameters
        KinematicType kinematic = newKinematicFromTask(task);

        //get GPD types
        List<GPDType> gpdTypeList = this->getGPDTypeListFromTask(task);

        //get CCF module
        ConvolCoeffFunctionModule<KinematicType, ResultType>* pConvolCoeffFunctionModule =
                newConvolCoeffFunctionModuleFromTask(task);

        //make computation
        ResultType result = computeSingleKinematic(kinematic,
                pConvolCoeffFunctionModule, gpdTypeList);

        //remove reference to pConvolCoeffFunctionModule pointer.
        this->m_pModuleObjectFactory->updateModulePointerReference(
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
    List<ResultType> computeManyKinematicTask(Task& task) {

        //get kinematics
        List<KinematicType> listOfKinematic = newListOfKinematicFromTask(task);

        //get GPD types
        List<GPDType> gpdTypeList = this->getGPDTypeListFromTask(task);

        //get CCF module
        ConvolCoeffFunctionModule<KinematicType, ResultType>* pConvolCoeffFunctionModule =
                newConvolCoeffFunctionModuleFromTask(task);

        //make computation
        List<ResultType> results = computeManyKinematic(listOfKinematic,
                pConvolCoeffFunctionModule, gpdTypeList);

        //remove reference to pConvolCoeffFunctionModule pointer
        this->m_pModuleObjectFactory->updateModulePointerReference(
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
            ConvolCoeffFunctionModule<KinematicType, ResultType>* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList) const {

        //initialize
        List<GPDType> restrictedByGPDTypeListFinal = gpdTypeList;

        //get list of GPD types available
        restrictedByGPDTypeListFinal =
                pConvolCoeffFunctionModule->getListOfAvailableGPDTypeForComputation();

        //intersection between available GPDType and GPDType asked
        if (!gpdTypeList.isEmpty()) {
            restrictedByGPDTypeListFinal = VectorUtils::intersection(
                    restrictedByGPDTypeListFinal, gpdTypeList);
        }

        //print info
        this->info(__func__,
                ElemUtils::Formatter() << restrictedByGPDTypeListFinal.size()
                        << " GPDType will be computed");

        //return
        return restrictedByGPDTypeListFinal;
    }
};

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::CCF_SERVICE_COMPUTE_SINGLE_KINEMATIC =
        "computeSingleKinematic";

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::CCF_SERVICE_COMPUTE_MANY_KINEMATIC =
        "computeManyKinematic";

template<typename KinematicType, typename ResultType>
const std::string ConvolCoeffFunctionService<KinematicType, ResultType>::CCF_SERVICE_GENERATE_PLOT_FILE =
        "generatePlotFile";

} /* namespace PARTONS */

#endif /* CONVOL_COEFF_FUNCTION_SERVICE_H */
