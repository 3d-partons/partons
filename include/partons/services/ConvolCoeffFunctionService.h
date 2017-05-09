#ifndef DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H
#define DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H

/**
 * @file ConvolCoeffFunctionService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 07, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionKinematic.h"
#include "../beans/convol_coeff_function/DVCS/DVCSConvolCoeffFunctionResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../ServiceObjectTyped.h"

class ConvolCoeffFunctionModule;
class GPDModule;
class GPDService;

/**
 * @class ConvolCoeffFunctionService
 *
 * @brief \<singleton\> Used to handle and compute some pre-configured CCF modules. See also the general tutorial on [Computation Services](@ref services_computation).
 */
class ConvolCoeffFunctionService: public ServiceObjectTyped<
        DVCSConvolCoeffFunctionKinematic, DVCSConvolCoeffFunctionResult> {
public:
    static const std::string FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL;
    static const std::string FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL;
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL;
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE;

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor.
     */
    ConvolCoeffFunctionService(const std::string &className);
    /**
     * Default destructor.
     */
    virtual ~ConvolCoeffFunctionService();

    /**
     * See parent class for details.
     */
    void resolveObjectDependencies();

    /**
     * Method used in automation to compute given tasks.
     * @param task Automation task to compute.
     */
    virtual void computeTask(Task &task);

    /**
     * Computes a CCF Model for a list of kinematics.
     * @param kinematics List of kinematics.
     * @param pConvolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of DVCSConvolCoeffFunctionResult.
     */
    List<DVCSConvolCoeffFunctionResult> computeForOneCCFModelAndManyKinematics(
            List<DVCSConvolCoeffFunctionKinematic> &kinematics,
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList = List<GPDType>(),
            const bool storeInDB = 0);

    /**
     * Computes a ConvolCoeffFunctionModule at specific kinematics.
     * @param kinematic CCF Kinematics.
     * @param convolCoeffFunctionModule CCF model to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with (both) the ConvolCoeffFunctionModule (AND the underlying GPDModule, if any).
     * @return DVCSConvolCoeffFunctionResult.
     */
    virtual DVCSConvolCoeffFunctionResult computeForOneCCFModel(
            const DVCSConvolCoeffFunctionKinematic &kinematic,
            ConvolCoeffFunctionModule* convolCoeffFunctionModule,
            const List<GPDType> & gpdTypeList = List<GPDType>()) const;

//    virtual ResultList<DVCSConvolCoeffFunctionResult> computeListWithGPDModel(
//            const DVCSConvolCoeffFunctionKinematic &kinematic,
//            std::vector<ConvolCoeffFunctionModule*> listOfDVCSConvolCoeffFunctionModule,
//            GPDType::Type gpdType = GPDType::ALL) const;

//    ConvolCoeffFunctionModule* newConvolCoeffFunctionModuleFromTask(
//            const Task &task) const;
//
//    ConvolCoeffFunctionModule* configureConvolCoeffFunctionModule(
//            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
//            GPDModule* pGPDModule) const;

//    std::vector<DVCSConvolCoeffFunctionResult> compute(
//            std::vector<CFFInputData> ListOfCFFInputData,
//            GPDModule* _pGPDModule, double MuF, double MuR,
//            QCDOrderType qcdOrderType);
//
//    std::map<unsigned int, CFFInputData> getMapOfCFFInputDataFromFile(
//            const std::string & filePath);
//
//    std::vector<CFFInputData> getListOfCFFInputDataFromFile(
//            const std::string & filePath);

    /**
     * Uses an automation task (XML file) to configure a ConvolCoeffFunctionModule.
     * @param task
     * @return Pre-configured ConvolCoeffFunctionModule.
     */
    ConvolCoeffFunctionModule* newConvolCoeffFunctionModuleFromTask(
            const Task &task) const;

    /**
     * Uses an automation task (XML file) to set specific kinematics.
     * @param task
     * @return CCF kinematics.
     */
    DVCSConvolCoeffFunctionKinematic newKinematicFromTask(
            const Task &task) const;
    /**
     * Uses an automation task (XML file) to set a list of kinematics.
     * @param task
     * @return List of CCF kinematics.
     */
    List<DVCSConvolCoeffFunctionKinematic> newListOfKinematicFromTask(
            const Task &task) const;

private:
    GPDService* m_pGPDService;

    //TODO improve object copy
    DVCSConvolCoeffFunctionResult computeWithGPDModelTask(Task &task) const;
    List<DVCSConvolCoeffFunctionResult> computeManyKinematicOneModelTask(
            Task& task);
    void generatePlotFileTask(Task &task);

    List<GPDType> getFinalGPDTypeList(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList) const;

//    ConvolCoeffFunctionModule* prepareComputationConfiguration(
//            const List<List<ElemUtils::Parameter> >& moduleNameList,
//            unsigned int level = 0) const;

};

#endif /* DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
