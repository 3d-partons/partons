#ifndef GPD_EVOLUTION_MODULE_H
#define GPD_EVOLUTION_MODULE_H

/**
 * @file GPDEvolutionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date September 01, 2015
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <NumA/linear_algebra/vector/VectorD.h>
#include <map>
#include <string>
#include <vector>

#include "../../beans/active_flavors_thresholds/ActiveFlavorsThresholds.h"
#include "../../beans/automation/BaseObjectData.h"
#include "../../beans/gpd/GPDType.h"
#include "../../beans/PerturbativeQCDOrderType.h"
#include "../../beans/QuarkFlavor.h"
#include "../../ModuleObject.h"

class ActiveFlavorsThresholdsModule;
class GPDModule;
class PartonDistribution;
class QuarkDistribution;
class RunningAlphaStrongModule;
namespace NumA {
class MatrixD;
} /* namespace NumA */

/**
 * @class GPDEvolutionModule
 *
 * @brief
 */

class GPDEvolutionModule: public ModuleObject {
public:

    static const std::string GPD_EVOLUTION_MODULE_CLASS_NAME;

    enum Type {
        RELATIVE, ABSOLUTE, BOTH
    };

    GPDEvolutionModule(const std::string &className);
    virtual ~GPDEvolutionModule();
    virtual GPDEvolutionModule* clone() const = 0;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual void resolveObjectDependencies();

    PartonDistribution compute(double x, double xi, double t, double MuF2,
            double MuR2, GPDModule* pGPDModule, GPDType::Type gpdType);

    /**
     * Checks if MuF (from compute parameters) is revelant
     */
    bool isRunnable(double MuF2, double MuF2_ref,
            GPDEvolutionModule::Type testType);

    virtual void prepareSubModules(
            const std::map<std::string, BaseObjectData>& subModulesData);

    // ##### GETTERS & SETTERS #####

    void setGpdModule(GPDModule* gpdModule);
    PerturbativeQCDOrderType::Type getQcdOrderType() const;
    void setQcdOrderType(PerturbativeQCDOrderType::Type qcdOrderType);

protected:
    /**
     * Copy constructor
     */
    GPDEvolutionModule(const GPDEvolutionModule &other);

    double m_x;
    double m_xi;
    double m_t;
    double m_MuF2;
    double m_MuR2;

    GPDModule* m_pGPDModule;

    PerturbativeQCDOrderType::Type m_qcdOrderType;
    GPDType::Type m_currentGPDComputeType;

    ActiveFlavorsThresholdsModule* m_pNfFunction;
    ActiveFlavorsThresholdsModule* m_pNfEvolFunction;
    RunningAlphaStrongModule *m_pRunningAlphaStrong;

    //TODO unused
    double m_scaleDistinction;

    unsigned short m_currentNf;

    /* m_nonSingletIndex refers to the current nonsinglet contribution.
     * There are 2*m_nf-1 nonsinglet contributions for a given nf value.
     * Therefore m_nonSingletIndex should (and will) range between 0 and 2*m_nf-2.
     * It is the easiest way for the parent class GPDEvolutionModule to send
     * the information on the current nonsinglet GPD combination to a daughter class
     * specifying the implementation of evolution equations.
     * It is not, strictly speaking, an information related to the MuF interval.
     */
    unsigned short m_currentNonSingletIndex;

    //TODO a quoi cela sert ?
    unsigned int m_nbXPoints;	///< Max of points if adaptative method,
    unsigned int m_nbMuFPoints;	///< fixed otherwise
    double m_epsilon;
    double m_alpha;
    //TODO a quoi cela sert ?

    NumA::VectorD m_partonDistributionFlavorBase;
    NumA::VectorD m_partonDistributionEvolutionBase;

    void preCompute(double x, double xi, double t, double MuF2, double MuR2,
            GPDModule* pGPDModule, GPDType::Type gpdType);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    PartonDistribution makeFinalPartonDistribution();

    double nonSingletGPD(unsigned short nonSingletIndex,
            unsigned short currentNf, double y, double MuF2);

    virtual double integratedNonSingletMuFDerivative(
            const ActiveFlavorsThresholds &nfInterval) = 0;
    virtual double integratedSingletMuFDerivative(
            const ActiveFlavorsThresholds &nfInterval) = 0;
    virtual double integratedGluonMuFDerivative(
            const ActiveFlavorsThresholds &nfInterval) = 0;

private:
    static NumA::MatrixD conversionMatrix1;
    static NumA::MatrixD conversionMatrix2;
    static NumA::MatrixD conversionMatrix3;
    static NumA::MatrixD conversionMatrix4;
    static NumA::MatrixD conversionMatrix5;
    static NumA::MatrixD conversionMatrix6;

    static NumA::MatrixD invertMatrix1;
    static NumA::MatrixD invertMatrix2;
    static NumA::MatrixD invertMatrix3;
    static NumA::MatrixD invertMatrix4;
    static NumA::MatrixD invertMatrix5;
    static NumA::MatrixD invertMatrix6;

    bool isRelativeTest(double MuF2, double MuF2_ref);
    bool isAbsoluteTest(double MuF2, double MuF2_ref);

    QuarkDistribution makeFinalQuarkDistribution(QuarkFlavor::Type quarkFlavor,
            double quarkDistributionPlus, double quarkDistributionMinus);
    double calculateFq(double FMinus, double FPlus);

    // refactored

    // PartonDistribution computeGPDModelAtMufRef();

    NumA::VectorD makeVectorOfGPDCombinations(
            const PartonDistribution &partonDistribution);
    void resizeVectorOfGPDCombination(NumA::VectorD &vectorOfQuarkDistribution,
            unsigned short nfInterval);

    // PartonDistribution evolution(const PartonDistribution &partonDistribution);
    void evolution(const ActiveFlavorsThresholds &nfInterval);

    NumA::VectorD convertBasis(const NumA::VectorD &vectorToConvert,
            unsigned short matrixNum);
    double convertBasis(const NumA::VectorD &vectorToConvert,
            unsigned short currentNf, unsigned short nonSingletIndex);
    NumA::VectorD invertBasis(const NumA::VectorD &vectorToInvert,
            unsigned short matrixNum);

    void computeNonSinglet(const ActiveFlavorsThresholds &nfInterval);
    void computeSingletGluon(const ActiveFlavorsThresholds &nfInterval);

    // ***************

    std::vector<ActiveFlavorsThresholds> m_invertedIntervals;
    void evolutionR(double x, unsigned int indexCurrentInterval);
};

#endif /* GPD_EVOLUTION_MODULE_H */
