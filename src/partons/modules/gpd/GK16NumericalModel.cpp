#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/parameters/Parameters.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <NumA/integration/one_dimension/Integrator1D.h>
#include <NumA/integration/one_dimension/IntegratorType1D.h>
#include <algorithm>
#include <cmath>
#include <map>
#include <utility>
#include <NumA/functor/one_dimension/Functor1D.h>

#include "../../../../include/partons/beans/gpd/GPDType.h"
#include "../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/PartonDistribution.h"
#include "../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../include/partons/beans/QuarkFlavor.h"
#include "../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../include/partons/FundamentalPhysicalConstants.h"
#include "../../../../include/partons/modules/gpd/GK16NumericalModel.h"

// Initialise [class]::classId with a unique name.
const unsigned int GK16NumericalModel::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new GK16NumericalModel("GK11TestModel"));

//TODO initialise missing members
GK16NumericalModel::GK16NumericalModel(const std::string &className) :
        GPDModule(className) {
    m_nf = 3;
    fL = 0.;
    m_MuF2_ref = 4.;

    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::H, &GPDModule::computeH));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::E, &GPDModule::computeE));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Ht, &GPDModule::computeHt));
    m_listGPDComputeTypeAvailable.insert(
            std::make_pair(GPDType::Et, &GPDModule::computeEt));

    MathIntegratorModule();
    initFunctorsForIntegrations();
}

GK16NumericalModel::GK16NumericalModel(const GK16NumericalModel& other) :
        GPDModule(other) {
    c1 = other.c1;
    c2 = other.c2;
    c3 = other.c3;
    c4 = other.c4;
    c5 = other.c5;
    c6 = other.c6;
    c7 = other.c7;
    c8 = other.c8;
    b0 = other.b0;
    kappa_s = other.kappa_s;
    fL = other.fL;

    MathIntegratorModule();
    initFunctorsForIntegrations();
}

GK16NumericalModel* GK16NumericalModel::clone() const {
    return new GK16NumericalModel(*this);
}

GK16NumericalModel::~GK16NumericalModel() {
}

void GK16NumericalModel::configure(const ElemUtils::Parameters &parameters) {
    GPDModule::configure(parameters);
}

//TODO implement
void GK16NumericalModel::isModuleWellConfigured() {
    GPDModule::isModuleWellConfigured();

}

void GK16NumericalModel::initModule() {
    GPDModule::initModule();

    fL = log(m_MuF2 / m_MuF2_ref); // Logarithmic dependence on the scale

    /*   debug(__func__,
     ElemUtils::Formatter() << "fMuF2 = " << fMuF2 << " fL = " << fL);*/
}

void GK16NumericalModel::initFunctorsForIntegrations() {
    setIntegrator(NumA::IntegratorType1D::DEXP);

//Integrators for H

    m_pIntegralHuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHuVal);

    m_pIntegralHdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHdVal);

    m_pIntegralHuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHuValMx);

    m_pIntegralHdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHdValMx);

    m_pIntegralxLargeHsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHsSea);

    m_pIntegralxSmallHsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmallHsSea);

    m_pIntegralxSmall1HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall1HsSea);

    m_pIntegralxSmall2HsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall2HsSea);

    m_pIntegralxLargeHsSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHsSeaMx);

    m_pIntegralxLargeHg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHg);

    m_pIntegralxSmall1Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall1Hg);

    m_pIntegralxSmall2Hg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall2Hg);

    m_pIntegralxLargeHgMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHgMx);

    //integrators for Ht

    m_pIntegralHtuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHtuVal);

    m_pIntegralHtdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHtdVal);

    m_pIntegralHtuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHtuValMx);

    m_pIntegralHtdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralHtdValMx);

    m_pIntegralxLargeHtg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHtg);

    m_pIntegralxSmall1Htg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall1Htg);

    m_pIntegralxSmall2Htg = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall2Htg);

    m_pIntegralxLargeHtgMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeHtgMx);

//integrators for E

    m_pIntegralEuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEuVal);

    m_pIntegralEdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEdVal);

    m_pIntegralEuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEuValMx);

    m_pIntegralEdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEdValMx);

    m_pIntegralxLargeEsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeEsSea);
    m_pIntegralxLargeEsSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeEsSeaMx);

    m_pIntegralxSmallEsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmallEsSea);

    m_pIntegralxSmall1EsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall1EsSea);

    m_pIntegralxSmall2EsSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall2EsSea);

    m_pIntegralxLargeEgSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeEgSea);
    m_pIntegralxLargeEgSeaMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxLargeEgSeaMx);

    m_pIntegralxSmall1EgSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall1EgSea);

    m_pIntegralxSmall2EgSea = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralxSmall2EgSea);

    //integrators for Et

    m_pIntegralEtuVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEtuVal);

    m_pIntegralEtdVal = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEtdVal);

    m_pIntegralEtuValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEtuValMx);

    m_pIntegralEtdValMx = NumA::Integrator1D::newIntegrationFunctor(this,
            &GK16NumericalModel::IntegralEtdValMx);
}

