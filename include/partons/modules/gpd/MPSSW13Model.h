#ifndef MPSSW13_MODEL_H
#define MPSSW13_MODEL_H

/**
 * @file MPSSW13Model.h
 * @author Hervé MOUTARDE (SPhN / CEA Saclay)
 * @date 19 December 2014
 * @version 1.0
 *
 * @class MPSSW13Model
 *
 * @brief
 */

/*
 * \file MPSSW13Model.h
 *
 * \brief Previous BDP class, used first in Moutarde et al, 2013, arXiv1301.3819.
 *
 * \date 11 nov. 2014
 * \author ”H. Moutarde (CEA/Irfu, Saclay)"
 *
 * \remarks
 */

#include <string>
#include <vector>

#include "../GPDModule.h"
#include "../MathIntegratorModule.h"

class c_mstwpdf;

class MPSSW13Model: public GPDModule, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    MPSSW13Model(const std::string &className);
    virtual ~MPSSW13Model();

    virtual MPSSW13Model* clone() const;

    virtual void init();

    void setParameters(std::vector<double> Parameters);

    double getCA() const;
    double getCF() const;
    double getF1d() const;
    double getF1u() const;
    double getFD() const;
    double getGluonDTerm() const;
    void setGluonDTerm(double gluonDTerm);
    double getMx() const;
    unsigned int getNbOfColor() const;
    unsigned int getNbOfQuarkFlavor() const;
    double getProfileShapeGlue() const;
    double getProfileShapeSea() const;
    double getProfileShapeVal() const;
    double getQuarkDTerm() const;
    double getTF() const;

protected:
    MPSSW13Model(const MPSSW13Model& other);

private:
    unsigned int m_NbOfQuarkFlavor;
    unsigned int m_NbOfColor;

    double m_Mx;

    double m_CA;
    double m_CF;
    double m_TF;

    double m_F1u;
    double m_F1d;
    double m_FD;

    double m_ProfileShapeVal;
    double m_ProfileShapeSea;
    double m_ProfileShapeGlue;

    double m_QuarkDTerm;
    double m_GluonDTerm;

    c_mstwpdf* m_Forward;

    void ComputeDTerms();
    void ComputeFormFactors();

    double Profile(double N, double alpha, double beta);
    double IntegralHuVal(std::vector<double> Var, std::vector<double> Par);
    double IntegralHuValMx(std::vector<double> Var, std::vector<double> Par);
    double IntegralxLargeHuSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxLargeHuSeaMx(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall1HuSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall2HuSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralHdVal(std::vector<double> Var, std::vector<double> Par);
    double IntegralHdValMx(std::vector<double> Var, std::vector<double> Par);
    double IntegralxLargeHdSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxLargeHdSeaMx(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall1HdSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall2HdSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxLargeHsSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxLargeHsSeaMx(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall1HsSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxSmall2HsSea(std::vector<double> Var,
            std::vector<double> Par);
    double IntegralxLargeHg(std::vector<double> Var, std::vector<double> Par);
    double IntegralxLargeHgMx(std::vector<double> Var, std::vector<double> Par);
    double IntegralxSmall1Hg(std::vector<double> Var, std::vector<double> Par);
    double IntegralxSmall2Hg(std::vector<double> Var, std::vector<double> Par);

    double GammaQQ(const unsigned int nflavour, const unsigned int n);
    double GammaQG(const unsigned int nflavour, const unsigned int n);
    double GammaGQ(const unsigned int nflavour, const unsigned int n);
    double GammaGG(const unsigned int nflavour, const unsigned int n);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);
    virtual PartonDistribution computeH(); ///< Compute GPD H at considered kinematics

    void throwBetaException(const std::string &funcName, double betaValue);
};

#endif /* MPSSW13_H_ */
