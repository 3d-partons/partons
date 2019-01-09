#ifndef GPD_MODULE_H
#define GPD_MODULE_H

/**
 * @file GPDModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date August 06, 2014
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDKinematic.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../ModuleObject.h"

namespace PARTONS {
class GPDResult;
} /* namespace PARTONS */

namespace PARTONS {

class GPDEvolutionModule;

/**
 * @class GPDModule
 * @brief Abstract class that provides a skeleton to implement a Generalized Parton Distributions (GPD) module.
 *
 * It is best to use this module with the corresponding service: GPDService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class GPDModule: public ModuleObject {

public:

    static const std::string GPD_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~GPDModule();

    virtual GPDModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    void virtual configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param kinematic GPD kinematics object.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(const GPDKinematic &kinematic,
            GPDType::Type gpdType, bool evolution = true);

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param kinematic GPD kinematics object.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual GPDResult compute(const GPDKinematic &kinematic,
            const List<GPDType>& gpdType = List<GPDType>());

    /**
     * Virtual method, computes GPD with some input parameters.
     *
     * @param kinematic GPD kinematics object.
     * @param gpdType H, Ht, E, Et, ... or ALL. See GPDType for more details.
     * @param evolution Boolean to use evolution.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual GPDResult compute1(const GPDKinematic &kinematic,
            const List<GPDType>& gpdType, bool evolution);

    /**
     * Get list of available GPDTypes in the model considered. This list is set in the child class.
     */
    List<GPDType> getListOfAvailableGPDTypeForComputation() const;

    // ##### GETTERS & SETTERS #####

    /**
     * Get reference factorization scale used by the GPD model before evolution.
     */
    double getMuF2Ref() const;

    /**
     * Set underlying GPD Evolution module.
     */
    const GPDEvolutionModule* getEvolQcdModule() const;

    /**
     * Get  underlying GPD Evolution module.
     */
    void setEvolQcdModule(GPDEvolutionModule* pEvolQcdModule);

    // #### IMPLEMENTATION MEMBERS ####

    /**
     * This method can be implemented in the child class if the GPD H is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeH();

    /**
     * This method can be implemented in the child class if the GPD E is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeE();

    /**
     * This method can be implemented in the child class if the GPD Ht is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeHt();

    /**
     * This method can be implemented in the child class if the GPD Et is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeEt();

protected:

    /**
     * Default constructor.
     */
    GPDModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDModule(const GPDModule &other);

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set.
     */
    virtual void setKinematics(const GPDKinematic& kinematic);

    /**
     * Set current GPD type to be computed.
     */
    void setCurrentGPDType(GPDType::Type gpdType);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of GPD types that can be computed by the child class.
     * Needs to be set in the constructor of the child class, with the corresponding methods to be used.
     */
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()> m_listGPDComputeTypeAvailable;

    /**
     * Iterator.
     */
    std::map<GPDType::Type, PartonDistribution (GPDModule::*)()>::iterator m_it;

    double m_x; ///< Longitudinal momentum fraction of the active parton.
    double m_xi; ///< Skewness.
    double m_t; ///< Mandelstam variable, momentum transfer on the hadron target.
    double m_MuF2; ///< Factorization scale squared.
    double m_MuR2; ///< Renormalization scale squared.
    GPDType::Type m_currentGPDComputeType; ///< Current GPD type.

    double m_MuF2_ref; ///< Reference factorization scale used by the GPD model before evolution.

    GPDEvolutionModule* m_pGPDEvolutionModule; ///< Pointer to the underlying GPD Evolution module.
};

} /* namespace PARTONS */

#endif /* GPD_MODULE_H */