void GK16NumericalModel::throwBetaException(const std::string &funcName,
        double betaValue) {
    throw ElemUtils::CustomException(getClassName(), funcName,
            ElemUtils::Formatter()
                    << "Longitudinal momentum fraction should be in ] 0., +1. ]"
                    << '\n' << "Here beta = " << betaValue << '\n');
}
//Profile function
double GK16NumericalModel::Profile(double N, double beta, double alpha) {
    double profile = 0.;
    double ProfileShape = N;
    double TwiceProfileShapePlus1 = 2. * ProfileShape + 1;

    double alphaBeta = fabs(alpha) + fabs(beta);

    //TODO uncomment this test !
//    if (alphaBeta > 1.) {
//
//        error(__func__,
//                ElemUtils::Formatter()
//                        << "GK11TestModel: Parameters of profile function should be in rhombus | alpha | + | beta | <= 1."
//                        << '\n' << "Here alpha = " << alpha << " beta = "
//                        << beta << " | alpha | + | beta | = " << alphaBeta
//                        << " for GPD " << GPDType(m_gpdType).toString()
//                        << '\n');
//    }

    profile = pow((1. - fabs(beta)) * (1. - fabs(beta)) - alpha * alpha,
            ProfileShape);
    profile /= pow(1. - fabs(beta), TwiceProfileShapePlus1);
    profile *= tgamma(TwiceProfileShapePlus1 + 1.);
    profile /= (pow(2., TwiceProfileShapePlus1) * tgamma(ProfileShape + 1.)
            * tgamma(ProfileShape + 1.));

    return profile;
}
//forward limit ansatz for H
double GK16NumericalModel::GKPdfAnsatz(double N, double beta, double* coeff) {

    return pow(beta, -coeff[4]) * pow((1. - beta), 2 * N + 1)
            * (coeff[0] + coeff[1] * sqrt(beta) + coeff[2] * beta
                    + coeff[3] * beta * sqrt(beta));
}
//Forward limit ansatz for Ht
double GK16NumericalModel::GKPdfAnsatz_t(double N, double beta, double* coeff) {
// as in arXiv:1210.6975v3 pag 9
    return pow(beta, -coeff[4]) * pow((1. - beta), 2 * N + 1)
            * (coeff[0] + coeff[1] * beta + coeff[2] * beta * beta) * coeff[3];
}

double GK16NumericalModel::GKPdfAnsatz_tg(double N, double beta, double* coeff) {
// as in arXiv:1210.6975v3 pag 9
    return pow(beta, -coeff[4]) * pow((1. - beta), 2 * N + 1)

    * (coeff[0] + coeff[1] * sqrt(beta) + coeff[2] * beta) * coeff[3];
}

//DD for H
double GK16NumericalModel::GKtDependentsDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = 0.123 + 0.0003 * fL; // See table 1 p. 12
    c2 = -0.327 - 0.004 * fL; // See table 1 p. 12
    c3 = 0.692 - 0.068 * fL; // See table 1 p. 12
    c4 = -0.486 + 0.038 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    d0 = 1 + 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    alphaP = 0.15;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    return exp(b0 * m_t) * pow(absbeta, -alphaP * m_t)
            * GKPdfAnsatz(2, absbeta, coeff) * Profile(2, beta, alpha);
}
double GK16NumericalModel::GKtDependentuValDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    double uValDD;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = 1.52 + 0.248 * fL; // See eq. (27)
    c2 = 2.88 - 0.94 * fL; // See eq. (27)
    c3 = -0.095 * fL; // See eq. (27)
    c4 = 0.;
    d0 = 0.48;
    double coeff[5] = { c1, c2, c3, c4, d0 };
    alphaP = 0.9;

    if (beta > 0.) {
        uValDD = pow(absbeta, -alphaP * m_t) * GKPdfAnsatz(1, beta, coeff)
                * Profile(1, beta, alpha);
    } else {
        uValDD = 0.;
    }

    return uValDD;
}

double GK16NumericalModel::GKtDependentdValDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    double dValDD;
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = 0.76 + 0.248 * fL; // See table 1 p. 12
    c2 = 3.11 - 1.36 * fL; // See table 1 p. 12
    c3 = -3.99 + 1.15 * fL; // See table 1 p. 12
    c4 = 0.;
    d0 = 0.48 + 0.006 * fL;
