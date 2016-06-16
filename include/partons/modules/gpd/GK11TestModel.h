/*
 * GKK11TestModule.h
 *
 *  Created on: May 17, 2016
 *      Author: debian
 */

#ifndef GKK11TESTMODEL_H_
#define GKK11TESTMODEL_H_

/**
 * @file GK11Model.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author Herve MOUTARDE (CEA Saclay)
 * @author Luca COLANERI (IPNOrsay)
 * @date 2016
 * @version 3.0
 *
 * @class GK11TestModel
 *
 * @brief A module that implements GPD as defined by Kroll-Goloskokov model in 2011. Modified to perform numerical integration
 *
 *
 * !!!! Use tgamma() instead of gamma() (see: http://stackoverflow.com/questions/18116376/what-is-the-definition-for-gammadouble-x-and-why-is-it-different-on-two-gcc-ve)
 *
 *
 * August 2015 : Tested and approved by Pawel Sznajder
 */

#include <string>
#include <vector>

#include "../GPDModule.h"
#include "../MathIntegratorModule.h"
//TODO finir les messages de debug dans toutes les fonctions
//TODO supprimer au maximum les pointers qui ne sont pas obligatoires

class GK11TestModel: public GPDModule, public MathIntegratorModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Default constructor
     */
    // GK11Model();
    GK11TestModel(const std::string &className);

    /**
     * Default destructor
     */
    virtual ~GK11TestModel();

    virtual GK11TestModel* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString();

    // ##### GETTERS & SETTERS #####

    double getB0() const;
    double getC1() const;
    double getC2() const;
    double getC3() const;
    double getC4() const;
    double getC5() const;
    double getC6() const;
    double getC7() const;
    double getC8() const;
    const std::vector<double>& getEdval1mtab() const;
    const std::vector<double>& getEdval1tab() const;
    const std::vector<double>& getEi1tab() const;
    const std::vector<double>& getEs1tab() const;
    const std::vector<double>& getEtdval1mtab() const;
    const std::vector<double>& getEtdval1tab() const;
    const std::vector<double>& getEtuval1mtab() const;
    const std::vector<double>& getEtuval1tab() const;
    const std::vector<double>& getEuval1mtab() const;
    const std::vector<double>& getEuval1tab() const;
    double getEdValMx() const;
    double getEtdValMx() const;
    double getEtuValMx() const;
    double getEuValMx() const;
    double getHdValMx() const;
    double getHtdValMx() const;
    double getHtuValMx() const;
    double getHuValMx() const;
    double getL() const;
    const std::vector<double>& getHdval1mtab() const;
    const std::vector<double>& getHdval1tab() const;
    const std::vector<double>& getHi1tab() const;
    const std::vector<double>& getHs1tab() const;
    const std::vector<double>& getHtdval1mtab() const;
    const std::vector<double>& getHtdval1tab() const;
    const std::vector<double>& getHti1tab() const;
    const std::vector<double>& getHtuval1mtab() const;
    const std::vector<double>& getHtuval1tab() const;
    const std::vector<double>& getHuval1mtab() const;
    const std::vector<double>& getHuval1tab() const;
    double getKappaS() const;
    double getEdval() const;
    double getEgluon() const;
    double getEsea() const;
    double getEtdval() const;
    double getEtgluon() const;
    double getEtsea() const;
    double getEtuval() const;
    double getEuval() const;
    double getHdval() const;
    double getHgluon() const;
    double getHsea() const;
    double getHtdval() const;
    double getHtgluon() const;
    double getHtsea() const;
    double getHtuval() const;
    double getHuval() const;

protected:
    /**
     * Copy constructor
     *
     * Use by the factory
     *
     * @param other
     */
    GK11TestModel(const GK11TestModel& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);

    virtual PartonDistribution computeH(); ///< Compute GPD H at considered kinematics
    virtual PartonDistribution computeE(); ///< Compute GPD E at considered kinematics
    virtual PartonDistribution computeHt(); ///< Compute GPD Ht at considered kinematics
    virtual PartonDistribution computeEt(); ///< Compute GPD Et at considered kinematics

//    double computeSinglet(const QuarkDistribution &quarkDistribution_u,
//            const QuarkDistribution &quarkDistribution_d,
//            const QuarkDistribution &quarkDistribution_s);

    void calculateHCoefs();
    void calculateECoefs();
    void calculateHtCoefs();
    void calculateEtCoefs();

    double c1, c2, c3, c4, c5, c6, c7, c8; // Coefficients of the expansion of CTEQ PDF in terms of half-integer powers of beta (eq. (26))
    double b0; // Exponential t-dependence (eq. (22))
    double fL;
    double d0; //exponent appearing in PDF expasion (eq. (26))
    double alphaP; //coefficient for t and beta -dependence (eq. 22)

    std::vector<double> Hi1tab;               ///< Hi1(i=0,0.5,1,1.5) for gluons

