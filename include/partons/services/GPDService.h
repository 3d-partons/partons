#ifndef GPD_SERVICE_H
#define GPD_SERVICE_H

/**
 * @file GPDService.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date Aout 04, 2014
 * @version 1.0
 */

#include <string>

#include "../beans/gpd/GPDKinematic.h"
#include "../beans/gpd/GPDResult.h"
#include "../beans/gpd/GPDType.h"
#include "../beans/List.h"
#include "../ServiceObjectTyped.h"

namespace PARTONS {
class GPDModule;
} /* namespace PARTONS */

namespace PARTONS {

class GPDEvolutionModule;

/**
 * @class GPDService
 *
 * @brief Singleton to handle and compute some pre-configured GPD models.
 *
 * See the [general tutorial](@ref usage) and this [table](@ref usage_tasks) of examples.
 *
 * This service is used to call the different GPD models implemented and compute values at the given kinematics.
 * It also takes care of the evolution of GPDs. We give here examples of codes which can be done using the GPD service.
 *
 * 1. %Computation of a given GPD model called `MyFavoriteModelOfGPD` at a given kinematics (x, \f$\xi\f$, t, \f$\mu_{F}^{2}\f$, \f$\mu_{R}^{2}\f$), where \f$\mu_{F}^{2}\f$ and \f$\mu_{R}^{2}\f$ are
 * squares of factorization and renormalization scales, respectively:
 \code{.cpp}
 void computeSingleKinematicsForGPD() {

 // Retrieve GPD service
 PARTONS::GPDService* pGPDService =
 PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 // Create GPD module with the ModuleObjectFactory
 PARTONS::GPDModule* pGPDModel =
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteModelOfGPD::classId);

 // Create a GPDKinematic(x, xi, t, MuF2, MuR2) to compute
 PARTONS::GPDKinematic gpdKinematic(0.1, 0.2, -0.1, 2., 2.);

 // Run computation
 PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel);

 // Print results
 PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResult.toString());

 // Remove pointer reference ; Module pointers are managed by PARTONS.
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModel, 0);
 pGPDModel = 0;
 }
 \endcode
 *
 * 2. Computations of a list of kinematics using an external file:
 \code{.cpp}
 void computeManyKinematicsForGPD() {

 // Retrieve GPD service
 PARTONS::GPDService* pGPDService = PARTONS::Partons::getInstance()->getServiceObjectRegistry()->getGPDService();

 // Create GPD module with the ModuleObjectFactory
 PARTONS::GPDModule* pGPDModel = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteModelOfGPD::classId);

 // Load list of kinematics from file
 PARTONS::List<PARTONS::GPDKinematic> gpdKinematicList = PARTONS::KinematicUtils().getGPDKinematicFromFile("/path/to/kinematics_gpd.csv");

 // Run computation
 PARTONS::List<PARTONS::GPDResult> gpdResultList = pGPDService->computeManyKinematicOneModel(gpdKinematicList, pGPDModel);

 // Print results
 PARTONS::Partons::getInstance()->getLoggerManager()->info("main", __func__, gpdResultList.toString());

 // Remove pointer reference ; Module pointers are managed by PARTONS.
 PARTONS::Partons::getInstance()->getModuleObjectFactory()->updateModulePointerReference(pGPDModel, 0);
 pGPDModel = 0;
 }
 \endcode
 * In the file `kinematics_gpd.csv`, kinematic points are encoded in separate lines using the following format: "x|xi|t|MuF2|MuR2".
 *
 * 3. Compute a value of the GPD using Vinnikov evolution code (see Evolution Documentation) just change:
 * \code{.cpp}
 // Run computation
 PARTONS::GPDResult gpdResult = pGPDService->computeGPDModel(gpdKinematic, pGPDModel);
 \endcode
 * in case 1 by :
 *\code{.cpp}
 // Create an evolution Module
 PARTONS::GPDEvolutionModule* pEvolQCDModule = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDEvolutionModule() //TODO Complete the argument

 // Run computation
 PARTONS::GPDResult gpdResult = pGPDService->computeGPDModelWithEvolution(gpdKinematic, pGPDModel, pEvolQCDModule);
 \endcode
 *
 * 4. Use a list of GPD models instead of a single one. The code is the same than in case 1 but must be adapted to run several models, such as:
 \code{.cpp}
 // Create GPD module with the ModuleObjectFactory for Model 1
 PARTONS::GPDModule* pGPDModel1 = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteModelOfGPD1::classId);

 // Create GPD module with the ModuleObjectFactory for Model 2
 PARTONS::GPDModule* pGPDModel2 = PARTONS::Partons::getInstance()->getModuleObjectFactory()->newGPDModule(MyFavoriteModelOfGPD2::classId);

 //Create a list of model to compute
 std::vector<GPDModule*> listOfGPDToCompute(0);
 listOfGPDToCompute.push_back(pGPDModel1);
 listOfGPDToCompute.push_back(pGPDModel2);

 // Run computation
 PARTONS::List<PARTONS::GPDResult> gpdResult = pGPDService->computeListOfGPDModel(gpdKinematic, listOfGPDToCompute);
 \endcode
 *
 * 5. Restrictions to some specific GPDs can be provided. In this case use:
 *  \code{.cpp}
 computeListOfGPDModelRestrictedByGPDType(const GPDKinematic &gpdKinematic, std::vector<GPDModule*> &listOfGPDToCompute, GPDType gpdType);
 \endcode
 *instead of computeListOfGPDModel.
 */
class GPDService: public ServiceObjectTyped<GPDKinematic, GPDResult> {

public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    static const std::string GPD_SERVICE_COMPUTE_SINGLE_KINEMATIC; ///< Name of the XML task used to compute a GPD at given kinematics.
    static const std::string GPD_SERVICE_COMPUTE_MANY_KINEMATIC; ///< Name of the XML task used to compute GPDs for a list of kinematics.
    static const std::string GPD_SERVICE_GENERATE_PLOT_FILE; ///< Name of the XML task used for generating a data file ready for plotting.

