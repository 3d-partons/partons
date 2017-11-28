#ifndef GPD_EVOLUTION_MODULE_H
#define GPD_EVOLUTION_MODULE_H

/**
 * @file GPDEvolutionModule.h
 * @author Pawel Sznajder (NCBJ)
 * @date June 01, 2017
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../../../beans/automation/BaseObjectData.h"
#include "../../../beans/gpd/GPDType.h"
#include "../../../beans/parton_distribution/PartonDistribution.h"
#include "../../../beans/parton_distribution/QuarkDistribution.h"
#include "../../../beans/PerturbativeQCDOrderType.h"
#include "../../../beans/QuarkFlavor.h"
#include "../../../beans/QuarkNonSingletCombination.h"
#include "../../../ModuleObject.h"
#include "../../MathIntegratorModule.h"

namespace PARTONS {

class ActiveFlavorsThresholdsModule;
class GPDModule;
class RunningAlphaStrongModule;

/**
 * @class GPDEvolutionModule
 *
 * @brief Abstract class for GPD evolution implementation.
 *
 * This class acts as an abstract (mother) class for modules implementing GPD evolution.
 */
class GPDEvolutionModule: public ModuleObject, public MathIntegratorModule {

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

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    /**
     * Evaluate PartonDistribution.
     * @param x Longitudinal momentum of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param pGPDModule Pointer to GPD module to be evaluated.
     * @param gpdType Type of GPD.
     * @return Evaluated result encapsulated in PartonDistribution object.
     */
    PartonDistribution compute(double x, double xi, double t, double MuF2,
            double MuR2, GPDModule* pGPDModule, GPDType::Type gpdType);

    // GETTERS and SETTERS

    /**
     * Get pQCD order assumed in the calculation.
     */
    PerturbativeQCDOrderType::Type getQcdOrderType() const;

    /**
     * Set pQCD order assumed in the calculation.
     */
    void setQcdOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

    /**
     * Set pointer to RunningAlphaStrongModule object.
     */
    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;

    /**
     * Set pointer to RunningAlphaStrongModule object.
     */
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* runningAlphaStrongModule);

    /**
     * Get pointer to ActiveFlavorsThresholdsModule object.
     */
    ActiveFlavorsThresholdsModule* getActiveFlavorsModule() const;

    /**
     * Set pointer to ActiveFlavorsThresholdsModule object.
     */
    void setActiveFlavorsModule(
            ActiveFlavorsThresholdsModule* activeFlavorsModule);

