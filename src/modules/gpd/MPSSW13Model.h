#ifndef MPSSW13_MODEL_H
#define MPSSW13_MODEL_H

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

class c_mstwpdf;
class GPDOutputData;
class GPDResultData;

class MPSSW13Model: public GPDModule {
public:
    static const std::string moduleID;
    MPSSW13Model();
    virtual ~MPSSW13Model();

    virtual MPSSW13Model* clone() const;

    void setParameters(std::vector<double> Parameters);

    double getCA() const;
    double getCF() const;
    double getF1d() const;
    double getF1u() const;
    double getFD() const;
    double getGluonDTerm() const;
    void setGluonDTerm(double gluonDTerm);
    double getMuF2() const;
    double getMuF2Ref() const;
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
    double m_MuF2;
    double m_MuF2_ref;

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
    double IntegralHuVal(double* Var, double* Par);
    double IntegralHuValMx(double* Var, double* Par);
    double IntegralxLargeHuSea(double* Var, double* Par);
    double IntegralxSmall1HuSea(double* Var, double* Par);
    double IntegralxSmall2HuSea(double* Var, double* Par);
    double IntegralHdVal(double* Var, double* Par);
    double IntegralHdValMx(double* Var, double* Par);
    double IntegralxLargeHdSea(double* Var, double* Par);
    double IntegralxSmall1HdSea(double* Var, double* Par);
    double IntegralxSmall2HdSea(double* Var, double* Par);
    double IntegralxLargeHsSea(double* Var, double* Par);
    double IntegralxSmall1HsSea(double* Var, double* Par);
    double IntegralxSmall2HsSea(double* Var, double* Par);
    double IntegralxLargeHg(double* Var, double* Par);
    double IntegralxSmall1Hg(double* Var, double* Par);
    double IntegralxSmall2Hg(double* Var, double* Par);

    double GammaQQ(const unsigned int nflavour, const unsigned int n);
    double GammaQG(const unsigned int nflavour, const unsigned int n);
    double GammaGQ(const unsigned int nflavour, const unsigned int n);
    double GammaGG(const unsigned int nflavour, const unsigned int n);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);
    virtual GPDResultData computeH(); ///< Compute GPD H at considered kinematics
};

#endif /* MPSSW13_H_ */