private:
    double m_Mx;
    double kappa_s; // Flavour symmetry breaking factor, eq. (36)

    double fHuValMx;                                          ///< HuVal( - fx )
    double fHdValMx;                                          ///< HdVal( - fx )
    double fEuValMx;                                          ///< HuVal( - fx )
    double fEdValMx;                                          ///< HdVal( - fx )
    double fHtuValMx;                                         ///< HuVal( - fx )
    double fHtdValMx;                                         ///< HdVal( - fx )
    double fEtuValMx;                                         ///< HuVal( - fx )
    double fEtdValMx;                                         ///< HdVal( - fx )

    double kHgluon;                   ///< Exponent of correlated x-t dependence
    double kHsea;                     ///< Exponent of correlated x-t dependence
    double kHuval;                    ///< Exponent of correlated x-t dependence
    double kHdval;                    ///< Exponent of correlated x-t dependence

    double kEgluon;                   ///< Exponent of correlated x-t dependence
    double kEsea;                     ///< Exponent of correlated x-t dependence
    double kEuval;                    ///< Exponent of correlated x-t dependence
    double kEdval;                    ///< Exponent of correlated x-t dependence

    double kHtgluon;                  ///< Exponent of correlated x-t dependence
    double kHtsea;                    ///< Exponent of correlated x-t dependence
    double kHtuval;                   ///< Exponent of correlated x-t dependence
    double kHtdval;                   ///< Exponent of correlated x-t dependence

    double kEtgluon;                  ///< Exponent of correlated x-t dependence
    double kEtsea;                    ///< Exponent of correlated x-t dependence
    double kEtuval;                   ///< Exponent of correlated x-t dependence
    double kEtdval;                   ///< Exponent of correlated x-t dependence

    std::vector<double> Huval1tab;           ///< Hval1(i=0,0.5,1) for valence u
    std::vector<double> Hdval1tab;           ///< Hval1(i=0,0.5,1) for valence d
    std::vector<double> Huval1mtab;  ///< Hval1(i=0,0.5,1) for valence u for -xb
    std::vector<double> Hdval1mtab;  ///< Hval1(i=0,0.5,1) for valence d for -xb
    std::vector<double> Hs1tab; ///< Hs1(i=0,0.5,1,1.5) for sea, same for u, d, s

    std::vector<double> Euval1tab;               ///< Eval1(i=0,1) for valence u
    std::vector<double> Edval1tab;            ///< Eval1(i=0,..,7) for valence d
    std::vector<double> Euval1mtab;      ///< Eval1(i=0,1) for valence u for -xb
    std::vector<double> Edval1mtab;   ///< Eval1(i=0,..,7) for valence d for -xb
    std::vector<double> Es1tab;      ///< Es1(i=0,1,2) for sea, same for u, d, s
    std::vector<double> Ei1tab;                       ///< Ei1(i=0,1) for gluons

    std::vector<double> Htuval1tab;           ///< Htval1(i=0,1,2) for valence u
    std::vector<double> Htdval1tab;           ///< Htval1(i=0,1,2) for valence d
    std::vector<double> Htuval1mtab;  ///< Htval1(i=0,1,2) for valence u for -xb
    std::vector<double> Htdval1mtab;  ///< Htval1(i=0,1,2) for valence d for -xb
    std::vector<double> Hti1tab;                 ///< Hti1(i=0,0.5,1) for gluons

    std::vector<double> Etuval1tab;         ///< Etval1(i=0,1,2,3) for valence u
    std::vector<double> Etdval1tab;         ///< Etval1(i=0,1,2,3) for valence d
    std::vector<double> Etuval1mtab; ///< Etval1(i=0,1,2,3) for valence u for -xb
    std::vector<double> Etdval1mtab; ///< Etval1(i=0,1,2,3) for valence d for -xb

    void calculateHKas();
    void calculateEKas();
    void calculateHtKas();
    void calculateEtKas();

    double Et_pole(double x); ///< Pion pole contribution to Et

    double Hi1(double x, double i, double k);
    double Hs1(double x, double i, double k);
    double Hval1(double x, double i, double k);
    double Ei1(double x, double i, double k);
    double Es1(double x, double i, double k);
    double Eval1(double x, double i, double k);
    double Hti1(double x, double i, double k);
    double Htval1(double x, double i, double k);
    double Etval1(double x, double i, double k);

    double Hi1_alt(double x, double i, double k);
    double Hs1_alt(double x, double i, double k);
    double Hval1_alt(double x, double i, double k);
    double Ei1_alt(double x, double i, double k);
    double Es1_alt(double x, double i, double k);
    double Eval1_alt(double x, double i, double k);
    double Hti1_alt(double x, double i, double k);
    double Htval1_alt(double x, double i, double k);
    double Etval1_alt(double x, double i, double k);

    double Profile(double N, double alpha, double beta);
    double GKPdfAnsatz(double N, double beta, double* coeff);
    double GKPdfAnsatz_t(double N, double beta, double* coeff);
    double GKtDependentsDD(double beta, double alpha);
    double GKtDependentuDD_t(double beta, double alpha);
    double GKtDependentdDD_t(double beta, double alpha);
    double GKtDependentuValDD(double beta, double alpha);
    double GKtDependentdValDD(double beta, double alpha);
    double GKtDependentgDD(double beta, double alpha);

    void initFunctorsForIntegrations();
