#ifndef EVOL_QCD_MODULE_H
#define EVOL_QCD_MODULE_H

/**
 * @file EvolQCDModule.h
 * @author Bryan BERTHOU (SPhN / CEA Saclay)
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 21 October 2014
 * @version 1.0
 *
 * @class EvolQCDModule
 *
 * @brief
 */

#include <string>
#include <vector>

#include "../beans/gpd/GPDType.h"
#include "../beans/parton_distribution/PartonDistribution.h"
#include "../beans/PerturbativeQCDOrderType.h"
#include "../beans/QuarkFlavor.h"
#include "../utils/MatrixD.h"
#include "ModuleObject.h"

class GPDModule;

class EvolQCDModule: public ModuleObject {
public:
    enum Type {
        RELATIVE, ABSOLUTE, BOTH
    };

    EvolQCDModule(const std::string &className);
    virtual ~EvolQCDModule();
    virtual EvolQCDModule* clone() const = 0;

    virtual PartonDistribution compute(double x, double xi, double t,
            double MuF, double MuR, GPDModule* pGPDModule,
            PartonDistribution partonDistribution) = 0;

    /**
     * Checks if MuF (from compute parameters) is revelant
     */
    bool isRunnable(double MuF, double MuF_ref, EvolQCDModule::Type testType);

    // ##### GETTERS & SETTERS #####

    const GPDModule* getGpdModule() const;
    void setGpdModule(GPDModule* gpdModule);
    PerturbativeQCDOrderType::Type getQcdOrderType() const;
    void setQcdOrderType(PerturbativeQCDOrderType::Type qcdOrderType);
    int getEvolutionActiveFlavors() const;
    void setEvolutionActiveFlavors(int nfEvol);

    //TODO a verifier
    unsigned int getNbMuFPoints() const;
    virtual void setNbMuFPoints(unsigned int nbMuFPoints) = 0;
    unsigned int getNbXPoints() const;
    virtual void setNbXPoints(unsigned int nbXPoints) = 0;

protected:
    /**
     * Copy constructor
     */
    EvolQCDModule(const EvolQCDModule &other);

    double m_x;
    double m_xi;
    double m_t;
    double m_MuF;
    double m_MuR;
    double m_MuF_ref;

    GPDModule* m_pGPDModule;

    PerturbativeQCDOrderType::Type m_qcdOrderType;
    GPDType::Type m_currentGPDComputeType;

    //RunningAlphaStrong m_runningAlphaS;
    double m_alphaS;
    double m_scaleDistinction;

    int m_nfEvol;
    int m_nfMin;		///< nf_min donne la taille de la matrice
    unsigned int m_nbXPoints;	///< Max of points if adaptative method,
    unsigned int m_nbMuFPoints;	///< fixed otherwise
    double m_epsilon;
    double m_alpha;

    PartonDistribution m_partonDistribution;

    MatrixD m_currentConvertMatrix;
    MatrixD m_currentInvertMatrix;

    std::vector<double> m_vectorOfGPDCombination;
    //Matrix<double> m_matrixOfUnknownPartonDistribution; ///< HuPlus, HuMinus, ...

    void preCompute(double x, double xi, double t, double MuF, double MuR,
            GPDModule* pGPDModule,
            PartonDistribution partonDistribution);

    virtual void initModule();
    virtual void isModuleWellConfigured();

    std::vector<double> convertBasis(std::vector<double> vectorToConvert);
    std::vector<double> invertBasis(std::vector<double> vectorToInvert);

    PartonDistribution makeFinalPartonDistribution();
    std::vector<double> MakeVectorOfGPDCombinations(
            const PartonDistribution &partonDistribution);

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

    bool isRelativeTest(double MuF, double MuF_ref);
    bool isAbsoluteTest(double MuF, double MuF_ref);

    void initNfMin();
    void initMatrixValue();
    void initVectorOfGPDCombinations();

    QuarkDistribution makeFinalQuarkDistribution(QuarkFlavor::Type quarkFlavor,
            double quarkDistributionPlus, double quarkDistributionMinus);
    double calculateFq(double FMinus, double FPlus);
};

#endif /* EVOL_QCD_MODULE_H */