protected:

    /**
     * Copy constructor.
     * @param other Object to be copied.
     */
    GPDEvolutionModule(const GPDEvolutionModule &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    /**
     * Precompute (in particular set initial variables).
     * @param x Longitudinal momentum of active parton.
     * @param xi Skewness variable.
     * @param t Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
     * @param MuF2 Factorization scale squared (in \f$GeV^{2}\f$).
     * @param MuR2 Renormalization scale squared (in \f$GeV^{2}\f$).
     * @param pGPDModule Pointer to GPD module to be evaluated.
     * @param gpdType Type of GPD.
     */
    void preCompute(double x, double xi, double t, double MuF2, double MuR2,
            GPDModule* pGPDModule, GPDType::Type gpdType);

    /**
     * Evaluate change of non-singlet for given factorization scale squared.
     * @param MuF2 Factorization scale squared.
     * @param quarkNonSingletCombination Type of non-singlet combinations.
     * @return Evaluated change of non-singlet distibution.
     */
    virtual double nonSingletMu2FDerivative(double MuF2,
            QuarkNonSingletCombination::Type quarkNonSingletCombination) = 0;

    /**
     * Evaluate change of singlet for given factorization scale squared.
     * @param MuF2 Factorization scale squared.
     * @return Evaluated change of singlet distibution.
     */
    virtual double singletMuF2Derivative(double MuF2) = 0;

    /**
     * Evaluate change of gluon distribution for given factorization scale squared.
     * @param MuF2 Factorization scale squared.
     * @return Evaluated change of gluon distibution.
     */
    virtual double gluonMuF2Derivative(double MuF2) = 0;

    GPDModule* m_pGPDModule;    ///< Pointer to GPD module.
    double m_x;     ///< Longitudinal momentum of active parton.
    double m_xi;    ///< Skewness variable.
    double m_t; ///< Four-momentum transfer squared of hadron target (in \f$GeV^{2}\f$).
    double m_MuF2;  ///< Factorization scale squared (in \f$GeV^{2}\f$).
    double m_MuR2;  ///< Renormalization scale squared (in \f$GeV^{2}\f$).
    double m_MuF2_ref; ///< Reference factorization scale squared (in \f$GeV^{2}\f$).
    GPDType::Type m_currentGPDComputeType;  ///< GPD type.

    size_t m_nFlavors; ///< Number of active flavors at current scale.
    size_t m_nFlavors_ref; ///< Numer of active flavors at reference scale.

    PerturbativeQCDOrderType::Type m_qcdOrderType; ///< pQCD order assumed in the calculation.

    RunningAlphaStrongModule* m_pRunningAlphaStrong; ///< Pointer to RunningAlphaStrongModule object being used.
    ActiveFlavorsThresholdsModule* m_pActiveFlavorsModule; ///< Pointer to ActiveFlavorsThresholdsModule object being used.

    PartonDistribution m_PartonDistributionAtMuF2_ref; ///< PartonDistribution object evaluated at reference scale.

    /**
     * Get map containing non-singlet values for given number of active flavors from PartonDistribution object.
     * @param nFlavors Number of active flavors.
     * @param partonDistribution Input PartonDistribution object.
     * @return Map containing non-singlet values.
     */
    std::map<QuarkNonSingletCombination::Type, double> getNS(
            const size_t nFlavors,
            const PartonDistribution& partonDistribution) const;

    /**
     * Get singlet value for given number of active flavors from PartonDistribution object.
     * @param nFlavors Number of active flavors.
     * @param partonDistribution Input PartonDistribution object.
     * @return Singlet value.
     */
    double getS(const size_t nFlavors,
            const PartonDistribution& partonDistribution) const;

    /**
     * Get value for gluon contribution from PartonDistribution object.
     * @param partonDistribution Input PartonDistribution object.
     * @return Value for gluon contribution.
     */
    double getG(const PartonDistribution& partonDistribution) const;

    /**
     * Get PartonDistribution object for given number of active flavors from non-singlet, singlet and gluon values.
     * @param nFlavors Number of active flavors.
     * @param NS Map containing non-singlet values.
     * @param S Singlet value.
     * @param G Value for gluon contribution.
     * @return PartonDistribution object.
     */
    PartonDistribution getPartonDistribution(const size_t nFlavors,
            const std::map<QuarkNonSingletCombination::Type, double>& NS,
            double S, double G) const;

    /**
     * Get constant iterator for given std::map<QuarkFlavor::Type, QuarkDistribution> map and key.
     * @param map Map.
     * @param key Key.
     * @param mapName Map name.
     * @return Requested const iterator.
     */
    inline std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator findInQuarkFlavorMap(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& map,
            QuarkFlavor::Type key, const std::string& mapName) const;

    /**
     * Get constant iterator for given std::map<QuarkNonSingletCombination::Type, double> map and key.
     * @param map Map.
     * @param key Key.
     * @param mapName Map name.
     * @return Requested const iterator.
     */
    inline std::map<QuarkNonSingletCombination::Type, double>::const_iterator findInQuarkNonSingletCombinationMap(
            const std::map<QuarkNonSingletCombination::Type, double>& map,
            QuarkNonSingletCombination::Type key,
            const std::string& mapName) const;

private:

    std::map<size_t, NumA::MatrixD> conversionMatrices; ///< Map containing conversion matrices.

    /**
     * Wrapper to integrate GPDEvolutionModule::nonSingletMu2FDerivative.
     */
    double integrateComputeOutputNS(double MuF2, std::vector<double> par);

    /**
     * Wrapper to integrate GPDEvolutionModule::singletMuF2Derivative.
     */
    double integrateComputeOutputS(double MuF2, std::vector<double> par);

    /**
     * Wrapper to integrate GPDEvolutionModule::gluonMuF2Derivative.
     */
    double integrateComputeOutputG(double MuF2, std::vector<double> par);

    /**
     * Initialize functors for integration.
     */
    void initFunctorsForIntegrations();

    NumA::FunctionType1D* p_intIntegrateComputeOutputNS; ///< Functor to GPDEvolutionModule::integrateComputeOutputNS.
    NumA::FunctionType1D* p_intIntegrateComputeOutputS; ///< Functor to GPDEvolutionModule::integrateComputeOutputS.
    NumA::FunctionType1D* p_intIntegrateComputeOutputG; ///< Functor to GPDEvolutionModule::integrateComputeOutputG.
};

} /* namespace PARTONS */

#endif /* GPD_EVOLUTION_MODULE_H */