//    double IntegralHuVal(double x, std::vector<double> Par);
    double IntegralHdValBp(double x, std::vector<double> Par);
    double IntegralHdValBm(double x, std::vector<double> Par);
//    double IntegralHuValMx(double x, std::vector<double> Par);
//    double IntegralHdValMx(double x, std::vector<double> Par);
    double IntegralHg(double x, std::vector<double> Par);
    double IntegralHs(double x, std::vector<double> Par);
    double IntegralHsBm(double x, std::vector<double> Par);

    double IntegralHuVal(double x, std::vector<double> Par);
    double IntegralHtuVal(double x, std::vector<double> Par);
    double IntegralHtdVal(double x, std::vector<double> Par);
    double IntegralHuValMx(double x, std::vector<double> Par);
    double IntegralHtuValMx(double x, std::vector<double> Par);
    double IntegralHtdValMx(double x, std::vector<double> Par);
    double IntegralHdVal(double x, std::vector<double> Par);
    double IntegralHdValMx(double x, std::vector<double> Par);
    double IntegralxLargeHsSea(double x, std::vector<double> Par);
    double IntegralxLargeHsSeaMx(double x, std::vector<double> Par);
    double IntegralxSmallHsSea(double x, std::vector<double> Par);
    double IntegralxSmall1HsSea(double x, std::vector<double> Par);
    double IntegralxSmall2HsSea(double x, std::vector<double> Par);
    double IntegralxLargeHg(double x, std::vector<double> Par);
    double IntegralxLargeHgMx(double x, std::vector<double> Par);
    double IntegralxSmall1Hg(double x, std::vector<double> Par);
    double IntegralxSmall2Hg(double x, std::vector<double> Par);

    void throwBetaException(const std::string &funcName, double betaValue);
//    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHdValBp;
    NumA::FunctionType1D* m_pIntegralHdValBm;
//    NumA::FunctionType1D* m_pIntegralHuValMx;
//    NumA::FunctionType1D* m_pIntegralHdValMx;
    NumA::FunctionType1D* m_pIntegralHg;
    NumA::FunctionType1D* m_pIntegralHs;
    NumA::FunctionType1D* m_pIntegralHsBm;

    NumA::FunctionType1D* m_pIntegralHuVal;
    NumA::FunctionType1D* m_pIntegralHtuVal;
    NumA::FunctionType1D* m_pIntegralHdVal;
    NumA::FunctionType1D* m_pIntegralHtdVal;
    NumA::FunctionType1D* m_pIntegralHuValMx;
    NumA::FunctionType1D* m_pIntegralHtuValMx;
    NumA::FunctionType1D* m_pIntegralHdValMx;
    NumA::FunctionType1D* m_pIntegralHtdValMx;
    NumA::FunctionType1D* m_pIntegralxLargeHuSea;
    NumA::FunctionType1D* m_pIntegralxLargeHdSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HuSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HuSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HdSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HdSea;
    NumA::FunctionType1D* m_pIntegralxLargeHuSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHdSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHsSea;
    NumA::FunctionType1D* m_pIntegralxSmallHsSea;
    NumA::FunctionType1D* m_pIntegralxSmall1HsSea;
    NumA::FunctionType1D* m_pIntegralxSmall2HsSea;
    NumA::FunctionType1D* m_pIntegralxLargeHsSeaMx;
    NumA::FunctionType1D* m_pIntegralxLargeHg;
    NumA::FunctionType1D* m_pIntegralxSmall1Hg;
    NumA::FunctionType1D* m_pIntegralxSmall2Hg;
    NumA::FunctionType1D* m_pIntegralxLargeHgMx;

};

#endif /* GK11_MODEL_H */
