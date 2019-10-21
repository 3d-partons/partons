#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06.h"

#include <ElementaryUtils/logger/CustomException.h>
#include <ElementaryUtils/string_utils/Formatter.h>
#include <gsl/gsl_monte.h>
#include <gsl/gsl_monte_vegas.h>
#include <gsl/gsl_rng.h>
#include <stddef.h>
#include <cmath>
#include <utility>

#include "../../../../../include/partons/beans/gpd/GPDKinematic.h"
#include "../../../../../include/partons/beans/MesonPolarization.h"
#include "../../../../../include/partons/beans/MesonType.h"
#include "../../../../../include/partons/beans/parton_distribution/GluonDistribution.h"
#include "../../../../../include/partons/beans/parton_distribution/QuarkDistribution.h"
#include "../../../../../include/partons/beans/PerturbativeQCDOrderType.h"
#include "../../../../../include/partons/BaseObjectRegistry.h"
#include "../../../../../include/partons/modules/convol_coeff_function/DVMP/DVMPCFFGK06Integration.h"
#include "../../../../../include/partons/modules/gpd/GPDModule.h"

namespace PARTONS {

const unsigned int DVMPCFFGK06::classId =
        BaseObjectRegistry::getInstance()->registerBaseObject(
                new DVMPCFFGK06("DVMPCFFGK06"));

DVMPCFFGK06::DVMPCFFGK06(const std::string &className) :
        DVMPConvolCoeffFunctionModule(className), m_cNf(3.), m_cLambdaQCD(0.22) {

    //relate GPD types with functions to be used
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::H,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::E,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Ht,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
    m_listOfCFFComputeFunctionAvailable.insert(
            std::make_pair(GPDType::Et,
                    &DVMPConvolCoeffFunctionModule::computeCFF));
}

DVMPCFFGK06::DVMPCFFGK06(const DVMPCFFGK06 &other) :
        DVMPConvolCoeffFunctionModule(other), m_cNf(other.m_cNf), m_cLambdaQCD(
                other.m_cLambdaQCD) {
}

DVMPCFFGK06* DVMPCFFGK06::clone() const {
    return new DVMPCFFGK06(*this);
}

DVMPCFFGK06::~DVMPCFFGK06() {
}

void DVMPCFFGK06::resolveObjectDependencies() {
    DVMPConvolCoeffFunctionModule::resolveObjectDependencies();
}

void DVMPCFFGK06::initModule() {
    DVMPConvolCoeffFunctionModule::initModule();
}

void DVMPCFFGK06::isModuleWellConfigured() {
    DVMPConvolCoeffFunctionModule::isModuleWellConfigured();
}

std::complex<double> DVMPCFFGK06::computeCFF() {

    //check pQCD
    if (m_qcdOrderType != PerturbativeQCDOrderType::LO) {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "QCD order: "
                        << PerturbativeQCDOrderType(m_qcdOrderType).toString()
                        << " not implemented");
    }

    //check meson
    if (m_mesonType == MesonType::RHO0) {

        std::complex<double> cff_g = gluonIntegratedAmplitude(GPDType::H)
                + gluonIntegratedAmplitude(GPDType::E);
        std::complex<double> cff_u = quarkIntegratedAmplitude(GPDType::H,
                QuarkFlavor::UP)
                + quarkIntegratedAmplitude(GPDType::E, QuarkFlavor::UP);
        std::complex<double> cff_d = quarkIntegratedAmplitude(GPDType::H,
                QuarkFlavor::DOWN)
                + quarkIntegratedAmplitude(GPDType::E, QuarkFlavor::DOWN);
        std::complex<double> cff_s = quarkIntegratedAmplitude(GPDType::H,
                QuarkFlavor::STRANGE)
                + quarkIntegratedAmplitude(GPDType::E, QuarkFlavor::STRANGE);

        //TODO implement sum from Eq. (3, 5, 6) from https://arxiv.org/pdf/hep-ph/0611290.pdf
        //TODO integration done in gluonIntegratedAmplitude() and quarkIntegratedAmplitude()
        return 0.;

    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type: "
                        << MesonType(m_mesonType).toString()
                        << " not implemented");
    }

    return 0.;
}

double DVMPCFFGK06::alphaS() const {

    //TODO implementation of one loop alpha_s, variables:
    m_cNf;
    m_cLambdaQCD;
    m_MuR2;
}

double DVMPCFFGK06::sudakovFactor(double tau, double b) const {

    //sqrt of Q2
    double Q = sqrt(m_Q2);

    //beta0 factor
    double beta0 = 11. - 2 * m_cNf / 3.;

    //b^
    double bHat = -1 * log(b * m_cLambdaQCD);

    //Eq. (12) from https://arxiv.org/pdf/hep-ph/0611290.pdf
    return sudakovFactorFunctionS(tau, b) + sudakovFactorFunctionS(1. - tau, b)
            - (4. / beta0) * log(log(m_MuR2 / m_cLambdaQCD) / bHat);
}

double DVMPCFFGK06::sudakovFactorFunctionS(double tau, double b) const {

    double Q = sqrt(m_Q2);

    //TODO implementation of Sudakov factor function s, like Eq. (14) from https://arxiv.org/pdf/hep-ph/0611290.pdf
    return 0.;
}

