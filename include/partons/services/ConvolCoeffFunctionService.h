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

namespace PARTONS {

class ConvolCoeffFunctionModule;
class GPDModule;
class GPDService;

/**
 * @class ConvolCoeffFunctionService
 *
 * @brief Singleton to handle and compute some pre-configured CCF modules.
 *
 * See [tutorial](@ref usage).
 *
 * Please find below some examples how to use the different functions provided by this service.
 * For now, only DVCS CFF are available, at Leading Order, Next-to-Leading Order, and including heavy quark masses in the NLO loop.
 *
 * 1. Compute the coefficient functions at specific kinematics (\f$\xi\f$, t, \f$Q^{2}\f$, \f$\mu_{F}^{2}\f$, \f$\mu_{R}^{2}\f$) using the GPD model `MyFavoriteGPDModel`:
 * \code{.cpp}
 void computeSingleKinematicsForDVCSComptonFormFactor() {

 // Retrieve service
 PARTONS::ConvolCoeffFunctionService* pDVCSConvolCoeffFunctionService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getConvolCoeffFunctionService();

 // Create GPD module with the ModuleObjectFactory
 PARTONS::GPDModule* pGPDModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteGPDModel::classId);

 // Create CFF module with the ModuleObjectFactory
 PARTONS::DVCSConvolCoeffFunctionModule* pDVCSCFFModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newDVCSConvolCoeffFunctionModule(PARTONS::DVCSCFFStandard::classId);

 // Create parameters to configure later DVCSCFFModel with PerturbativeQCD = LO
 // This can be switched to NLO //TODO What about Heavy Quark Masses?
 ElemUtils::Parameters parameters(PARTONS::PerturbativeQCDOrderType::PARAMETER_NAME_PERTURBATIVE_QCD_ORDER_TYPE, PARTONS::PerturbativeQCDOrderType::LO);

 // Configure DVCSCFFModule with previous parameters.
 pDVCSCFFModule->configure(parameters);

 // Link GPDModule to DVCSCFFModule
 pDVCSCFFModule->setGPDModule(pGPDModule);

 // Create kinematic (xi,t,Q2,MuF2,MuR2)
 PARTONS::DVCSConvolCoeffFunctionKinematic cffKinematic = PARTONS::DVCSConvolCoeffFunctionKinematic(0.01, -0.1, 4., 4., 4.);

 // Run computation
 PARTONS::DVCSConvolCoeffFunctionResult cffResult = pDVCSConvolCoeffFunctionService->computeForOneCCFModel(cffKinematic, pDVCSCFFModule);

 // Print results for DVCSCFFModule
 PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, cffResult.toString());

 // Remove DVCSCFFModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pDVCSCFFModule, 0);
 pDVCSCFFModule = 0;

 // Remove GPDModule pointer reference
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModule, 0);
 pGPDModule = 0;
 }

 \endcode
 *
 * 2. The same thing can be done when dealing with many kinematics, by adapting the code with the following lines:
 \code{.cpp}
 // Load list of kinematics from file
 PARTONS::List<PARTONS::DVCSConvolCoeffFunctionKinematic> cffKinematicList = PARTONS::KinematicUtils().getCCFKinematicFromFile("/path/to/kinematics_dvcs_cff.csv");

 // Run computation
 PARTONS::List<PARTONS::DVCSConvolCoeffFunctionResult> cffResultList = pDVCSConvolCoeffFunctionService->computeForOneCCFModelAndManyKinematics(cffKinematicList, pDVCSCFFModule);
 \endcode
 * In the file `kinematics_dvcs_cff.csv`, kinematic points are encoded in separate lines using the following format: "xi|t|Q2|MuF2|MuR2".
 */
class ConvolCoeffFunctionService: public ServiceObjectTyped<
        DVCSConvolCoeffFunctionKinematic, DVCSConvolCoeffFunctionResult> {
public:
    static const std::string FUNCTION_NAME_COMPUTE_WITH_GPD_MODEL; ///< Name of the XML task used for computing a CCF.
    static const std::string FUNCTION_NAME_COMPUTE_LIST_WITH_GPD_MODEL; //TODO What's this?!
    static const std::string FUNCTION_NAME_COMPUTE_MANY_KINEMATIC_ONE_MODEL; ///< Name of the XML task used for computing CCFs with a list of kinematics.
    static const std::string FUNCTION_NAME_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

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
    GPDService* m_pGPDService; ///< Pointer to the singleton GPDService, used in case the CCF modules have an underlying GPD module.

    //TODO improve object copy
    /**
     * Method used in the automated interface to compute CCF.
     * @param task Automated XML task.
     * @return DVCSConvolCoeffFunctionResult object.
     */
    DVCSConvolCoeffFunctionResult computeWithGPDModelTask(Task &task) const;
    /**
     * Method used in the automated interface to compute CCFs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of DVCSConvolCoeffFunctionResult objects.
     */
    List<DVCSConvolCoeffFunctionResult> computeManyKinematicOneModelTask(
            Task& task);
    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pConvolCoeffFunctionModule ConvolCoeffFunctionModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(
            ConvolCoeffFunctionModule* pConvolCoeffFunctionModule,
            const List<GPDType> &gpdTypeList) const;

//    ConvolCoeffFunctionModule* prepareComputationConfiguration(
//            const List<List<ElemUtils::Parameter> >& moduleNameList,
//            unsigned int level = 0) const;

};

} /* namespace PARTONS */

#endif /* DVCS_CONVOL_COEFF_FUNCTION_SERVICE_H */
