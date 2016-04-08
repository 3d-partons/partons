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

namespace NumA {
class FunctionType1D;
} /* namespace NumA */

class c_mstwpdf;

class MPSSW13Model: public GPDModule, public MathIntegratorModule {
public:
    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    MPSSW13Model(const std::string &className);
    virtual ~MPSSW13Model();

    virtual MPSSW13Model* clone() const;

    virtual void resolveObjectDependencies();

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
    double IntegralHuVal(double x, std::vector<double> Par);
    double IntegralHuValMx(double x, std::vector<double> Par);
    double IntegralxLargeHuSea(double x, std::vector<double> Par);
    double IntegralxLargeHuSeaMx(double x, std::vector<double> Par);
    double IntegralxSmall1HuSea(double x, std::vector<double> Par);
    double IntegralxSmall2HuSea(double x, std::vector<double> Par);
    double IntegralHdVal(double x, std::vector<double> Par);
    double IntegralHdValMx(double x, std::vector<double> Par);
    double IntegralxLargeHdSea(double x, std::vector<double> Par);
    double IntegralxLargeHdSeaMx(double x, std::vector<double> Par);
    double IntegralxSmall1HdSea(double x, std::vector<double> Par);
    double IntegralxSmall2HdSea(double x, std::vector<double> Par);
    double IntegralxLargeHsSea(double x, std::vector<double> Par);
    double IntegralxLargeHsSeaMx(double x, std::vector<double> Par);
    double IntegralxSmall1HsSea(double x, std::vector<double> Par);
    double IntegralxSmall2HsSea(double x, std::vector<double> Par);
    double IntegralxLargeHg(double x, std::vector<double> Par);
    double IntegralxLargeHgMx(double x, std::vector<double> Par);
    double IntegralxSmall1Hg(double x, std::vector<double> Par);
    double IntegralxSmall2Hg(double x, std::vector<double> Par);

    double GammaQQ(const unsigned int nflavour, const unsigned int n);
    double GammaQG(const unsigned int nflavour, const unsigned int n);
    double GammaGQ(const unsigned int nflavour, const unsigned int n);
    double GammaGG(const unsigned int nflavour, const unsigned int n);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);
    virtual PartonDistribution computeH(); ///< Compute GPD H at considered kinematics

    void throwBetaException(const std::string &funcName, double betaValue);

    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHdVal;
    NumA::FunctionType1D* m_pIntegralHuValMx;
    NumA::FunctionType1D* m_pIntegralHdValMx;
    NumA::FunctionType1D* m_pIntegralxLargeHuSea;
    NumA::FunctionType1D* m_pIntegralxLargeHdSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HuSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HuSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HdSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HdSea;
    NumA::FunctionType1D* m_pIntegralxLargeHuSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHdSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHsSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HsSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HsSea;
    NumA::FunctionType1D* m_pIntegralxLargeHsSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHg;
    NumA::FunctionType1D* m_pIntegralxSmall1Hg;
    NumA::FunctionType1D* m_pIntegralxSmall2Hg;
    NumA::FunctionType1D* m_pIntegralxLargeHgMx;

    void initFunctorsForIntegrations();
};

#endif /* MPSSW13_H_ */