    /**
     * Default destructor.
     */
    virtual ~GPDService();

    virtual void resolveObjectDependencies();
    virtual void computeTask(Task &task);

    /**
     * Computes GPD model at specific kinematics.
     *
     * @param gpdKinematic GPD kinematic.
     * @param pGPDModule GPDModule to use.
     * @param gpdType List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @return GPDResult
     */
    GPDResult computeSingleKinematic(const GPDKinematic &gpdKinematic,
            GPDModule* pGPDModule, const List<GPDType> & gpdType =
                    List<GPDType>()) const;

    /**
     * Computes a GPD model for a list of kinematics.
     *
     * @param gpdKinematicList List of GPDKinematics.
     * @param pGPDModule GPDModule to use for the computation.
     * @param gpdTypeList List of GPDType to compute. Default: all the GPDTypes available with the GPDModule.
     * @param storeInDB Boolean to store the results and kinematics on the database. Default: false.
     * @return List of GPDResults.
     */
    List<GPDResult> computeManyKinematic(
            const List<GPDKinematic> &gpdKinematicList, GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList = List<GPDType>(),
            const bool storeInDB = 0);

    /**
     * Devises the GPDModule from an automation task.
     *
     * @param task Automation task.
     * @return GPDModule pointer.
     */
    GPDModule* newGPDModuleFromTask(const Task &task) const;

    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return GPD kinematics.
     */
    GPDKinematic newKinematicFromTask(const Task &task) const;

    /**
     * Devises the GPD kinematics from an automation task.
     *
     * @param task Automation task.
     * @return List of GPD kinematics.
     */
    List<GPDKinematic> newListOfKinematicFromTask(const Task &task) const;

private:

    /**
     * Default constructor used by the registry.
     */
    GPDService(const std::string &className);

    /**
     * Method used in the automated interface to compute GPD.
     * @param task Automated XML task.
     * @return GPDResult object.
     */
    GPDResult computeSingleKinematicTask(Task &task);

    /**
     * Method used in the automated interface to compute GPDs for a list of kinematics.
     * @param task Automated XML task.
     * @return List of GPD results.
     */
    List<GPDResult> computeManyKinematicTask(Task &task);

    /**
     * Method used in the automated interface to generate a data file ready for plotting.
     * @param task Automated XML task.
     */
    void generatePlotFileTask(Task &task);

    /**
     * Method used to derive an intersection of available GPD types from the various underlying modules.
     * @param pGPDModule GPDModule used for the computation.
     * @param gpdTypeList List of desired GPD types to compute.
     * @return List of GPD types.
     */
    List<GPDType> getFinalGPDTypeList(GPDModule* pGPDModule,
            const List<GPDType> &gpdTypeList) const;
};

} /* namespace PARTONS */

#endif /* GPD_SERVICE_H */
