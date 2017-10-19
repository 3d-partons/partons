#ifndef GPD_EVOLUTION_MODULE_H
#define GPD_EVOLUTION_MODULE_H

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/matrix/MatrixD.h>
#include <stddef.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/parton_distribution/PartonDistribution.h"
#include "../../beans/parton_distribution/QuarkDistribution.h"
#include "../../beans/PerturbativeQCDOrderType.h"
#include "../../beans/QuarkFlavor.h"
#include "../../ModuleObject.h"
#include "../MathIntegratorModule.h"
#include "QuarkNonSingletCombination.h"

namespace PARTONS {

class ActiveFlavorsThresholdsModule;
class GPDModule;
class PartonDistribution;
class RunningAlphaStrongModule;

class GPDEvolutionModule: public ModuleObject, public MathIntegratorModule {

public:

    static const std::string GPD_EVOLUTION_MODULE_CLASS_NAME;

    GPDEvolutionModule(const std::string &className);
    virtual ~GPDEvolutionModule();

    virtual GPDEvolutionModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);
    virtual void resolveObjectDependencies();

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    PartonDistribution compute(double x, double xi, double t, double MuF2,
            double MuR2, GPDModule* pGPDModule, GPDType::Type gpdType);

    // GETTERS and SETTERS

    PerturbativeQCDOrderType::Type getQcdOrderType() const;
    void setQcdOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

    RunningAlphaStrongModule* getRunningAlphaStrongModule() const;
    void setRunningAlphaStrongModule(
            RunningAlphaStrongModule* runningAlphaStrongModule);

    ActiveFlavorsThresholdsModule* getActiveFlavorsModule() const;
    void setActiveFlavorsModule(
            ActiveFlavorsThresholdsModule* activeFlavorsModule);

protected:

    GPDEvolutionModule(const GPDEvolutionModule &other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    void preCompute(double x, double xi, double t, double MuF2, double MuR2,
            GPDModule* pGPDModule, GPDType::Type gpdType);

    virtual double nonSingletMu2FDerivative(double MuF2,
            QuarkNonSingletCombination::Type quarkNonSingletCombination) = 0;
    virtual double singletMuF2Derivative(double MuF2) = 0;
    virtual double gluonMuF2Derivative(double MuF2) = 0;

    GPDModule* m_pGPDModule;
    double m_x;
    double m_xi;
    double m_t;
    double m_MuF2;
    double m_MuR2;
    double m_MuF2_ref;
    GPDType::Type m_currentGPDComputeType;

    size_t m_nFlavors;
    size_t m_nFlavors_ref;

    PerturbativeQCDOrderType::Type m_qcdOrderType;

    RunningAlphaStrongModule* m_pRunningAlphaStrong;
    ActiveFlavorsThresholdsModule* m_pActiveFlavorsModule;

    PartonDistribution m_PartonDistributionAtMuF2_ref;

    std::map<QuarkNonSingletCombination::Type, double> getNS(
            const size_t nFlavors,
            const PartonDistribution& partonDistribution) const;
    double getS(const size_t nFlavors,
            const PartonDistribution& partonDistribution) const;
    double getG(const PartonDistribution& partonDistribution) const;
    PartonDistribution getPartonDistribution(const size_t nFlavors,
            const std::map<QuarkNonSingletCombination::Type, double>& NS,
            double S, double G) const;

    inline std::map<QuarkFlavor::Type, QuarkDistribution>::const_iterator findInQuarkFlavorMap(
            const std::map<QuarkFlavor::Type, QuarkDistribution>& map,
            QuarkFlavor::Type key, const std::string& mapName) const;

    inline std::map<QuarkNonSingletCombination::Type, double>::const_iterator findInQuarkNonSingletCombinationMap(
            const std::map<QuarkNonSingletCombination::Type, double>& map,
            QuarkNonSingletCombination::Type key,
            const std::string& mapName) const;

private:

    std::map<size_t, NumA::MatrixD> conversionMatrices;

    double integrateComputeOutputNS(double MuF2, std::vector<double> par);
    double integrateComputeOutputS(double MuF2, std::vector<double> par);
    double integrateComputeOutputG(double MuF2, std::vector<double> par);

    void initFunctorsForIntegrations();

    NumA::FunctionType1D* p_intIntegrateComputeOutputNS;
    NumA::FunctionType1D* p_intIntegrateComputeOutputS;
    NumA::FunctionType1D* p_intIntegrateComputeOutputG;
};

} /* namespace PARTONS */

#endif /* GPD_EVOLUTION_MODULE_H */
