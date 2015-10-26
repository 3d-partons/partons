#ifndef GPD_EVOLUTION_MODULE_H
#define GPD_EVOLUTION_MODULE_H

/**
 * @file GPDEvolutionModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @date 01 September 2015
 * @version 1.0
 *
 * @class GPDEvolutionModule
 *
 * @brief
 */

#include <NumA/linear_algebra/vector/VectorD.h>
#include <string>

#include "../../beans/gpd/GPDType.h"
#include "../../beans/PerturbativeQCDOrderType.h"
#include "../../beans/QuarkFlavor.h"
#include "../../ModuleObject.h"

class ActiveFlavorsModule;
class GPDModule;
class MatrixD;
class NfInterval;
class PartonDistribution;
class QuarkDistribution;
class RunningAlphaStrongModule;

class GPDEvolutionModule: public ModuleObject {
public:
    enum Type {
        RELATIVE, ABSOLUTE, BOTH
    };

    GPDEvolutionModule(const std::string &className);
    virtual ~GPDEvolutionModule();
    virtual GPDEvolutionModule* clone() const = 0;

    virtual void configure(ParameterList parameters);

    virtual void init();

    PartonDistribution compute(double x, double xi, double t, double MuF2,
            double MuR2, GPDModule* pGPDModule, GPDType::Type gpdType);

    /**
     * Checks if MuF (from compute parameters) is revelant
     */
    bool isRunnable(double MuF2, double MuF2_ref,
            GPDEvolutionModule::Type testType);

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

    ActiveFlavorsModule* m_pNfFunction;
    ActiveFlavorsModule* m_pNfEvolFunction;
    RunningAlphaStrongModule *m_pRunningAlphaStrong;

    //TODO unused
    double m_scaleDistinction;

    unsigned short m_currentNf;
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

    virtual double nonSingletMuFDerivative(const NfInterval &nfInterval) = 0;
    virtual double singletMuFDerivative(const NfInterval &nfInterval) = 0;
    virtual double gluonMuFDerivative(const NfInterval &nfInterval) = 0;

private:
    static MatrixD conversionMatrix1;
    static MatrixD conversionMatrix2;
    static MatrixD conversionMatrix3;
    static MatrixD conversionMatrix4;
    static MatrixD conversionMatrix5;
    static MatrixD conversionMatrix6;

    static MatrixD invertMatrix1;
    static MatrixD invertMatrix2;
    static MatrixD invertMatrix3;
    static MatrixD invertMatrix4;
    static MatrixD invertMatrix5;
    static MatrixD invertMatrix6;

    bool isRelativeTest(double MuF2, double MuF2_ref);
    bool isAbsoluteTest(double MuF2, double MuF2_ref);

    QuarkDistribution makeFinalQuarkDistribution(QuarkFlavor::Type quarkFlavor,
            double quarkDistributionPlus, double quarkDistributionMinus);
    double calculateFq(double FMinus, double FPlus);

    // refactored

    PartonDistribution computeGPDModelAtMufRef();

    NumA::VectorD makeVectorOfGPDCombinations(
            const PartonDistribution &partonDistribution);
    void resizeVectorOfGPDCombination(NumA::VectorD &vectorOfQuarkDistribution,
            unsigned short nfInterval);

    PartonDistribution evolution(const PartonDistribution &partonDistribution);
    void evolution(const NfInterval &nfInterval);

    NumA::VectorD convertBasis(const NumA::VectorD &vectorToConvert,
            unsigned short matrixNum);
    double convertBasis(const NumA::VectorD &vectorToConvert,
            unsigned short currentNf, unsigned short nonSingletIndex);
    NumA::VectorD invertBasis(const NumA::VectorD &vectorToInvert,
            unsigned short matrixNum);

    void computeNonSinglet(const NfInterval &nfInterval);
    void computeSingletGluon(const NfInterval &nfInterval);
};

#endif /* GPD_EVOLUTION_MODULE_H */
