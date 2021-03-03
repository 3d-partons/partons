#ifndef COLLINEAR_DISTRIBUTION_MODULE_H
#define COLLINEAR_DISTRIBUTION_MODULE_H

/**
 * @file CollinearDistributionModule.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <map>
#include <string>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/collinear_distribution/CollinearDistributionType.h"
#include "../../beans/List.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../ModuleObject.h"

namespace PARTONS {

class CollinearDistributionKinematic;
class CollinearDistributionResult;
class CollinearDistributionEvolutionModule;

/**
 * @class CollinearDistributionModule
 * @brief Abstract class that provides a skeleton to implement a Generalized Parton Distributions (CollinearDistribution) module.
 *
 * It is best to use this module with the corresponding service: CollinearDistributionService (see examples therein), as explained in the [general tutorial](@ref usage).
 */
class CollinearDistributionModule: public ModuleObject {

public:

    static const std::string COLLINEAR_DISTRIBUTION_MODULE_CLASS_NAME; ///< Type of the module in XML automation.

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionModule();

    virtual CollinearDistributionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void run();
    void virtual configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Virtual method, computes collinear distribution with some input parameters.
     *
     * @param kinematic collinear distribution kinematics object.
     * @param colldistType. See CollinearDistributionType for more details.
     * @param evolution Boolean to use evolution.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution compute(const CollinearDistributionKinematic &kinematic, CollinearDistributionType::Type colldistType);

    /**
     * Virtual method, computes collinear distribution with some input parameters.
     *
     * @param kinematic collinear distribution kinematics object.
     * @param colldistType. See CollinearDistributionType for more
     * details.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual CollinearDistributionResult compute(const CollinearDistributionKinematic &kinematic,
            const List<CollinearDistributionType>& colldistType = List<CollinearDistributionType>());

    /**
     * Get list of available CollinearDistributionTypes in the model considered. This list is set in the child class.
     */
    List<CollinearDistributionType> getListOfAvailableCollinearDistributionTypeForComputation() const;

    // ##### IMPLEMENTATION FUNCTIONS #####

    /**
     * This method can be implemented in the child class if the collinear distribution H is available to compute.
     *
     * @return PartonDistribution object.
     * Contains results for each flavor of partons.
     */
    virtual PartonDistribution computeUnpolPDF();
    virtual PartonDistribution computePolPDF();
    virtual PartonDistribution computeTransPDF();
    virtual PartonDistribution computeUnpolFF();
    virtual PartonDistribution computePolFF();
    virtual PartonDistribution computeTransFF();

    // ##### GETTERS & SETTERS #####

    /**
     * Get reference factorization scale used by the collinear distribution model before evolution.
     */
    double getMuF2Ref() const;

    /**
     * Set underlying collinear distribution Evolution module.
     */
    const CollinearDistributionEvolutionModule* getEvolQcdModule() const;

    /**
     * Get  underlying collinear distribution Evolution module.
     */
    void setEvolQcdModule(CollinearDistributionEvolutionModule* pEvolQcdModule);

protected:

    /**
     * Default constructor.
     */
    CollinearDistributionModule(const std::string &className);

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionModule(const CollinearDistributionModule &other);

    /**
     * Set internal kinematics.
     * @param kinematic Kinematics to be set.
     */
    virtual void setKinematics(const CollinearDistributionKinematic& kinematic);
    virtual void initModule();
    virtual void isModuleWellConfigured();

    /**
     * List of collinear distribution types that can be computed by the child class.
     * Needs to be set in the constructor of the child class, with the corresponding methods to be used.
     */
    std::map<CollinearDistributionType::Type, PartonDistribution (CollinearDistributionModule::*)()> m_listCollinearDistributionComputeTypeAvailable;

    double m_x; ///< Longitudinal momentum fraction of the active parton.
    double m_MuF2; ///< Factorization scale squared.
    double m_MuR2; ///< Renormalization scale squared.
    double m_MuF2_ref; ///< Reference factorization scale used by the collinear model before evolution.

    CollinearDistributionEvolutionModule* m_pCollinearDistributionEvolutionModule; ///< Pointer to the underlying collinear distribution Evolution module.
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_MODULE_H */