//    d0 = 0.48;
    double coeff[5] = { c1, c2, c3, c4, d0 };
    alphaP = 0.9 - 0.05 * fL;

    if (beta > 0.) {
        dValDD = pow(absbeta, -alphaP * m_t) * GKPdfAnsatz(1, beta, coeff)
                * Profile(1, beta, alpha);
    } else {
        dValDD = 0.;
    }

    return dValDD;
}

double GK16NumericalModel::GKtDependentgDD(double beta, double alpha) {
    double absbeta = fabs(beta);
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = 2.23 + 0.362 * fL; // See table 1 p. 12
    c2 = 5.43 - 7.0 * fL; // See table 1 p. 12
    c3 = -34.0 + 22.5 * fL; // See table 1 p. 12
    c4 = 40.6 - 21.6 * fL; // See table 1 p. 12
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
//    d0 = 1 + 0.10 + 0.06 * fL - 0.0027 * fL * fL;
    d0 = 1 + 0.10 + 0.06 * fL;
    alphaP = 0.15;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    return exp(b0 * m_t) * pow(absbeta, -alphaP * m_t)
            * GKPdfAnsatz(2, absbeta, coeff) * Profile(2, beta, alpha);
}

double GK16NumericalModel::GKtDependentuDD_t(double beta, double alpha) {
    double absbeta = fabs(beta);
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = 0.170 + 0.03 * fL; //parameters from GK3 table 4  and note 11 december 2008
    c2 = 1.340 - 0.02 * fL;
    c3 = 0.120 - 0.40 * fL;

    c4 = 0.926;

    //etau = 0.926;

    b0 = 0.;

    d0 = 0.48;
    alphaP = 0.45;
    double alphaH = d0 + alphaP * m_t;

    double coeff[5] = { c1, c2, c3, c4, d0 };
    double A = tgamma(1 - d0) * tgamma(4) / tgamma(5 - d0) // as in arXiv:1210.6975v3 pag 9 eq 31.
            * (coeff[0] + coeff[1] * (1 - d0) / (5 - d0)
                    + coeff[2] * (2 - d0) * (1 - d0) / ((6 - d0) * (5 - d0)));

    return exp(b0 * m_t) * pow(absbeta, -alphaH) / pow(absbeta, -d0)
            * GKPdfAnsatz_t(1, absbeta, coeff) * Profile(1, beta, alpha) / A;

}
//DD for Ht

double GK16NumericalModel::GKtDependentdDD_t(double beta, double alpha) {
    double absbeta = fabs(beta);
    /*    if (beta <= 0 || beta > 1.) {
     throwBetaException(__func__, x);
     }*/
    c1 = -0.320 - 0.040 * fL; // as in arXiv:1210.6975v3 table 2
    c2 = -1.427 - 0.176 * fL;
    c3 = 0.692 - 0.068 * fL;

    c4 = -0.341; //etad in the paper

    b0 = 0.;

    d0 = 0.48;
    alphaP = 0.45;
    double alphaH = d0 + alphaP * m_t;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    double A = tgamma(1 - d0) * tgamma(4) / tgamma(5 - d0) // as in arXiv:1210.6975v3 pag 9 eq 31.
            * (coeff[0] + coeff[1] * (1 - d0) / (5 - d0)
                    + coeff[2] * (2 - d0) * (1 - d0) / ((6 - d0) * (5 - d0)));

    return exp(b0 * m_t) * pow(absbeta, -alphaH) / pow(absbeta, -d0)
            * GKPdfAnsatz_t(1, absbeta, coeff) * Profile(1, beta, alpha) / A;

}

double GK16NumericalModel::GKtDependentHtgDD(double beta, double alpha) {

    double absbeta = fabs(beta);

    c1 = 3.39 - 0.864 * fL;
    c2 = 1.73 + 0.24 * fL - 0.17 * fL * fL;
    c3 = 0.42 - 0.115 * fL - 0.069 * fL * fL;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2));

    c4 = 1;

    alphaP = 0.15; //slope of gluon trajectory
    d0 = -0.78 + 0.17 * fL;

    double alphaH = d0 + alphaP * m_t;
    double coeff[5] = { c1, c2, c3, c4, d0 };

    return exp(b0 * m_t) * pow(absbeta, -alphaH) / pow(absbeta, -d0)
            * GKPdfAnsatz_tg(2, absbeta, coeff) * Profile(2, beta, alpha);

}
//DD for E
double GK16NumericalModel::GKtDependentDD_EuVal(double beta, double alpha) {
    double absbeta = fabs(beta);

    double EuVal;
    double betauVal = 4;
    double ku = 1.67;
    alphaP = 0.9;
    double alphaval = 0.48;

    b0 = 0.;
    double B = tgamma(1 - alphaval) * tgamma(1 + betauVal)
            / tgamma(2 - alphaval + betauVal);
    if (beta > 0) {
        EuVal = pow(absbeta, -alphaP * m_t) * ku * pow(absbeta, -alphaval)
                * pow(1 - beta, betauVal) * Profile(1, beta, alpha) / B;
    } else {
        EuVal = 0;
    }
    return EuVal;

}

