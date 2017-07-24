#ifndef GPD_GK16_NUMERICAL_H
#define GPD_GK16_NUMERICAL_H

/**
 * @file GPDGK16Numerical.h
 * @author Bryan BERTHOU (CEA Saclay)
 * @author Herve MOUTARDE (CEA Saclay)
 * @author Luca COLANERI (IPNOrsay)
 * @date 2016
 * @version 1.0
 */

#include <ElementaryUtils/parameters/Parameters.h>
#include <string>
#include <vector>

#include "../MathIntegratorModule.h"
#include "GPDModule.h"

//TODO finir les messages de debug dans toutes les fonctions
//TODO supprimer au maximum les pointers qui ne sont pas obligatoires

/**
 * @class GPDGK16Numerical
 *
 * @brief A module that implements GPD as defined by Kroll-Goloskokov model in 2011.
 *
 * This model is defined in the series of references:
 * - arxiv:hep-ph/0611290 \cite Goloskokov:2006hr ;
 * - arxiv:0708.3569 \cite Goloskokov:2007nt ;
 * - arxiv:0809.4126 \cite Goloskokov:2008ib ;
 * - arxiv:0906.0460 \cite Goloskokov:2009ia ;
 * - arxiv:1106.4897 \cite Goloskokov:2011rd ;
 * - arxiv:1210.6975 \cite Kroll2012sm ;
 * - arxiv:1407.1141 \cite Goloskokov:2014ika.
 *
 * Modified from GK16Model to perform numerical integration (and remove cln dependency).
 *
 * Available GPD types: H, E, Ht, Et.
 *
 * May 2017 : Update to correct pion pole issue in Et as in GK16Model.
 *
 */
class GPDGK16Numerical: public GPDModule, public MathIntegratorModule {
public:

    static const unsigned int classId; ///< Unique ID to automatically register the class in the registry.

    /**
     * Constructor.
     * See BaseObject::BaseObject and ModuleObject::ModuleObject for more details.
     *
     * @param className name of child class.
     */
    GPDGK16Numerical(const std::string &className);

    /**
     * Default destructor.
     */
    virtual ~GPDGK16Numerical();

    virtual GPDGK16Numerical* clone() const;

    virtual void configure(const ElemUtils::Parameters &parameters);

    virtual std::string toString() const;



protected:
    /**
     * Copy constructor.
     *
     * Use by the factory.
     *
     * @param other
     */
    GPDGK16Numerical(const GPDGK16Numerical& other);

    virtual void isModuleWellConfigured();
    virtual void initModule();

    //GPDResultData compute(GPDComputeType gpdComputeType);

    virtual PartonDistribution computeH();
    virtual PartonDistribution computeE();
    virtual PartonDistribution computeHt();
    virtual PartonDistribution computeEt();

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

private:
    double m_Mx;
    double kappa_s; // Flavour symmetry breaking factor, eq. (36)

    double Et_pole(double x); ///< Pion pole contribution to Et

    double Profile(double N, double alpha, double beta);
    double GKPdfAnsatz(double N, double beta, double* coeff);
    double GKPdfAnsatz_t(double N, double beta, double* coeff);
    double GKPdfAnsatz_tg(double N, double beta, double* coeff);
    double GKtDependentsDD(double beta, double alpha);
    double GKtDependentuDD_t(double beta, double alpha);
    double GKtDependentdDD_t(double beta, double alpha);
    double GKtDependentuValDD(double beta, double alpha);
    double GKtDependentdValDD(double beta, double alpha);
    double GKtDependentgDD(double beta, double alpha);

    double GKtDependentHtgDD(double beta, double alpha);

    //DD E
    double GKtDependentDD_EuVal(double beta, double alpha);
    double GKtDependentDD_EdVal(double beta, double alpha);
    double GKtDependentDD_Es(double beta, double alpha);
    double GKtDependentDD_Eg(double beta, double alpha);
    //DD Et
    double GKtDependentDD_EtuVal(double beta, double alpha);
    double GKtDependentDD_EtdVal(double beta, double alpha);

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
//integrals for Ht
    double IntegralxLargeHtg(double x, std::vector<double> Par);
    double IntegralxLargeHtgMx(double x, std::vector<double> Par);
    double IntegralxSmall1Htg(double x, std::vector<double> Par);
    double IntegralxSmall2Htg(double x, std::vector<double> Par);

//integrals for E
    double IntegralEuVal(double x, std::vector<double> Par);
    double IntegralEdVal(double x, std::vector<double> Par);
    double IntegralEuValMx(double x, std::vector<double> Par);
    double IntegralEdValMx(double x, std::vector<double> Par);
    double IntegralxLargeEsSea(double x, std::vector<double> Par);
    double IntegralxLargeEsSeaMx(double x, std::vector<double> Par);
    double IntegralxSmallEsSea(double x, std::vector<double> Par);
    double IntegralxSmall1EsSea(double x, std::vector<double> Par);
    double IntegralxSmall2EsSea(double x, std::vector<double> Par);

    double IntegralxLargeEgSea(double x, std::vector<double> Par);
    double IntegralxLargeEgSeaMx(double x, std::vector<double> Par);
    //   double IntegralxSmallEgSea(double x, std::vector<double> Par);
    double IntegralxSmall1EgSea(double x, std::vector<double> Par);
    double IntegralxSmall2EgSea(double x, std::vector<double> Par);
//integrals for Et
    double IntegralEtuVal(double x, std::vector<double> Par);
    double IntegralEtdVal(double x, std::vector<double> Par);
    double IntegralEtuValMx(double x, std::vector<double> Par);
    double IntegralEtdValMx(double x, std::vector<double> Par);

    void throwBetaException(const std::string &funcName, double betaValue);

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

    NumA::FunctionType1D* m_pIntegralxLargeHtg;
    NumA::FunctionType1D* m_pIntegralxSmall1Htg;
    NumA::FunctionType1D* m_pIntegralxSmall2Htg;
    NumA::FunctionType1D* m_pIntegralxLargeHtgMx;

    NumA::FunctionType1D* m_pIntegralEuVal;
    NumA::FunctionType1D* m_pIntegralEdVal;
    NumA::FunctionType1D* m_pIntegralEuValMx;
    NumA::FunctionType1D* m_pIntegralEdValMx;
    NumA::FunctionType1D* m_pIntegralxLargeEsSea;
    NumA::FunctionType1D* m_pIntegralxLargeEsSeaMx;
    NumA::FunctionType1D* m_pIntegralxSmallEsSea;
    NumA::FunctionType1D* m_pIntegralxSmall1EsSea;
    NumA::FunctionType1D* m_pIntegralxSmall2EsSea;
    NumA::FunctionType1D* m_pIntegralxLargeEgSea;
    NumA::FunctionType1D* m_pIntegralxLargeEgSeaMx;
    NumA::FunctionType1D* m_pIntegralxSmall1EgSea;
    NumA::FunctionType1D* m_pIntegralxSmall2EgSea;

    NumA::FunctionType1D* m_pIntegralEtuVal;
    NumA::FunctionType1D* m_pIntegralEtdVal;
    NumA::FunctionType1D* m_pIntegralEtuValMx;
    NumA::FunctionType1D* m_pIntegralEtdValMx;

};

#endif /* GPD_GK16_NUMERICAL_H */
