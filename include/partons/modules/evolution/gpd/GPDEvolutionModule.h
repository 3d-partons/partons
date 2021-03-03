#ifndef GPD_EVOLUTION_MODULE_H
#define GPD_EVOLUTION_MODULE_H

/**
 * @file GPDEvolutionModule.h
 * @author Valerio BERTONE (CEA Saclay)
 * @date Sptember 17, 2020
 * @version 1.0
 */

#include "../../../ModuleObject.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/gpd/GPDKinematic.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/PerturbativeQCDOrderType.h"

namespace PARTONS {

class GPDModule;
class RunningAlphaStrongModule;
class ActiveFlavorsThresholdsModule;

/**
 * @class GPDEvolutionModule
 *
 * @brief Abstract class for GPD evolution implementation.
 *
 * This class acts as an abstract (mother) class for modules implementing GPD evolution.
 */
class GPDEvolutionModule: public ModuleObject {

public:

    /**
     * Type of module name used by the automatization.
     */
    static const std::string GPD_EVOLUTION_MODULE_CLASS_NAME;

    /**
     * Constructor.
     * @param className Name of class.
     */
    GPDEvolutionModule(const std::string &className);

    /**
     * Destructor.
     */
    virtual ~GPDEvolutionModule();

    virtual GPDEvolutionModule* clone() const = 0;
    virtual std::string toString() const;
    virtual void resolveObjectDependencies();
    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void prepareSubModules(const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate PartonDistribution.
     */
    PartonDistribution compute(const GPDKinematic &kinematic, GPDModule* pGPDModule, const GPDType::Type &type);

    // ##### GETTERS & SETTERS #####

    void setMuF2_ref(const double& MuF2_ref);
    void setPertOrder(const PerturbativeQCDOrderType::Type& pertOrder);
    void setGPDType(GPDType::Type type);
    void setRunningAlphaStrongModule(RunningAlphaStrongModule* runningAlphaStrongModule);
    void setActiveFlavorsModule(ActiveFlavorsThresholdsModule* activeFlavorsModule);

    double getMuF2_ref() const;
    double getMuF_ref() const;
    PerturbativeQCDOrderType::Type getPertOrder() const;
    GPDType::Type getGPDType() const;
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;
    ActiveFlavorsThresholdsModule* getActiveFlavorsModule() const;

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDEvolutionModule(const GPDEvolutionModule &other);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    virtual void setKinematics(const GPDKinematic& kinematic);
    virtual GPDKinematic getKinematics() const;

    virtual PartonDistribution compute(GPDModule* pGPDModule, const GPDType::Type &type) = 0;

    /**
     * Attributes
     */
    double m_MuF2_ref;                          ///< Reference factorization scale squared (in \f$GeV^{2}\f$).
    PerturbativeQCDOrderType::Type m_pertOrder; ///< pQCD order assumed in the calculation.
    GPDType::Type m_type;                       ///< GPD distribution type.

    RunningAlphaStrongModule* m_pRunningAlphaStrongModule; ///< Pointer to RunningAlphaStrongModule object being used.
    ActiveFlavorsThresholdsModule* m_pActiveFlavorsModule; ///< Pointer to ActiveFlavorsThresholdsModule object being used.

    /**
     * Kinematics
     */
    double m_x;     ///< Longitudinal momentum
    double m_xi;    ///< Skewness variable.
    double m_t;     ///< Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
    double m_MuF2;  ///< Factorization scale squared (in \f$GeV^{2}\f$).
    double m_MuR2;  ///< Renormalization scale squared (in \f$GeV^{2}\f$).
};

} /* namespace PARTONS */

#endif /* GPD_EVOLUTION_MODULE_H */