double DVMPCFFGK06::mesonWF(double tau, double b) const {

    //TODO define parameters and use given parameterization.

    if (m_mesonType == MesonType::RHO0) {

        double f;
        double a;

        if (m_mesonPolarization == MesonPolarization::L) {

            f = 0.216; //from Eq. (49) of https://arxiv.org/pdf/hep-ph/0501242.pdf
            a = 0.52;  //from Eq. (51) of ...
        } else if (m_mesonPolarization == MesonPolarization::T) {

            f = 0.250; //from Eq. (50) of https://arxiv.org/pdf/hep-ph/0501242.pdf
            a = 0.65;  //from Eq. (52) of ...
        } else {
            throw ElemUtils::CustomException(getClassName(), __func__,
                    ElemUtils::Formatter() << "Polarization: "
                            << MesonPolarization(m_mesonPolarization).toString()
                            << " for meson: "
                            << MesonType(m_mesonType).toString()
                            << " not implemented");
        }

        return mesonWFGaussian(tau, b, f, a);

    } else {
        throw ElemUtils::CustomException(getClassName(), __func__,
                ElemUtils::Formatter() << "Meson type: "
                        << MesonType(m_mesonType).toString()
                        << " not implemented");
    }

    return 0.;
}

double DVMPCFFGK06::mesonWFGaussian(double tau, double b, double f,
        double a) const {

    //TODO: like Eq. (41) from https://arxiv.org/pdf/hep-ph/0611290.pdf, but in b instead of k_perp.
    return 0.;
}

double DVMPCFFGK06::quarkPropagator(double x, double tau, double b,
        GPDType::Type gpdType) const {

    //TODO propagators - implement cases depending on:
    //TODO meson type (different for vector and pseudoscalar)
    //TODO (TO BE CHECKED) target helicity combination (e.g. for vector mesons: different for H and E)
    //TODO (TO BE CHECKED) meson polarization (e.g. for vector mesons: different for L and T)
    return 0.;
}

double DVMPCFFGK06::gluonPropagator(double x, double tau, double b,
        GPDType::Type gpdType) const {

    //TODO propagators - implement cases depending on:
    //TODO meson type (different for vector and pseudoscalar)
    //TODO (TO BE CHECKED) target helicity combination (e.g. for vector mesons: different for H and E)
    //TODO (TO BE CHECKED) meson polarization (e.g. for vector mesons: different for L and T)
    return 0.;
}

double DVMPCFFGK06::quarkUnintegratedAmplitude(double x, double tau, double b,
        GPDType::Type gpdType, QuarkFlavor::Type quarkType) const {

    //Eqs. (6, 10) from from https://arxiv.org/pdf/hep-ph/0611290.pdf
    //here for GPDs we get singlet combination, so we need to integrate between (0, 1) only.
    return mesonWF(tau, b) * quarkPropagator(x, tau, b, gpdType) * alphaS()
            * exp(-1 * sudakovFactor(tau, b))
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    gpdType).getQuarkDistribution(quarkType).getQuarkDistributionPlus();
}

double DVMPCFFGK06::gluonUnintegratedAmplitude(double x, double tau, double b,
        GPDType::Type gpdType) const {

    //Eqs (5, 10) from from https://arxiv.org/pdf/hep-ph/0611290.pdf
    return mesonWF(tau, b) * gluonPropagator(x, tau, b, gpdType) * alphaS()
            * exp(-1 * sudakovFactor(tau, b))
            * m_pGPDModule->compute(GPDKinematic(x, m_xi, m_t, m_MuF2, m_MuR2),
                    gpdType).getGluonDistribution().getGluonDistribution();
}

double DVMPCFFGK06::quarkIntegratedAmplitude(GPDType::Type gpdType,
        QuarkFlavor::Type quarkType) const {

    //parameters
    DVMPCFFGK06IntegrationParameters params;

    params.m_pDVMPCFFGK06 = this;
    params.m_gpdType = gpdType;
    params.m_quarkType = quarkType;

    //range (x, tau, b)
    //TODO max b?
    double rangeMin[3] = { 0., 0., 0. };
    double rangeMax[3] = { 1., 1., 10. };

    //result and error
    double result, error;

    //number of calls
    const size_t nCalls = 100;

    //random generator
    gsl_rng* gslRnd;

    //function
    gsl_monte_function gslFunction;

    gslFunction.f = &DVMPCFFGK06IntegrationFunctionQuark;
    gslFunction.dim = 3;
    gslFunction.params = &params;

    //state
    gsl_monte_vegas_state* gslState = gsl_monte_vegas_alloc(3);

    //integrate
    gsl_monte_vegas_integrate(&gslFunction, rangeMin, rangeMax, 3, nCalls,
            gslRnd, gslState, &result, &error);

    //free
    gsl_monte_vegas_free(gslState);
    gsl_rng_free(gslRnd);
}

double DVMPCFFGK06::gluonIntegratedAmplitude(GPDType::Type gpdType) const {
    return 0.;
}

} /* namespace PARTONS */
