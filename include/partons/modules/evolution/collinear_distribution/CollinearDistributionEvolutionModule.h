#ifndef COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_H
#define COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_H

/**
 * @file CollinearDistributionEvolutionModule.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date July 18, 2020
 * @version 1.0
 */

#include "../../../ModuleObject.h"
#include "../../../beans/collinear_distribution/CollinearDistributionType.h"
#include "../../../beans/collinear_distribution/CollinearDistributionKinematic.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/PerturbativeQCDOrderType.h"

namespace PARTONS {

class CollinearDistributionModule;
class RunningAlphaStrongModule;

/**
 * @class CollinearDistributionEvolutionModule
 *
 * @brief Abstract class for collinear distribution evolution implementation.
 *
 * This class acts as an abstract (mother) class for modules implementing collinear distribution evolution.
 */
class CollinearDistributionEvolutionModule: public ModuleObject {

public:

    /**
     * Settable parameters
     */
    static const std::string PARAM_NAME_MUF2_REF;

    /**
     * Type of module name used by the automatization.
     */
    static const std::string COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    CollinearDistributionEvolutionModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~CollinearDistributionEvolutionModule();

    virtual CollinearDistributionEvolutionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate PartonDistribution.
     */
    PartonDistribution compute(const CollinearDistributionKinematic &kinematic, CollinearDistributionModule* pCollinearDistributionModule);

    // ##### GETTERS & SETTERS #####

    void setMuF2_ref(const double& MuF2_ref);
    void setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder);
    void setCollinearDistributionType(CollinearDistributionType::Type type);
    void setRunningAlphaStrongModule(RunningAlphaStrongModule* runningAlphaStrongModule);

    double getMuF2_ref() const;
    double getMuF_ref() const;
    PerturbativeQCDOrderType::Type getPertOrder() const;
    CollinearDistributionType::Type getCollinearDistributionType() const;
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    CollinearDistributionEvolutionModule(const CollinearDistributionEvolutionModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual void setKinematics(const CollinearDistributionKinematic& kinematic);
    virtual CollinearDistributionKinematic getKinematics() const;

    virtual PartonDistribution compute(CollinearDistributionModule* pCollinearDistributionModule) = 0;

    /**
     * Attributes
     */
    double m_MuF2_ref; ///< Reference factorization scale squared (in \f$GeV^{2}\f$).
    PerturbativeQCDOrderType::Type m_pertOrder; ///< pQCD order assumed in the calculation.
    CollinearDistributionType::Type m_type;  ///< collinear distribution type.

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule; ///< Pointer to RunningAlphaStrongModule object being used.

    /**
     * Kinematics
     */
    double m_x;     ///< Longitudinal momentum
    double m_MuF2;  ///< Factorization scale squared (in \f$GeV^{2}\f$).
    double m_MuR2;  ///< Renormalization scale squared (in \f$GeV^{2}\f$).
};

} /* namespace PARTONS */

#endif /* COLLINEAR_DISTRIBUTION_EVOLUTION_MODULE_H */