double GK16NumericalModel::GKtDependentDD_EdVal(double beta, double alpha) {
    double absbeta = fabs(beta);

    double EdVal;
    double betauVal = 5.6;
    double ku = -2.03;
    alphaP = 0.9;
    double alphaval = 0.48;

    b0 = 0.;
    double B = tgamma(1 - alphaval) * tgamma(1 + betauVal)
            / tgamma(2 - alphaval + betauVal);
    if (beta > 0) {
        EdVal = pow(absbeta, -alphaP * m_t) * ku * pow(absbeta, -alphaval)
                * pow(1 - beta, betauVal) * Profile(1, beta, alpha) / B;
    } else {
        EdVal = 0;
    }
    return EdVal;

}

double GK16NumericalModel::GKtDependentDD_Es(double beta, double alpha) {
    double absbeta = fabs(beta);
    double Es;
    double betas = 7;
    double Ns = -0.155;
    double alphahg = 1.10 + 0.06 * fL - 0.0027 * fL * fL;
    double deltag = alphahg - 1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    alphaP = 0.15;
    Es = exp(b0 * m_t) * pow(absbeta, -alphaP * m_t) * Ns
            * pow(absbeta, -1 - deltag) * pow(1 - absbeta, betas)
            * Profile(2, beta, alpha);

    return Es;

}

double GK16NumericalModel::GKtDependentDD_Eg(double beta, double alpha) {
    double absbeta = fabs(beta);
    double Eg;
    double betag = 6;
    double Ng = 0.779;
    double alphahg = 1.10 + 0.06 * fL - 0.0027 * fL * fL;
    double deltag = alphahg - 1;
    b0 = 2.58 + 0.25 * log(0.880354 / (0.880354 + m_MuF2)); // See eq. (39) p. 14
    alphaP = 0.15;
    Eg = exp(b0 * m_t) * pow(absbeta, -alphaP * m_t) * Ng
            * pow(absbeta, -deltag) * pow(1 - absbeta, betag)
            * Profile(2, beta, alpha);

    return Eg;

}
//DDs for Et
double GK16NumericalModel::GKtDependentDD_EtuVal(double beta, double alpha) {
    double absbeta = fabs(beta);
    double EtuVal;
    double N = 14;
    alphaP = 0.45;
    double alphaval = 0.48;

    b0 = 0.9;

    if (beta > 0) {
        EtuVal = exp(b0 * m_t) * pow(absbeta, -alphaP * m_t) * N
                * pow(absbeta, -alphaval) * pow(1 - beta, 5)
                * Profile(1, beta, alpha);
    } else {
        EtuVal = 0;
    }
    return EtuVal;

}

double GK16NumericalModel::GKtDependentDD_EtdVal(double beta, double alpha) {
    double absbeta = fabs(beta);
    double EduVal;
    double N = 4;
    alphaP = 0.45;
    double alphaval = 0.48;

    b0 = 0.9;

    if (beta > 0) {
        EduVal = exp(b0 * m_t) * pow(absbeta, -alphaP * m_t) * N
                * pow(absbeta, -alphaval) * pow(1 - beta, 5)
                * Profile(1, beta, alpha);
    } else {
        EduVal = 0;
    }
    return EduVal;
}

//Integrals for GPD Et
double GK16NumericalModel::IntegralEtuVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EtuVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEtdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EtdVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEtuValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EtuVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEtdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EtdVal(beta, alpha) / m_xi;

}

//Integrals for GPD E
double GK16NumericalModel::IntegralEuVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EuVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EdVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEuValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EuVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralEdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_EdVal(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralxLargeEsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Es(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeEsSeaMx(double beta,
        std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Es(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmallEsSea(double beta,
        std::vector<double> Par) {
    return (GKtDependentDD_Es(beta, (m_x - beta) / m_xi)
            - GKtDependentDD_Es(beta, (m_x + beta) / m_xi)) / m_xi;
}

double GK16NumericalModel::IntegralxSmall1EsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Es(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall2EsSea(double beta,
        std::vector<double> Par) {
    double Integral;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Es(beta, (m_x + beta) / m_xi) / m_xi;
}

double GK16NumericalModel::IntegralxLargeEgSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Eg(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeEgSeaMx(double beta,
        std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Eg(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall1EgSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Eg(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall2EgSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentDD_Eg(beta, alpha) / m_xi;
}

//inetgrals for H
double GK16NumericalModel::IntegralHuVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentuValDD(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralHuValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentuValDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralHdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentdValDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralHdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentdValDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeHg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return beta * GKtDependentgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeHgMx(double beta, std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return beta * GKtDependentgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall1Hg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return beta * GKtDependentgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall2Hg(double beta, std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return beta * GKtDependentgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeHsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentsDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeHsSeaMx(double beta,
        std::vector<double> Par) {
    double alpha = (m_x + beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentsDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmallHsSea(double beta,
        std::vector<double> Par) {
    return (GKtDependentsDD(beta, (m_x - beta) / m_xi)
            - GKtDependentsDD(beta, (m_x + beta) / m_xi)) / m_xi;
}

double GK16NumericalModel::IntegralxSmall1HsSea(double beta,
        std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentsDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall2HsSea(double beta,
        std::vector<double> Par) {
    double Integral;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentsDD(beta, (m_x + beta) / m_xi) / m_xi;
}

//Integrals for Ht
double GK16NumericalModel::IntegralHtuVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentuDD_t(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralHtdVal(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentdDD_t(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralHtuValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentuDD_t(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralHtdValMx(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentdDD_t(beta, alpha) / m_xi;

}

double GK16NumericalModel::IntegralxLargeHtg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentHtgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxLargeHtgMx(double beta,
        std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentHtgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall1Htg(double beta, std::vector<double> Par) {
    double alpha = (m_x - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentHtgDD(beta, alpha) / m_xi;
}

double GK16NumericalModel::IntegralxSmall2Htg(double beta, std::vector<double> Par) {
    double alpha = (m_Mx - beta) / m_xi;

    if (beta <= 0 || beta > 1.) {
        throwBetaException(__func__, beta);
    }

    return GKtDependentHtgDD(beta, alpha) / m_xi;
}

PartonDistribution GK16NumericalModel::computeH() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Eps = 1.e-9;
    m_Mx = -m_x;

// Integration limits and methods
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta2Min = std::min<double>(Beta2, Beta2Mx);
    std::vector<double> emptyParameters;

// Gluons

    double Hg = 0.;

    if (m_x >= m_xi) {
        // Integration
        Hg = integrate(m_pIntegralxLargeHg, Beta1, Beta2, emptyParameters);

    }

    if (fabs(m_x) < m_xi) {
        Hg = integrate(m_pIntegralxSmall1Hg, 0., Beta2, emptyParameters);
        Hg += integrate(m_pIntegralxSmall2Hg, 0., Beta2Mx, emptyParameters);
    }

    if (m_x <= -m_xi) {
        // Integration
        Hg = integrate(m_pIntegralxLargeHgMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    GluonDistribution gluonDistribution(Hg);

// s quark
    double Hs = 0;

    if (m_x >= m_xi) {
        Hs = integrate(m_pIntegralxLargeHsSea, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        Hs = integrate(m_pIntegralxSmallHsSea, Eps, Beta2Min, emptyParameters);
        Hs += integrate(m_pIntegralxSmall1HsSea, Beta2Min, Beta2,
                emptyParameters);
        Hs -= integrate(m_pIntegralxSmall2HsSea, Beta2Min, Beta2Mx,
                emptyParameters);

    }

    if (m_x <= -m_xi) {
        Hs = -integrate(m_pIntegralxLargeHsSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    quarkDistribution_s.setQuarkDistribution(Hs);

// u-d quark, valence part evaluated at fx

    double HuVal = 0.;
    double HdVal = 0.;

    if (m_x >= m_xi) {
        // Integration, u quark
        HuVal = integrate(m_pIntegralHuVal, Beta1, Beta2, emptyParameters);

        // Integration, d quark
        HdVal = integrate(m_pIntegralHdVal, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        HuVal = integrate(m_pIntegralHuVal, Eps, Beta2, emptyParameters);

        // Integration, d quark
        HdVal = integrate(m_pIntegralHdVal, Eps, Beta2, emptyParameters);
    }

///////////////////////////////////////////////////////////////////////
//   u and d quarks, valence part evaluated at fMx (instead of fx)   //
///////////////////////////////////////////////////////////////////////

    double HuValMx = 0.;
    double HdValMx = 0.;

    if (m_Mx >= m_xi) {

        // Integration, u quark
        HuValMx = integrate(m_pIntegralHuValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        HdValMx = integrate(m_pIntegralHdValMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    if (fabs(m_Mx) < m_xi) {
        // Integration, u quark
        HuValMx = integrate(m_pIntegralHuValMx, Eps, Beta2Mx, emptyParameters);

        // Integration, d quark
        HdValMx = integrate(m_pIntegralHdValMx, Eps, Beta2Mx, emptyParameters);
    }

// u and d quarks, sea part

    kappa_s = 1. + 0.68 / (1. + 0.52 * log(m_MuF2 / m_MuF2_ref)); // See eq. (36)

    double uSea = kappa_s * quarkDistribution_s.getQuarkDistribution(); // See eq. (35)
    double dSea = uSea;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(HuVal + uSea);
    quarkDistribution_d.setQuarkDistribution(HdVal + dSea);

// Set Hq(+)
    quarkDistribution_u.setQuarkDistributionPlus(HuVal - HuValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(HdVal - HdValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(
            2 * quarkDistribution_s.getQuarkDistribution());

// Set Hq(-)
    quarkDistribution_u.setQuarkDistributionMinus(HuVal + HuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(HdVal + HdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK16NumericalModel::computeHt() {

    debug(__func__, "Entered function ...");

    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double delta, etau, etad, Nu, Nd;
    // Integration limits and methods
    m_Mx = -m_x;
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper
    double Eps = 1.e-9;
    std::vector<double> emptyParameters;
    // Gluons

    double Htg = 0.;

    if (m_x >= m_xi) {
        // Integration
        Htg = integrate(m_pIntegralxLargeHtg, Beta1, Beta2, emptyParameters);

    }

    if (fabs(m_x) < m_xi) {

        if (m_x > 0) {
            Htg = integrate(m_pIntegralxSmall1Htg, Eps, Beta2, emptyParameters);

            Htg -= integrate(m_pIntegralxSmall2Htg, Eps, Beta2Mx,
                    emptyParameters);
        }

        if (m_x < 0) {

            Htg = integrate(m_pIntegralxSmall2Htg, Eps, Beta2Mx,
                    emptyParameters);

            Htg -= integrate(m_pIntegralxSmall1Htg, Eps, Beta2,
                    emptyParameters);
        }

    }

    if (m_x <= -m_xi) {
        // Integration
        Htg = integrate(m_pIntegralxLargeHtgMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    GluonDistribution gluonDistribution(Htg);

// s quark,  Ht_sea = 0 for GK
    quarkDistribution_s.setQuarkDistribution(0.);

    // u and d quark, valence part
    double HtuVal = 0.;
    double HtdVal = 0.;

    if (m_x >= m_xi) {
        // Integration, u quark
        HtuVal = integrate(m_pIntegralHtuVal, Beta1, Beta2, emptyParameters);
        // Integration, d quark
        HtdVal = integrate(m_pIntegralHtdVal, Beta1, Beta2, emptyParameters);

    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        HtuVal = integrate(m_pIntegralHtuVal, Eps, Beta2, emptyParameters);
        // Integration, d quark
        HtdVal = integrate(m_pIntegralHtdVal, Eps, Beta2, emptyParameters);

    }

    // u and d quark, valence part for Mx
    double HtuValMx = 0.;
    double HtdValMx = 0.;

    if (m_Mx >= m_xi) {

        // Integration, u quark
        HtuValMx = integrate(m_pIntegralHtuValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark

        HtdValMx = integrate(m_pIntegralHtdValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

    }

    if (fabs(m_Mx) < m_xi) {

        // Integration, u quark
        HtuValMx = integrate(m_pIntegralHtuValMx, Eps, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        HtdValMx = integrate(m_pIntegralHtdValMx, Eps, Beta2Mx,
                emptyParameters);
    }

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(HtuVal + uSea);
    quarkDistribution_d.setQuarkDistribution(HtdVal + dSea);

// Set Htq(+)
    quarkDistribution_u.setQuarkDistributionPlus(HtuVal + HtuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(HtdVal + HtdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// Set Htq(-)
    quarkDistribution_u.setQuarkDistributionMinus(HtuVal - HtuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(HtdVal - HtdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK16NumericalModel::computeE() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Eps = 1.e-9;
    m_Mx = -m_x;

// Integration limits and methods
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta2Min = std::min<double>(Beta2, Beta2Mx);
    std::vector<double> emptyParameters;

    // Gluons

    double Eg = 0.;

    if (m_x >= m_xi) {
        // Integration
        Eg = integrate(m_pIntegralxLargeEgSea, Beta1, Beta2, emptyParameters);

    }

    if (fabs(m_x) < m_xi) {
        Eg = integrate(m_pIntegralxSmall1EgSea, 0., Beta2, emptyParameters);
        Eg += integrate(m_pIntegralxSmall2EgSea, 0., Beta2Mx, emptyParameters);
    }

    if (m_x <= -m_xi) {
        // Integration
        Eg = integrate(m_pIntegralxLargeEgSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    GluonDistribution gluonDistribution(Eg);

// s quark (sea)

    // s quark
    double Es = 0;

    if (m_x >= m_xi) {
        Es = integrate(m_pIntegralxLargeEsSea, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        Es = integrate(m_pIntegralxSmallEsSea, Eps, Beta2Min, emptyParameters);
        Es += integrate(m_pIntegralxSmall1EsSea, Beta2Min, Beta2,
                emptyParameters);
        Es -= integrate(m_pIntegralxSmall2EsSea, Beta2Min, Beta2Mx,
                emptyParameters);

    }

    if (m_x <= -m_xi) {
        Es = -integrate(m_pIntegralxLargeEsSeaMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    quarkDistribution_s.setQuarkDistribution(Es);
    // u-d quark, valence part evaluated at fx

    double EuVal = 0.;
    double EdVal = 0.;

    if (m_x >= m_xi) {
        // Integration, u quark
        EuVal = integrate(m_pIntegralEuVal, Beta1, Beta2, emptyParameters);
        EdVal = integrate(m_pIntegralEdVal, Beta1, Beta2, emptyParameters);
        // Integration, d quark
        // HdVal = integrate(m_pIntegralHdVal, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        EuVal = integrate(m_pIntegralEuVal, Eps, Beta2, emptyParameters);
        EdVal = integrate(m_pIntegralEdVal, Eps, Beta2, emptyParameters);

        // Integration, d quark
        //HdVal = integrate(m_pIntegralHdVal, Eps, Beta2, emptyParameters);
    }

    ///////////////////////////////////////////////////////////////////////
    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    ///////////////////////////////////////////////////////////////////////

    double EuValMx = 0.;
    double EdValMx = 0.;

    if (m_Mx >= m_xi) {

        // Integration, u quark
        EuValMx = integrate(m_pIntegralEuValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        EdValMx = integrate(m_pIntegralEdValMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    if (fabs(m_Mx) < m_xi) {
        // Integration, u quark
        EuValMx = integrate(m_pIntegralEuValMx, Eps, Beta2Mx, emptyParameters);

        // Integration, d quark
        EdValMx = integrate(m_pIntegralEdValMx, Eps, Beta2Mx, emptyParameters);
    }

// u and d quarks, sea part

//EuSea = Es
    double uSea = quarkDistribution_s.getQuarkDistribution();
//EdSea = Es
    double dSea = quarkDistribution_s.getQuarkDistribution();

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(EuVal + uSea);
    quarkDistribution_d.setQuarkDistribution(EdVal + dSea);

// Set Eq(+)
    quarkDistribution_u.setQuarkDistributionPlus(EuVal - EuValMx + 2 * uSea);
    quarkDistribution_d.setQuarkDistributionPlus(EdVal - EdValMx + 2 * dSea);
    quarkDistribution_s.setQuarkDistributionPlus(
            2 * quarkDistribution_s.getQuarkDistribution());

// Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(EuVal + EuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(EdVal + EdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

PartonDistribution GK16NumericalModel::computeEt() {
    PartonDistribution partonDistribution;

    QuarkDistribution quarkDistribution_u(QuarkFlavor::UP);
    QuarkDistribution quarkDistribution_d(QuarkFlavor::DOWN);
    QuarkDistribution quarkDistribution_s(QuarkFlavor::STRANGE);

    double Eps = 1.e-9;
    m_Mx = -m_x;

// Integration limits and methods
    double Beta1 = (m_x - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2 = (m_x + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta1Mx = (m_Mx - m_xi) / (1. - m_xi); // eq. (54) in A. Radyushkin's paper
    double Beta2Mx = (m_Mx + m_xi) / (1. + m_xi); // eq. (54) in A. Radyushkin's paper

    double Beta2Min = std::min<double>(Beta2, Beta2Mx);
    std::vector<double> emptyParameters;

    double Nu, Nd;
    double PionPolePx, PionPoleMx;

    PionPolePx = Et_pole(m_x);
    PionPoleMx = Et_pole(-m_x);

// Gluons,  Et_gluons = 0 for GK
    GluonDistribution gluonDistribution(0.);

// s quark,  Et_sea = 0 for GK
    quarkDistribution_s.setQuarkDistributionPlus(0.);

    // u-d quark, valence part evaluated at fx

    double EtuVal = 0.;
    double EtdVal = 0.;

    if (m_x >= m_xi) {
        // Integration, u quark
        EtuVal = integrate(m_pIntegralEtuVal, Beta1, Beta2, emptyParameters);
        EtdVal = integrate(m_pIntegralEtdVal, Beta1, Beta2, emptyParameters);
        // Integration, d quark
        // HdVal = integrate(m_pIntegralHdVal, Beta1, Beta2, emptyParameters);
    }

    if (fabs(m_x) < m_xi) {
        // Integration, u quark
        EtuVal = integrate(m_pIntegralEtuVal, Eps, Beta2, emptyParameters);
        EtdVal = integrate(m_pIntegralEtdVal, Eps, Beta2, emptyParameters);

        // Integration, d quark
        //HdVal = integrate(m_pIntegralHdVal, Eps, Beta2, emptyParameters);
    }

    ///////////////////////////////////////////////////////////////////////
    //   u and d quarks, valence part evaluated at fMx (instead of fx)   //
    ///////////////////////////////////////////////////////////////////////

    double EtuValMx = 0.;
    double EtdValMx = 0.;

    if (m_Mx >= m_xi) {

        // Integration, u quark
        EtuValMx = integrate(m_pIntegralEtuValMx, Beta1Mx, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        EtdValMx = integrate(m_pIntegralEtdValMx, Beta1Mx, Beta2Mx,
                emptyParameters);
    }

    if (fabs(m_Mx) < m_xi) {
        // Integration, u quark
        EtuValMx = integrate(m_pIntegralEtuValMx, Eps, Beta2Mx,
                emptyParameters);

        // Integration, d quark
        EtdValMx = integrate(m_pIntegralEtdValMx, Eps, Beta2Mx,
                emptyParameters);
    }

    EtuVal += PionPolePx;
    EtuValMx += PionPoleMx;
    EtdVal -= PionPolePx;
    EtdValMx -= PionPoleMx;

// u and d quark, sea part
    double uSea = 0.;
    double dSea = 0.;

// u and d quarks, valence + sea parts

    quarkDistribution_u.setQuarkDistribution(EtuVal + uSea);
    quarkDistribution_d.setQuarkDistribution(EtdVal + dSea);

// Set Etq(+)
    quarkDistribution_u.setQuarkDistributionPlus(EtuVal + EtuValMx);
    quarkDistribution_d.setQuarkDistributionPlus(EtdVal + EtdValMx);
    quarkDistribution_s.setQuarkDistributionPlus(0.);

// Set Eq(-)
    quarkDistribution_u.setQuarkDistributionMinus(EtuVal - EtuValMx);
    quarkDistribution_d.setQuarkDistributionMinus(EtdVal - EtdValMx);
    quarkDistribution_s.setQuarkDistributionMinus(0.);

    partonDistribution.setGluonDistribution(gluonDistribution);
    partonDistribution.addQuarkDistribution(quarkDistribution_u);
    partonDistribution.addQuarkDistribution(quarkDistribution_d);
    partonDistribution.addQuarkDistribution(quarkDistribution_s);

    return partonDistribution;
}

double GK16NumericalModel::Et_pole(double x) {

//TODO extraire les constantes si existantes

    double dum = 0.; //  Initialize to 0
    double eps;
    double eps2;
    double tmin;
    double xbj;
    double Q2 = m_MuF2;
    double tOverQ2 = m_t / Q2;
    double y = (x + m_xi) / 2. / m_xi;
    double MPi2 = PI_ZERO_MASS * PI_ZERO_MASS;
    double gpiNN = 13.4;
    double f_pi = 0.131; // f_pi=0.131 GeV from paper. Actually 130.4 from PDG (Jul2010)
    double Lambda_N2 = 0.51 * 0.51;
    double Fp;
    double FpiNN;

    xbj = 2. * m_xi / (m_xi - tOverQ2 * m_xi + 1. + tOverQ2 * 0.5);
    eps = 2. * xbj * PROTON_MASS / sqrt(Q2);
    eps2 = eps * eps;

    if (eps < 1 && (4. * xbj * (1. - xbj) + eps2) != 0) {

        tmin = -Q2 * (2. * (1. - xbj) * (1 - sqrt(1. + eps2)) + eps2)
                / (4. * xbj * (1. - xbj) + eps2);
        FpiNN = (Lambda_N2 - MPi2) / (Lambda_N2 - (m_t - tmin));
        Fp = -PROTON_MASS * f_pi * (2. * sqrt(2.) * gpiNN * FpiNN)
                / (m_t - MPi2);

        if (x < m_xi && x > -m_xi && m_t < tmin) {
//          dum = ( Fp( pKinematicVariables->getT(), Q2, tmin ) / 4. / pGPDData->getXi() )  *  Phi_pi( ( x + pGPDData->getXi() ) / 2. / pGPDData->getXi() ) ;
            dum = (Fp / 4. / m_xi) * 6. * y * (1. - y);
        }

    }

    return dum;
}

std::string GK16NumericalModel::toString() const {
    return GPDModule::toString();
}
